#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits.h>
#include <cmath>
#include <chrono>

using namespace std;

vector<vector<char>> grid_init;
int s_x;
int s_y;
int e_x;
int e_y;

struct Dijkstra
{
    Dijkstra(int x, int y, int d) : x(x), y(y), d(d) {}
    int x;
    int y;
    int d;
    bool operator<(Dijkstra& m) { return d > m.d; }
};
struct DijkstraComp
{
    bool operator()(Dijkstra& m1, Dijkstra& m2) { return m1 < m2; }
};

struct AStar
{
    AStar(int x, int y, int d, int e_x, int e_y) : x(x), y(y), d(d), e_x(e_x), e_y(e_y) {}
    int x;
    int y;
    int d;
    int e_x;
    int e_y;
    bool operator<(AStar& m)
    {
        int disToEnd1 = abs(x-e_x) + abs(y-e_y);
        int disToEnd2 = abs(m.x-e_x) + abs(m.y-e_y);
        if (d + disToEnd1 != m.d + disToEnd2) return d + disToEnd1 > m.d + disToEnd1;
        else if (disToEnd1 != disToEnd2) return disToEnd1 > disToEnd2;
        return d > m.d;
    }
};
struct AStarComp
{
    bool operator()(AStar& m1, AStar& m2) { return m1 < m2; }
};

void initGrid()
{
    char obstacle = 'x';

    for (int i = 2; i <= 28; i++)
        grid_init[i][14] = obstacle;
    for (int i = 2; i <= 13; i++)
        grid_init[13][i] = obstacle;
    for (int i = 2; i <= 13; i++)
        grid_init[15][i] = obstacle;
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

void initStartEnd()
{
    printGrid(grid_init);
    s_x = -1, s_y = -1, e_x = -1, e_y = -1;
    while (1)
    {
        cout << "Start X (1~30) : "; cin >> s_x;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        cout << "Start Y (1~30) : "; cin >> s_y;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }

        if (s_x < 1 || s_x > 30 || s_y < 1 || s_y > 30 || grid_init[s_y-1][s_x-1] == 'x') cout << "Not available. Try again." << endl;
        else break;
    }
    while (1)
    {
        cout << "End X (1~30) : "; cin >> e_x;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        cout << "End Y (1~30) : "; cin >> e_y;
        if (cin.fail())
        {
            cout << "Not a number" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }
        if (e_x < 1 || e_x > 30 || e_y < 1 || e_y > 30 || grid_init[e_y-1][e_x-1] == 'x') cout << "Not available. Try again." << endl;
        else break;
    }

    cout << "======================================================" << endl;
    cout << "Start  : " << s_x << ", " << s_y << " | End : " << e_x << ", " << e_y << endl;
    cout << "======================================================" << endl;
    s_x--, s_y--, e_x--, e_y--;
}

bool canMove(const vector<vector<char>>& grid, const vector<vector<bool>>& visited, int x, int y)
{
    int rowSize = grid.size();
    int colSize = grid[0].size();
    if (x < 0 || y < 0 || x >= colSize || y >= rowSize || grid[y][x] == 'x' || visited[y][x])
        return false;
    return true;
}

void findWayDijkstra()
{
    int rowSize = grid_init.size();
    int colSize = grid_init[0].size();

    vector<vector<char>>grid = grid_init;
    grid[s_y][s_x] = 'S'; grid[e_y][e_x] = 'E';

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<vector<bool>> visited_init(rowSize, vector<bool>(colSize, false));
    vector<vector<bool>> visited = visited_init;
    visited[s_y][s_x] = true;

    priority_queue<Dijkstra, vector<Dijkstra>, DijkstraComp> pq;
    pq.push(Dijkstra(s_x, s_y, 0));

    vector<vector<int>> dis(rowSize, vector<int>(colSize, INT_MAX));
    vector<vector<pair<int, int>>> fromInfo(rowSize, vector<pair<int, int>>(colSize, {-1, -1}));
    fromInfo[s_y][s_x] = {s_x, s_y};

    int findCnt = 0;
    while (!pq.empty())
    {
        findCnt++;
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
                pq.push(Dijkstra(x, y-1, d+1));
            }
        }
        if (canMove(grid, visited, x, y+1))
        {
            if (d+1 < dis[y+1][x])
            {
                fromInfo[y+1][x] = {x, y};
                dis[y+1][x] = d+1;
                pq.push(Dijkstra(x, y+1, d+1));
            }
        }
        if (canMove(grid, visited, x-1, y))
        {
            if (d+1 < dis[y][x-1])
            {
                fromInfo[y][x-1] = {x, y};
                dis[y][x-1] = d+1;
                pq.push(Dijkstra(x-1, y, d+1));
            }
        }
        if (canMove(grid, visited, x+1, y))
        {
            if (d+1 < dis[y][x+1])
            {
                fromInfo[y][x+1] = {x, y};
                dis[y][x+1] = d+1;
                pq.push(Dijkstra(x+1, y, d+1));
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

    cout << "Move Count : " << findCnt << endl;
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << ((std::chrono::duration<double>)(end - begin)).count() << " sec" << endl;
    cout << "======================================================" << endl;
}

void findWayAStar()
{
    int rowSize = grid_init.size();
    int colSize = grid_init[0].size();

    vector<vector<char>>grid = grid_init;
    grid[s_y][s_x] = 'S'; grid[e_y][e_x] = 'E';

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<vector<bool>> visited_init(rowSize, vector<bool>(colSize, false));
    vector<vector<bool>> visited = visited_init;
    visited[s_y][s_x] = true;

    priority_queue<AStar, vector<AStar>, AStarComp> pq;
    pq.push(AStar(s_x, s_y, 0, e_x, e_y));

    vector<vector<int>> dis(rowSize, vector<int>(colSize, INT_MAX));
    vector<vector<pair<int, int>>> fromInfo(rowSize, vector<pair<int, int>>(colSize, {-1, -1}));
    fromInfo[s_y][s_x] = {s_x, s_y};

    int findCnt = 0;
    while (!pq.empty())
    {
        findCnt++;
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
                pq.push(AStar(x, y-1, d+1, e_x, e_y));
            }
        }
        if (canMove(grid, visited, x, y+1))
        {
            if (d+1 < dis[y+1][x])
            {
                fromInfo[y+1][x] = {x, y};
                dis[y+1][x] = d+1;
                pq.push(AStar(x, y+1, d+1, e_x, e_y));
            }
        }
        if (canMove(grid, visited, x-1, y))
        {
            if (d+1 < dis[y][x-1])
            {
                fromInfo[y][x-1] = {x, y};
                dis[y][x-1] = d+1;
                pq.push(AStar(x-1, y, d+1, e_x, e_y));
            }
        }
        if (canMove(grid, visited, x+1, y))
        {
            if (d+1 < dis[y][x+1])
            {
                fromInfo[y][x+1] = {x, y};
                dis[y][x+1] = d+1;
                pq.push(AStar(x+1, y, d+1, e_x, e_y));
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

    cout << "Move Count : " << findCnt << endl;
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << ((std::chrono::duration<double>)(end - begin)).count() << " sec" << endl;
    cout << "======================================================" << endl;
}

int main()
{
    int rowSize = 30;
    int colSize = 30;
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
            initStartEnd();
            cout << "Path Find by Dijkstar" << endl;
            findWayDijkstra();
            cout << "Path Find by AStar" << endl;
            findWayAStar();
        }
    }

    return 0;
}
