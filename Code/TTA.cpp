#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
#include <memory>

using namespace std;

// Define the game board
vector<vector<int>> board = {
    {71, 1, 1, 71, 711, 711, 17, 711, 71, 1, 71, 17, 71, 711, 711, 1},
    {71, 711, 17, 71, 711, 17, 1, 71, 17, 17, 711, 711, 17, 711, 1, 71},
    {17, 711, 1, 711, 71, 1, 711, 17, 711, 1, 711, 17, 71, 1, 17, 711},
    {1, 71, 1, 71, 17, 711, 71, 71, 17, 711, 1, 17, 71, 711, 71, 0}, // Note: Added 0 to make it 16 elements
    {1, 71, 711, 71, 17, 1, 711, 17, 71, 1, 71, 17, 71, 711, 71, 17},
    {71, 711, 1, 71, 17, 1, 711, 1, 711, 71, 1, 71, 17, 711, 71, 0}, // Note: Added 0 to make it 16 elements
    {71, 1, 711, 17, 17, 1, 711, 17, 71, 1, 711, 17, 1, 711, 711, 17},
    {1, 711, 1, 17, 711, 71, 711, 1, 71, 17, 711, 71, 711, 17, 17, 1}
};

struct MoveResult {
    vector<int> next_pose;
    pair<int, int> next_position;
};

struct PathInfo {
    int sum_value;
    vector<int> pose;
    vector<pair<int, int>> path;
};

MoveResult move(const vector<int>& cur_pose, int direction, const pair<int, int>& cur_index) {
    vector<int> next_pose(5);
    pair<int, int> next_position;
    
    if (cur_pose[0]) { // bottom triangle pointing up, i.e. mode 1
        if (direction == 1) {
            next_pose = {0, cur_pose[2], cur_pose[4], cur_pose[3], cur_pose[1]};
            next_position = {cur_index.first, cur_index.second - 1};
        } else if (direction == 2) {
            next_pose = {0, cur_pose[1], cur_pose[3], cur_pose[4], cur_pose[2]};
            next_position = {cur_index.first, cur_index.second + 1};
        } else { // direction == 3
            next_pose = {0, cur_pose[4], cur_pose[2], cur_pose[1], cur_pose[3]};
            next_position = {cur_index.first + 1, cur_index.second + 1};
        }
    } else { // pointing down, i.e. mode 0
        if (direction == 1) {
            next_pose = {1, cur_pose[3], cur_pose[2], cur_pose[4], cur_pose[1]};
            next_position = {cur_index.first - 1, cur_index.second - 1};
        } else if (direction == 2) {
            next_pose = {1, cur_pose[4], cur_pose[1], cur_pose[3], cur_pose[2]};
            next_position = {cur_index.first, cur_index.second + 1};
        } else { // direction == 3
            next_pose = {1, cur_pose[1], cur_pose[4], cur_pose[2], cur_pose[3]};
            next_position = {cur_index.first, cur_index.second - 1};
        }
    }
    
    return {next_pose, next_position};
}

int get_item(int i, int j) {
    if (i < 0 || i >= board.size() || j < 0 || j >= board[i].size()) {
        return -1;
    }
    return board[i][j];
}

vector<int> find_possible_direction(const vector<int>& cur_pose, const pair<int, int>& cur_index) {
    vector<int> possible_directions;
    
    if (cur_pose[0]) { // pointing up
        // direction 1
        if (get_item(cur_index.first, cur_index.second - 1) == cur_pose[1]) {
            possible_directions.push_back(1);
        }
        // direction 2
        if (get_item(cur_index.first, cur_index.second + 1) == cur_pose[2]) {
            possible_directions.push_back(2);
        }
        // direction 3
        if (get_item(cur_index.first + 1, cur_index.second + 1) == cur_pose[3]) {
            possible_directions.push_back(3);
        }
    } else { // pointing down
        // direction 1
        if (get_item(cur_index.first - 1, cur_index.second - 1) == cur_pose[1]) {
            possible_directions.push_back(1);
        }
        // direction 2
        if (get_item(cur_index.first, cur_index.second + 1) == cur_pose[2]) {
            possible_directions.push_back(2);
        }
        // direction 3
        if (get_item(cur_index.first, cur_index.second - 1) == cur_pose[3]) {
            possible_directions.push_back(3);
        }
    }
    
    return possible_directions;
}

bool direction_indicator(const vector<vector<vector<int>>>& ds) {
    for (const auto& row : ds) {
        for (const auto& cell : row) {
            if (!cell.empty() && cell != vector<int>{-1}) {
                return true;
            }
        }
    }
    return false;
}

bool update_indicator(const pair<int, int>& cur_index, const pair<int, int>& dest_index, 
                     const vector<vector<shared_ptr<PathInfo>>>& path_store) {
    if (path_store[dest_index.first][dest_index.second] == nullptr) {
        return true;
    } else if (path_store[dest_index.first][dest_index.second]->sum_value > 
               path_store[cur_index.first][cur_index.second]->sum_value + 
               board[dest_index.first][dest_index.second]) {
        return true;
    } else {
        return false;
    }
}

vector<vector<shared_ptr<PathInfo>>> evolve(const vector<int>& ini_pose, const pair<int, int>& ini_index) {
    // Initialize path_store and direction_store
    vector<vector<shared_ptr<PathInfo>>> path_store(8, vector<shared_ptr<PathInfo>>(16, nullptr));
    vector<vector<vector<int>>> direction_store(8, vector<vector<int>>(16, {-1}));
    
    // Initialize starting position
    auto initial_path_info = make_shared<PathInfo>();
    initial_path_info->sum_value = board[ini_index.first][ini_index.second];
    initial_path_info->pose = ini_pose;
    initial_path_info->path = {ini_index};
    
    path_store[ini_index.first][ini_index.second] = initial_path_info;
    direction_store[ini_index.first][ini_index.second] = find_possible_direction(ini_pose, ini_index);
    
    // Main evolution loop
    while (direction_indicator(direction_store)) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 16; j++) {
                if (!direction_store[i][j].empty() && direction_store[i][j] != vector<int>{-1}) {
                    while (!direction_store[i][j].empty()) {
                        int d = direction_store[i][j].back();
                        direction_store[i][j].pop_back();
                        
                        auto [dest_pose, dest] = move(path_store[i][j]->pose, d, {i, j});
                        
                        if (update_indicator({i, j}, dest, path_store)) {
                            auto new_path_info = make_shared<PathInfo>();
                            new_path_info->sum_value = path_store[i][j]->sum_value + board[dest.first][dest.second];
                            new_path_info->pose = dest_pose;
                            new_path_info->path = path_store[i][j]->path;
                            new_path_info->path.push_back(dest);
                            
                            path_store[dest.first][dest.second] = new_path_info;
                            direction_store[dest.first][dest.second] = find_possible_direction(dest_pose, dest);
                        }
                        
                        if (path_store[dest.first][dest.second] != nullptr && 
                            dest_pose != path_store[dest.first][dest.second]->pose) {
                            cout << "two poses found, need more considerations" << endl;
                            return path_store;
                        }
                    }
                }
            }
        }
    }
    
    return path_store;
}

void show(const vector<vector<shared_ptr<PathInfo>>>& b) {
    for (const auto& row : b) {
        vector<string> tmp;
        for (const auto& cell : row) {
            if (cell == nullptr) {
                tmp.push_back("-");
            } else {
                tmp.push_back(to_string(cell->sum_value));
            }
        }
        for (const auto& val : tmp) {
            cout << val << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> generate_permutations(const vector<int>& elements) {
    vector<vector<int>> result;
    vector<int> temp = elements;
    sort(temp.begin(), temp.end());
    
    do {
        result.push_back(temp);
    } while (next_permutation(temp.begin(), temp.end()));
    
    return result;
}

int main() {
    // Generate initial poses
    vector<int> base_elements = {1, 17, 71, 711};
    vector<vector<int>> initial_pose;
    
    auto permutations = generate_permutations(base_elements);
    for (const auto& perm : permutations) {
        vector<int> pose = {0}; // mode 0
        pose.insert(pose.end(), perm.begin(), perm.end());
        initial_pose.push_back(pose);
    }
    
    // The main solution
    for (int i = 1; i < 16; i += 2) {
        cout << "processing position " << i << endl;
        for (const auto& p : initial_pose) {
            if (p[4] == board[0][i]) {
                auto possible_dirs = find_possible_direction(p, {0, i});
                if (!possible_dirs.empty()) {
                    cout << "current initial pose is ";
                    for (int val : p) {
                        cout << val << " ";
                    }
                    cout << endl;
                    
                    auto result = evolve(p, {0, i});
                    show(result);
                    cout << endl;
                }
            }
        }
    }
    
    return 0;
}