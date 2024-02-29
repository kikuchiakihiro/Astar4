#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

// 2D�m�[�h��\���\����
struct Node {
    int x, y;  // �m�[�h�̍��W
    int g, h;  // g: �X�^�[�g����̃R�X�g, h: �S�[���܂ł̃q���[���X�e�B�b�N�R�X�g

    // �R���X�g���N�^
    Node(int x, int y, int g, int h) : x(x), y(y), g(g), h(h) {}

    // �]���֐� f(n) = g(n) + h(n)
    int getF() const {
        return g + h;
    }
};

// �I�[�v�����X�g�̔�r�֐�
struct CompareNodes {
    bool operator()(const Node& lhs, const Node& rhs) const {
        return lhs.getF() > rhs.getF();
    }
};

// A*�A���S���Y������������N���X
class AStar {
public:
    AStar(const vector<vector<int>>& grid) : grid(grid) {}

    // A*�A���S���Y���Ōo�H��T������֐�
    vector<pair<int, int>> findPath(pair<int, int> start, pair<int, int> goal) {
        int rows = grid.size();
        int cols = grid[0].size();

        // �ړ�����: �㉺���E�Ǝ΂�
        const vector<pair<int, int>> directions = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        // �I�[�v�����X�g�ƃN���[�Y�h���X�g
        priority_queue<Node, vector<Node>, CompareNodes> openList;
        vector<vector<bool>> closedList(rows, vector<bool>(cols, false));
        vector<vector<Node>> cameFrom(rows, vector<Node>(cols, Node(-1, -1, -1, -1)));

        // �X�^�[�g�m�[�h���I�[�v�����X�g�ɒǉ�
        openList.push(Node(start.first, start.second, 0, calculateH(start, goal)));

        while (!openList.empty()) {
            // �I�[�v�����X�g����ŏ��]���m�[�h�����o��
            Node current = openList.top();
            openList.pop();

            // �S�[���ɓ��B������o�H���\�z���ĕԂ�
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

            // �m�[�h���N���[�Y�h���X�g�ɒǉ�
            closedList[current.x][current.y] = true;

            // �אڃm�[�h�̒T��
            for (const auto& dir : directions) {
                int nextX = current.x + dir.first;
                int nextY = current.y + dir.second;

                // �{�[�h�͈͓̔��ł��ʍs�\�ȏꍇ
                if (nextX >= 0 && nextX < rows && nextY >= 0 && nextY < cols && grid[nextX][nextY] == 0 && !closedList[nextX][nextY]) {
                    int g = current.g + 1;  // ����g�l

                    // ���łɃI�[�v�����X�g�Ɋ܂܂�Ă���ꍇ�A��菬����g�l�ōX�V
                    if (grid[nextX][nextY] == 0 || g < current.g) {
                        openList.push(Node(nextX, nextY, g, calculateH({ nextX, nextY }, goal)));
                        cameFrom[nextX][nextY] = current;  // ���݂̃m�[�h��e�Ƃ��ċL�^
                    }
                }
            }
        }

        // �S�[���ɓ��B�ł��Ȃ��ꍇ�͋�̃p�X��Ԃ�
        return {};
    }

private:
    vector<vector<int>> grid;  // ���H�̏��

    // �}���n�b�^���������v�Z����֐�
    int calculateH(pair<int, int> current, pair<int, int> goal) {
        return abs(current.first - goal.first) + abs(current.second - goal.second);
    }
};

// ���H��\������֐�
void printMaze(const vector<vector<int>>& maze) {
    for (const auto& row : maze) {
        for (int cell : row) {
            if (cell == 0) {
                cout << " ";  // �ʘH
            }
            else {
                cout << "#";  // ��Q��
            }
        }
        cout << endl;
    }
}

int main() {
    // 15x15�̖��H
    vector<vector<int>> maze = {
        // ���H�f�[�^
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

    // �o�H�T��
    vector<pair<int, int>> path = astar.findPath(start, goal);

    // ���ʂ̕\��
    if (!path.empty()) {
        cout << "�ŒZ�o�H: ";
        for (const auto& point : path) {
            cout << "(" << point.first << ", " << point.second << ") ";
        }
        cout << endl;
    }
    else {
        cout << "�S�[���ɓ��B�ł��܂���ł����B" << endl;
    }

    return 0;
}
