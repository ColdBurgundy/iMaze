#include "Board.h"

Board::Board(int size, int tmp)
{
    _size = size % 2 == 0 ? size + 1 : size;

    Initialize();

    // ���� �̷� ���
    std::cout << "\n������ �̷�:\n";
    Draw(false);

    // �ⱸ ��ġ ���
    std::cout << "\nMaze Generated Successfully!" << std::endl;
    std::cout << "Exit Position: (" << getEndRow() << ", " << getEndCol() << ")\n";

    // ��� Ž�� ����
    std::cout << "\nFinding path from start to exit...\n";
    findPathWrapper(0, 0);

    // Ż�� ��� ǥ��
    std::cout << "\nPath to the exit:\n";
    Draw(true); // ��θ� ǥ���ϴ� ������� Draw ȣ�� (�ʿ信 ���� ����)

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
    int cnt = 1;
    while (true) {
        endRow = getRand(_size);
        endCol = getRand(_size);
        std::cout << cnt++ << "��° setExit ������..." << std::endl;
        // ��ȿ�� ��ΰ� �ִ��� ����
        if (_wall[endRow][endCol] == static_cast<bool>(EWallType::Road) && bfs(endRow, endCol) != -1) {
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
                std::cout << "#"; // ��
            else if (showPath && autoPath[i][j] != -1)
                std::cout << "."; // ��� ǥ��
            else if (i == curRow && j == curCol)
                std::cout << "P"; // ���� ��ġ
            else if (i == endRow && j == endCol)
                std::cout << "E"; // �ⱸ
            else
                std::cout << " "; // ��
        }
        std::cout << std::endl;
    }
}

int Board::bfs(int startRow, int startCol)
{
    int height = _size, width = _size;

    if (_wall[startRow][startCol] != 0) return -1;

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    q.push({ startRow, startCol });
    visited[startRow][startCol] = true;

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        int row = current.first;
        int col = current.second;
        q.pop();

        // �������� �����ϸ� ����
        if (row == 0 && col == 0) {
            return 1;
        }

        // �����¿� Ž��
        for (int i = 0; i < 4; i++) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            // ��ȿ�� ��ǥ���� Ȯ��
            if (newRow >= 0 && newRow < height && newCol >= 0 && newCol < width &&
                !visited[newRow][newCol] && _wall[newRow][newCol] == 0) {
                visited[newRow][newCol] = true;
                q.push({ newRow, newCol });
            }
        }
    }

    return -1; // No path found
}

int Board::getEndRow()
{
    return endRow;
}

int Board::getEndCol()
{
    return endCol;
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

            // �������Ͽ� �ִ� ��θ� autoPath�� ����
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
    ini_vis();      // �湮 �ʱ�ȭ
    ini_autoPath(); // ��� ��� �ʱ�ȭ
}

void Board::findPathWrapper(int startRow, int startCol) {
    std::cout << "Starting pathfinding from (" << startRow << ", " << startCol << ")...\n";
    findPath(startRow, startCol, 0);
}

int Board::getCell(int x, int y) const {
    return _wall[y][x];
}

int** Board::getAutoPath() const {
    return autoPath;
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
