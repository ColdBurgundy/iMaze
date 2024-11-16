#include "Board.h"
#include <random>
#include <iostream>

Board::Board(int size)
{
    _size = size;

    Initialize();
}

void Board::Initialize()
{
    endRow = -1;
    endCol = -1;
    _count = 0;

    dx = new int[4] { -1, 1, 0, 0 };
    dy = new int[4] { 0, 0, -1, 1 };

    _wall = new bool* [_size];
    for (int i = 0; i < _size; i++) {
        _wall[i] = new bool[_size];
        fill_n(_wall[i], _size, static_cast<bool>(EWallType::Wall));

        if (i % 2 == 0) continue;

        for (int j = 1; j < _size; j += 2) _wall[i][j] = static_cast<bool>(EWallType::Road);
    }

    for (int i = 1; i < _size; i += 2) {
        for (int j = 1; j < _size; j += 2) {
            _visRoad[Pos(i, j)] = false;
            _minCost[Pos(i, j)] = INT_MAX;
        }
    }

    makeEdge();
    makeRoad();

    setStartPos();
    setExit();
}

void Board::makeEdge()
{
    for (int row = 1; row < _size; row += 2) {
        for (int col = 1; col < _size; col += 2) {
            if (row < _size - 2) {
                int r = getRand();
                Pos u(row, col);
                Pos v(row + 2, col);
                _edge[u].push_back(EdgeInfo(v, r));
                _edge[v].push_back(EdgeInfo(u, r));
            }

            if (col < _size - 2) {
                int r = getRand();
                Pos u(row, col);
                Pos v(row, col + 2);
                _edge[u].push_back(EdgeInfo(v, r));
                _edge[v].push_back(EdgeInfo(u, r));
            }
        }
    }
}

void Board::makeRoad()
{
    priority_queue<EdgeInfo> PQ;
    PQ.push(EdgeInfo(Pos(1, 1), 0));
    _minCost[Pos(1, 1)] = 0;

    map<Pos, Pos> parent;
    parent[Pos(1, 1)] = Pos(1, 1);

    while (!PQ.empty()) {
        EdgeInfo node = PQ.top();
        PQ.pop();

        Pos curPos = node.pos;
        int curCost = node.cost;

        if (_visRoad[curPos]) continue;
        _visRoad[curPos] = true;

        int row = (parent[curPos].row + curPos.row) / 2;
        int col = (parent[curPos].col + curPos.col) / 2;
        _wall[row][col] = static_cast<bool>(EWallType::Road);

        for (const EdgeInfo& next : _edge[curPos]) {
            Pos nextPos = next.pos;
            int nextCost = next.cost;

            if (_visRoad[nextPos] || _minCost[nextPos] < nextCost) continue;

            parent[nextPos] = curPos;
            _minCost[nextPos] = nextCost;

            PQ.push(next);
        }
    }
}

void Board::setStartPos() {
    _wall[0][0] = static_cast<bool>(EWallType::Road);
    _wall[0][1] = static_cast<bool>(EWallType::Road);
}

void Board::setExit() {
    while (true) {
        endRow = getRand(_size);
        endCol = getRand(_size);

        // 유효한 경로가 있는지 검증
        if (_wall[endRow][endCol] == static_cast<bool>(EWallType::Road) && bfs(0, 0) != -1) {
            break;
        }
    }
}

int Board::getRand(int r)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, r - 1);
    return dist(gen);
}

void Board::Draw(bool empty, int curRow, int curCol)
{
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (_wall[i][j])
                cout << "#"; // 벽
            else if (!empty && i == curRow && j == curCol)
                cout << "P"; // 현재 위치
            else if (!empty && i == endRow && j == endCol)
                cout << "E"; // 출구
            else
                cout << " "; // 길
        }
        cout << endl;
    }
}

int Board::getEndRow()
{
    return endRow;
}

int Board::getEndCol()
{
    return endCol;
}

int Board::bfs(int row, int col, bool printDis)
{
    // BFS 구현
    return 0; // 기본 반환값
}

Board::~Board()
{
    for (int i = 0; i < _size; i++) {
        delete[] _wall[i];
    }
    delete[] _wall;

    delete[] dx;
    delete[] dy;
}


int main() {
    // 미로 크기 설정
    int mazeSize; // 홀수 크기만 허용


    cout << "미로 사이즈 입력: ";
    cin >> mazeSize;
    if (mazeSize % 2 == 0) mazeSize++;


    // Board 객체 생성 및 초기화
    Board board(mazeSize);

    // 최종 미로 출력
    std::cout << "Final Maze:\n";
    board.Draw(false);

    // 출구 위치 출력
    std::cout << "\nMaze Generated Successfully!" << std::endl;
    std::cout << "Exit Position: (" << board.getEndRow() << ", " << board.getEndCol() << ")\n";
    std::cout << "Press Enter to exit the program...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}