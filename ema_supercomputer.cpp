#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'twoPluses' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts STRING_ARRAY grid as parameter.
 */

int GetMaxValidCross(vector<string>& grid, int x, int y, pair<int, int> direction)
{
    int rows = grid.size();
    int cols = grid[0].size();
    int length = 0;
    while(0 <= x && x < rows && 0 <= y && y < cols) {
        if (grid[x][y] == 'G') {
            length += 1;
        }
        else {
            break;
        }
        x = x + direction.first;
        y = y + direction.second;
    }
    return length - 1;
}

int GetMaxProduct(vector<int> cross1, vector<int> cross2) {
    int x1 = cross1[0];
    int y1 = cross1[1];
    int l1 = cross1[2];
    int x2 = cross2[0];
    int y2 = cross2[1];
    int l2 = cross2[2];
    set<pair<int, int>> s1;
    int max_product = 0;
  
    // Just get product of 2 non-overlapping cross
    for (int i = 0; i <= l1; ++i) {
        s1.insert(make_pair(x1+i, y1));
        s1.insert(make_pair(x1-i, y1));
        s1.insert(make_pair(x1, y1-i));
        s1.insert(make_pair(x1, y1+i));
        for (int j = 0; j <= l2; ++j) {
            if (s1.find(make_pair(x2+j, y2)) == s1.end() 
            && s1.find(make_pair(x2-j, y2)) == s1.end()
            && s1.find(make_pair(x2, y2+j)) == s1.end()
            && s1.find(make_pair(x2, y2-j)) == s1.end()){
                int product = (4 * i + 1) * (4 * j + 1);
                if (product > max_product) max_product = product;
            }
            else {
                break;
            }
        }
    }
    return max_product;
}

// Time complexity: O(rows^2 * cols^2 in worst case)
// Space complexity: O(rows + cols)
int twoPluses(vector<string> grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<int>> crosses;
    vector<pair<int, int>> coords = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 'G') {
                int min_length = 100;
                for (auto& c: coords) {
                    int length = GetMaxValidCross(grid, i, j, c);
                    if (length != 0) {
                        min_length = min(min_length, length);
                    }
                    else {
                        min_length = 0;
                        break;
                    }
                }
                
                crosses.push_back({i, j, min_length});
            }
        }
    }
    int max_final = 0;
    for (int i = 0; i < crosses.size(); ++i) {
        for (int j = i+1; j < crosses.size(); ++j){
            int max_product = GetMaxProduct(crosses[i], crosses[j]);
            if (max_product > max_final) max_final = max_product;
        }
    }
    return max_final;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    vector<string> grid(n);

    for (int i = 0; i < n; i++) {
        string grid_item;
        getline(cin, grid_item);

        grid[i] = grid_item;
    }

    int result = twoPluses(grid);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
