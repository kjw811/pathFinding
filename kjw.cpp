#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits.h>
#include <cmath>

using namespace std;

vector<vector<char>> grid_init;

void initGrid()
{
    char obstacle = 'x';

    for (int i = 1; i < grid_init.size(); i++)
        grid_init[i][0] = obstacle;
    for (int i = 0; i < grid_init.size()-1; i++)
        grid_init[i][5] = obstacle;
    for (int i = 2; i <= 4; i++)
        grid_init[2][i] = obstacle;
    for (int i = 1; i <= 3; i++)
        grid_init[4][i] = obstacle;
    for (int i = 6; i <= 7; i++)
        grid_init[i][2] = obstacle;
    for (int i = 1; i <= 7; i++)
        grid_init[i][10] = obstacle;
    for (int i = 7; i <= 9; i++)
        grid_init[4][i] = obstacle;
    for (int i = 0; i <= 2; i++)
        grid_init[i][8] = obstacle;
    for (int i = 6; i <= 8; i++)
        grid_init[6][i] = obstacle;
}

void printGrid(const vector<vector<char>>& grid)
{
    int rowSize = grid.size();
    int colSize = grid[0].size();
    for (int i = 0; i < rowSize; i++)
    {
        string s = "";
        s += '|';
        for (int j = 0; j < colSize; j++)
        {
            s += grid[i][j];
            s += '|';
        }
        cout << s << endl;
    }
}

bool canMove(const vector<vector<char>>& grid, const vector<vector<bool>>& visited, int x, int y)
{
    int rowSize = grid.size();
    int colSize = grid[0].size();
    if (x < 0 || y < 0 || x >= colSize || y >= rowSize || grid[y][x] == 'x' || visited[y][x])
        return false;
    return true;
}

void findWay()
{
    int rowSize = grid_init.size();
    int colSize = grid_init[0].size();
    printGrid(grid_init);

    vector<vector<char>>grid = grid_init;

    int s_x = -1, s_y = -1, e_x = -1, e_y = -1;
    while (1)
    {
        cout << "Start X (1~12) : "; cin >> s_x;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        cout << "Start Y (1~8) : "; cin >> s_y;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }

        if (s_x < 1 || s_x > 12 || s_y < 1 || s_y > 8 || grid[s_y-1][s_x-1] == 'x') cout << "Not available. Try again." << endl;
        else break;
    }
    while (1)
    {
        cout << "End X (1~12) : "; cin >> e_x;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        cout << "End Y (1~8) : "; cin >> e_y;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        if (e_x < 1 || e_x > 12 || e_y < 1 || e_y > 8 || grid[e_y-1][e_x-1] == 'x') cout << "Not available. Try again." << endl;
        else break;
    }

    cout << "====================================" << endl;
    cout << "Start  : " << s_x << ", " << s_y << " | End : " << e_x << ", " << e_y << endl;
    s_x--, s_y--, e_x--, e_y--;
    grid[s_y][s_x] = 'S'; grid[e_y][e_x] = 'E';

    vector<vector<bool>> visited_init(rowSize, vector<bool>(colSize, false));
    vector<vector<bool>> visited = visited_init;
    visited[s_y][s_x] = true;

    struct MoveInfo
    {
        MoveInfo(int x, int y, int d) : x(x), y(y), d(d) {}
        int x;
        int y;
        int d;
        bool operator<(MoveInfo& m) { return d > m.d; }
    };
    struct comp
    {
        bool operator()(MoveInfo& m1, MoveInfo& m2) { return m1 < m2; }
    };

    priority_queue<MoveInfo, vector<MoveInfo>, comp> pq;
    pq.push(MoveInfo(s_x, s_y, 0));

    vector<vector<int>> dis(rowSize, vector<int>(colSize, INT_MAX));
    vector<vector<pair<int, int>>> fromInfo(rowSize, vector<pair<int, int>>(colSize, {-1, -1}));
    fromInfo[s_y][s_x] = {s_x, s_y};

    while (!pq.empty())
    {
        int x = pq.top().x;
        int y = pq.top().y;
        int d = pq.top().d;
        pq.pop();

        if (x == e_x && y == e_y)
        {
            cout << "Shortest way length : " << d << endl;
            break;
        }

        if (dis[y][x] < d) continue;
        if (canMove(grid, visited, x, y-1))
        {
            if (d+1 < dis[y-1][x])
            {
                fromInfo[y-1][x] = {x, y};
                dis[y-1][x] = d+1;
                pq.push(MoveInfo(x, y-1, d+1));
            }
        }
        if (canMove(grid, visited, x, y+1))
        {
            if (d+1 < dis[y+1][x])
            {
                fromInfo[y+1][x] = {x, y};
                dis[y+1][x] = d+1;
                pq.push(MoveInfo(x, y+1, d+1));
            }
        }
        if (canMove(grid, visited, x-1, y))
        {
            if (d+1 < dis[y][x-1])
            {
                fromInfo[y][x-1] = {x, y};
                dis[y][x-1] = d+1;
                pq.push(MoveInfo(x-1, y, d+1));
            }
        }
        if (canMove(grid, visited, x+1, y))
        {
            if (d+1 < dis[y][x+1])
            {
                fromInfo[y][x+1] = {x, y};
                dis[y][x+1] = d+1;
                pq.push(MoveInfo(x+1, y, d+1));
            }
        }
    }

    int from_x = e_x;
    int from_y = e_y;
    while (from_x != s_x ||from_y != s_y)
    {
        if (from_x != e_x || from_y != e_y)
            grid[from_y][from_x] = '.';
        int tmp_x = fromInfo[from_y][from_x].first;
        int tmp_y = fromInfo[from_y][from_x].second;
        from_x = tmp_x;
        from_y = tmp_y;
    }

    printGrid(grid);
}

int main()
{
    int rowSize = 8;
    int colSize = 12;
    grid_init.resize(rowSize, vector<char>(colSize, ' '));
    initGrid();

    while (1)
    {
        char c = ' ';
        cout << "Y for path finding, N for exit : ";
        cin >> c;
        if (c == 'N' || c == 'n') return 0;
        else if (c == 'Y' || c == 'y')
        {
            findWay();
        }
    }

    return 0;
}
