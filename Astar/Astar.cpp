#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

// 2Dノードを表す構造体
struct Node {
    int x, y;  // ノードの座標
    int g, h;  // g: スタートからのコスト, h: ゴールまでのヒューリスティックコスト

    // コンストラクタ
    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h) {}

    // 評価関数 f(n) = g(n) + h(n)
    int getF() const {
        return g + h;
    }
};

// オープンリストの比較関数
struct CompareNodes {
    bool operator()(const Node& lhs, const Node& rhs) const {
        return lhs.getF() > rhs.getF();
    }
};

// A*アルゴリズムを実装するクラス
class AStar {
public:
    AStar(const vector<vector<int>>& grid) : grid(grid) {}

    // A*アルゴリズムで経路を探索する関数
    vector<pair<int, int>> findPath(pair<int, int> start, pair<int, int> goal) {
        int rows = grid.size();
        int cols = grid[0].size();

        // 移動方向: 上下左右と斜め
        const vector<pair<int, int>> directions = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        // オープンリストとクローズドリスト
        priority_queue<Node, vector<Node>, CompareNodes> openList;
        vector<vector<bool>> closedList(rows, vector<bool>(cols, false));
        vector<vector<Node>> cameFrom(rows, vector<Node>(cols, Node(-1, -1, -1, -1)));

        // スタートノードをオープンリストに追加
        openList.push(Node(start.first, start.second, 0, calculateH(start, goal)));

        while (!openList.empty()) {
            // オープンリストから最小評価ノードを取り出す
            Node current = openList.top();
            openList.pop();

            // ゴールに到達したら経路を構築して返す
            if (current.x == goal.first && current.y == goal.second) {
                vector<pair<int, int>> path;
                while (!(current.x == start.first && current.y == start.second)) {
                    path.push_back({ current.x, current.y });
                    current = cameFrom[current.x][current.y];
                }
                path.push_back({ start.first, start.second });
                reverse(path.begin(), path.end());
                return path;
            }

            // ノードをクローズドリストに追加
            closedList[current.x][current.y] = true;

            // 隣接ノードの探索
            for (const auto& dir : directions) {
                int nextX = current.x + dir.first;
                int nextY = current.y + dir.second;

                // ボードの範囲内でかつ通行可能な場合
                if (nextX >= 0 && nextX < rows && nextY >= 0 && nextY < cols && grid[nextX][nextY] == 0 && !closedList[nextX][nextY]) {
                    int g = current.g + 1;  // 仮のg値

                    // すでにオープンリストに含まれている場合、より小さいg値で更新
                    if (grid[nextX][nextY] == 0 || g < current.g) {
                        openList.push(Node(nextX, nextY, g, calculateH({ nextX, nextY }, goal)));
                        cameFrom[nextX][nextY] = current;  // 現在のノードを親として記録
                    }
                }
            }
        }

        // ゴールに到達できない場合は空のパスを返す
        return {};
    }

private:
    vector<vector<int>> grid;  // 迷路の情報

    // マンハッタン距離を計算する関数
    int calculateH(pair<int, int> current, pair<int, int> goal) {
        return abs(current.first - goal.first) + abs(current.second - goal.second);
    }
};

// 迷路を表示する関数
void printMaze(const vector<vector<int>>& maze) {
    for (const auto& row : maze) {
        for (int cell : row) {
            if (cell == 0) {
                cout << " ";  // 通路
            }
            else {
                cout << "#";  // 障害物
            }
        }
        cout << endl;
    }
}

int main() {
    // 15x15の迷路
    vector<vector<int>> maze = {
        // 迷路データ
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
    };

    AStar astar(maze);

    pair<int, int> start = { 0, 0 };
    pair<int, int> goal = { 14, 14 };

    // 経路探索
    vector<pair<int, int>> path = astar.findPath(start, goal);

    // 結果の表示
    if (!path.empty()) {
        cout << "最短経路: ";
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;
    }
    else {
        cout << "ゴールに到達できませんでした。" << endl;
    }

    return 0;
}
