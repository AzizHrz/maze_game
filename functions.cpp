// functions.cpp
#include "functions.hpp" // Include the header file
#include "cell.hpp"

 //vector<vector<int>> direction = {{0,-1} , {0,1}, {-1,0} , {1,0} ,
       // {1,1} , {1,-1} , {-1,-1},{-1,1}};

vector<vector<int>> direction = {{1,-1} , {1,0}, {1,1} , {0,-1} ,
{0,1} , {-1,-1} , {-1,0},{-1,1}};  
    

bool verfier_colonne_ligne(int a,int b, int n , int m  ){
    if(a>=1 && a<= n && b>=1 && b<=m ) return true;
    return false;
}

vector<pair<int, int>> reconstructPath(pair<int, int> start, pair<int, int> end, vector<vector<pair<int, int>>>& predecessor) {
    vector<pair<int, int>> path;
    pair<int, int> current = end;

    while (current!= start) {
        path.push_back(current);
        current = predecessor[current.first][current.second];
       
    }
    path.push_back(start);

    reverse(path.begin(), path.end());
    return path;
}

pair<int,vector<pair<int,int>>> dijkstra( vector<vector<char>>& grille, vector<vector<int>>& cost_grid, pair<int, int> start, pair<int, int> end) {
    int rows = grille.size()-1;
    int cols = grille[0].size()-1;
    vector<vector<pair<int, int>>> predecessor(rows+5, vector<pair<int, int>>(cols+5, {-1, -1}));

    // Matrice de distances minimales
    vector<vector<int>> dist(rows+5, vector<int>(cols+5, INT_MAX));
    dist[start.first][start.second] = cost_grid[start.first][start.second];
    
    // File de priorité (min-heap)
    priority_queue<Cell, vector<Cell>, greater<Cell>> pq;
    pq.push(Cell(start.first, start.second, 0));
    
    while (!pq.empty()) {

        Cell current = pq.top();
        pq.pop();
        
        // Si on atteint la destination
        if (current.x == end.first && current.y == end.second) {
            vector<pair<int,int>> reconstructed_path = reconstructPath(start, end, predecessor);

            return {current.cost, reconstructed_path}  ;
        }
        
        // Explorer les voisins
        for (int i = 0; i < 8; ++i) {
            int nx = current.x + direction[i][0];
            int ny = current.y + direction[i][1];
            
            // Vérifier les limites de la grille
            if (nx >= 1 && nx <=rows && ny >= 1 && ny <= cols && grille[nx][ny]!='#') {
                int new_cost = current.cost + cost_grid[nx][ny];
                
                // Mettre à jour si meilleur chemin trouvé
                if (new_cost < dist[nx][ny]) {
                    dist[nx][ny] = new_cost;
                    pq.push(Cell(nx, ny, new_cost));
                    predecessor[nx][ny] = {current.x, current.y};
                }
            }
        }
    }
    
    return {-1 , {{-1,-1}}}; // Pas de chemin trouvé
}
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to check if a cell is valid
bool isValid(int x, int y, int n, int m, const vector<vector<char>>& grid, const vector<vector<int>>& dist) {
    return x >= 1 && x <= n && y >= 1 && y <= m && grid[x][y] != '#' && dist[x][y] == -1;
}

// BFS to compute shortest distances
void bfs(const vector<vector<char>>& grid, vector<vector<int>>& dist, pair<int, int> start) {
    int n = grid.size()-1;
    int m = grid[0].size()-1;
    queue<pair<int, int>> q;

    // Initialize distances
    dist[start.first][start.second] = 0;
    q.push(start);

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // Explore neighbors
        for (int i = 0; i < 8; i++) {
            int nx = x + direction[i][0];
            int ny = y + direction[i][1];

            if (isValid(nx, ny, n, m, grid, dist)) {
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
}

// Backtracking to find all shortest paths
void findAllPaths( vector<vector<char>>& grid, vector<vector<int>>& cost,  vector<vector<int>>& dist, vector<vector<pair<int, int>>>& paths, vector<pair<int, int>>& currentPath, int x, int y, pair<int, int> start) {
    int n = grid.size()-1;
    int m = grid[0].size()-1;

    // Add current cell to the path
    currentPath.push_back({x, y});

    // If we reached the start, save the path
    if (x == start.first && y == start.second) {
        reverse(currentPath.begin(), currentPath.end());
        paths.push_back(currentPath);
        reverse(currentPath.begin(), currentPath.end());
    } else {
        // Explore neighbors with the correct distance
        for (int i = 0; i < 8; i++) {
            int nx = x + direction[i][0];
            int ny = y + direction[i][1];

            if (nx >= 1 && nx <= n && ny >= 1 && ny <= m && dist[nx][ny] == dist[x][y] - cost[x][y]) {
                findAllPaths(grid, cost, dist, paths, currentPath, nx, ny, start);
            }
        }
    }

    // Remove current cell from the path (backtrack)
    currentPath.pop_back();
}

bool dfs_check_path(int x,int y, vector<vector<char>>& maze ,pair<int,int>goal ){
    int row = maze.size(),cols=maze[0].size();
    vector<vector<bool>>visited(row+1,vector<bool>(cols+1,false));
    return dfs_check(x,y,maze,visited,goal);
}
bool dfs_check(int position_x, int position_y, vector<vector<char>>& maze ,vector<vector<bool>>&visited,pair<int,int>goal ){
        visited[position_x][position_y]=true;
        if(goal.first==position_x && goal.second==position_y) {
            return true;
        }
        for(int i = 0 ;i<8;i++){
            int nx = position_x+direction[i][0];
            int ny = position_y+direction[i][1];
            if(verfier_colonne_ligne(nx,ny,maze.size(),maze[i].size()) && !visited[nx][ny] && maze[nx][ny]!='#'){
                if(dfs_check(nx,ny,maze,visited,goal)) return true;
            }
        }
        return false;
}