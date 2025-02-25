#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

const int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const std::vector<std::pair<int, int>> directions = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};
// Shuffle directions for randomness
void shuffleDirections(int directions[8]) {
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> dist(0, 7);
    
    for (int i = 0; i < 8; i++) {
        int r = dist(g);
        swap(directions[i], directions[r]);
    }
}

bool isInsideMaze(int x, int y, int n, int m) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

void generateMazeWallsDFS(int x, int y, vector<vector<char>>& maze) {
    maze[x][y] = '.';  // Mark the current cell as a path
    int n = maze.size();          
    int m = maze[0].size(); 

    int directions[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    shuffleDirections(directions);

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (isInsideMaze(nx, ny, n, m) && maze[nx][ny] == '#') {
            // Remove wall between current and next cell
            maze[x + dx[directions[i]]][y + dy[directions[i]]] = '.';
            generateMazeWallsDFS(nx, ny, maze);
        }
    }

    
}


bool dfs(int x, int y, int index, const std::string& word, vector<vector<char>>& M, vector<pair<int, int>>& path) {
    if (index == word.size()) return true;
    vector<pair<int, int>> shuffledDirections = directions;
    // Shuffle directions to ensure randomness
    random_device rd;
    mt19937 rng(rd());
    std::shuffle(shuffledDirections.begin(), shuffledDirections.end(), rng);
    
    for (const auto& [dx, dy] : shuffledDirections) {
        int newX = x + dx;
        int newY = y + dy;

        if (isInsideMaze(newX, newY, M.size(), M[0].size()) && M[newX][newY] == '.') {
            // Mark the cell as used
            M[newX][newY] = word[index];
            path.push_back({newX, newY});

            if (dfs(newX, newY, index + 1, word, M, path)) {
                return true;
            }

            // Backtrack
            M[newX][newY] = '.';
            path.pop_back();
        }
    }

    return false;
}
vector<vector<char>> placeWordsDFS(const vector<string>& list, vector<vector<char>>& M) {
    int n = M.size(), m = M[0].size();
    srand(static_cast<unsigned int>(std::time(0)));

    for (const string& word : list) {
        // Create a copy of M for tracking availability
        vector<vector<char>> M2 = M;
        bool placed = false;

        // Check if there's still a possible empty cell to place words
        bool possible = false;
        for (int i = 0; i < n && !possible; i++) {
            for (int j = 0; j < m && !possible; j++) {
                if (M2[i][j] == '.') {
                    possible = true;
                }
            }
        }

        // Attempt to place the word if possible
        while (!placed && possible) {
            int x = rand() % n;
            int y = rand() % m;

            // Ensure starting point is empty
            if (M[x][y] != '.'){
                continue;
            }
            //test possiblity 
            
            // Check if there are still free cells in M2
                possible = false;
                for (int i = 0; i < n && !possible; i++) {
                    for (int j = 0; j < m && !possible; j++) {
                        if (M2[i][j] == '.') {
                            possible = true;
                        }
                    }
                }
                
            std::vector<std::pair<int, int>> path;
            M[x][y] = word[0];  // Place the first letter
            M2[x][y] = '#';
            path.push_back({x, y});

            if (dfs(x, y, 1, word, M, path)) {
                placed = true;
            } else {
                // Reset if placement failed
                for (const auto& [px, py] : path) {
                    M[px][py] = '.';
                }
            }
        }
    }
    return M;
}

vector<vector<char>> generateMaze(vector<string> list, int d) {
    
    int n = (d-1)*10+7;
    int m = (d-1)*10+7;

     srand(time(0));
    
    vector<vector<char>> maze(n, vector<char>(m, '#'));
  //  maze = generateMazeWallsDFS(1, 1, maze);
    generateMazeWallsDFS(1, 1, maze);
    
    // Print maze 
    // for (const auto& row : maze) {
    //     for (char cell : row) {
    //         cout << cell << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl <<endl;

    maze = placeWordsDFS(list,maze);
    //Print maze 
    // for (const auto& row : maze) {
    //     for (char cell : row) {
    //         cout << cell << " ";
    //     }
    //     cout << endl;
    // }
    //  cout << endl << endl;
    // filling missing cells with random letters
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (maze[i][j] == '.') {
                maze[i][j] = static_cast<char>(std::rand() % 26 + 'a');
            }
        }
    }
   // cout << endl <<endl;

    //Print maze 
    // for (const auto& row : maze) {
    //     for (char cell : row) {
    //         cout << cell << " ";
    //     }
    //     cout << endl;
    // }
    return maze;
}