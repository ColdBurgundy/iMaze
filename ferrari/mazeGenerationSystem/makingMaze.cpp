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

void Board::Draw(bool showPath, int curRow, int curCol)
{
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (_wall[i][j])
                std::cout << "#"; // 벽
            else if (showPath && autoPath[i][j] != -1)
                std::cout << "."; // 경로 표시
            else if (i == curRow && j == curCol)
                std::cout << "P"; // 현재 위치
            else if (i == endRow && j == endCol)
                std::cout << "E"; // 출구
            else
                std::cout << " "; // 길
        }
        std::cout << std::endl;
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
    // BFS 구현 (단순 샘플, 필요에 따라 수정 가능)
    return 0; // 기본 반환값
}

void Board::findPath(int startRow, int startCol, int target) {
    ini_vis();
    ini_autoPath();
    queue<Pos> q;
    q.push(Pos(startRow, startCol));
    _vis[startRow][startCol] = true;

    map<Pos, Pos> parent;
    parent[Pos(startRow, startCol)] = Pos(-1, -1);

    while (!q.empty()) {
        Pos cur = q.front();
        q.pop();

        if (cur.row == endRow && cur.col == endCol) {
            std::cout << "Path to the exit found!\n";

            // 역추적하여 최단 경로만 autoPath에 저장
            Pos backtrack = cur;
            while (backtrack.row != -1 && backtrack.col != -1) {
                autoPath[backtrack.row][backtrack.col] = 0;
                backtrack = parent[backtrack];
            }
            return;
        }

        for (int i = 0; i < 4; i++) {
            int nextRow = cur.row + dx[i];
            int nextCol = cur.col + dy[i];

            if (nextRow >= 0 && nextRow < _size && nextCol >= 0 && nextCol < _size &&
                !_vis[nextRow][nextCol] && !_wall[nextRow][nextCol]) {
                _vis[nextRow][nextCol] = true;
                parent[Pos(nextRow, nextCol)] = cur;
                q.push(Pos(nextRow, nextCol));
            }
        }
    }

    std::cout << "No path to the exit exists!\n";
}




void Board::ini_vis() {
    if (!_vis) {
        _vis = new bool* [_size];
        for (int i = 0; i < _size; i++) {
            _vis[i] = new bool[_size];
        }
    }
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            _vis[i][j] = false;
        }
    }
}

void Board::ini_autoPath() {
    if (!autoPath) {
        autoPath = new int* [_size];
        for (int i = 0; i < _size; i++) {
            autoPath[i] = new int[_size];
        }
    }
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            autoPath[i][j] = -1;
        }
    }
}

void Board::ini_for_search() {
    ini_vis();      // 방문 초기화
    ini_autoPath(); // 경로 기록 초기화
}

void Board::findPathWrapper(int startRow, int startCol) {
    std::cout << "Starting pathfinding from (" << startRow << ", " << startCol << ")...\n";
    findPath(startRow, startCol, 0);
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
    int mazeSize;

    std::cout << "미로 사이즈 입력 (홀수로 입력해주세요): ";
    std::cin >> mazeSize;
    if (mazeSize % 2 == 0) mazeSize++; // 홀수로 보정

    // Board 객체 생성 및 초기화
    Board board(mazeSize);

    // 최종 미로 출력
    std::cout << "\n생성된 미로:\n";
    board.Draw(false);

    // 출구 위치 출력
    std::cout << "\nMaze Generated Successfully!" << std::endl;
    std::cout << "Exit Position: (" << board.getEndRow() << ", " << board.getEndCol() << ")\n";

    // 경로 탐색 실행
    std::cout << "\nFinding path from start to exit...\n";
    board.findPathWrapper(0, 0);

    // 탈출 경로 표시
    std::cout << "\nPath to the exit:\n";
    board.Draw(true); // 경로만 표시하는 방식으로 Draw 호출 (필요에 따라 수정)

    std::cout << "Press Enter to exit the program...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
