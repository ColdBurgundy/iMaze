#pragma once
#ifndef __BOARD_H__
#define __BOARD_H__
#define RandomMod 100
#define AI_MOVE_SECOND 125

#include <iostream>
#include <queue>
#include <map>
#include <climits>
#include <vector>
#include <algorithm>
#include <ctime>
#include <Windows.h>

using namespace std;

class Board
{
private:
	enum class EWallType
	{
		Road,
		Wall
	};

	struct Pos {
		int row, col;

		Pos() { row = -1, col = -1; }
		Pos(int row, int col) : row(row), col(col) {}

		bool operator<(const Pos& o) const {
			if (row == o.row) return col < o.col;
			return row < o.row;
		}
	};

	struct EdgeInfo
	{
		Pos pos;
		int cost;

		EdgeInfo(Pos pos, int cost) : pos(pos), cost(cost) {}

		bool operator<(const EdgeInfo& o) const { return cost > o.cost; }
	};

	struct Path {
		int _row, _col, _vis;

		Path(int row, int col, int vis) {
			_row = row;
			_col = col;
			_vis = vis;
		}
	};

	int* dx, * dy;
	int _size, endRow, endCol, _count;
	bool** _wall, ** _vis;
	int** autoPath;
	map<Pos, bool> _visRoad;
	map<Pos, int> _minCost;
	map<Pos, vector<EdgeInfo>> _edge;

	void Initialize();
	void makeEdge();
	void makeRoad();
	void setStartPos();
	void setExit();

	void findPath(int, int, int);
	void ini_vis();
	void ini_autoPath();

	int getRand(int r = RandomMod);

public:
	Board(int size);

	void Draw(bool empty, int curRow = 0, int curCol = 0);
	int getSize();
	bool** getWall();
	int getEndRow();
	int getEndCol();
	int getBFSCount();
	int bfs(int row, int col, bool printDis = false);
	bool search(bool isAI = false, int row = 0, int col = 0, int count = 0, int startRow = 0, int startCol = 0, int startCount = 0);
	void AIEscape();
	void ini_for_search();
	void print_player(int after_color = 15);
	void print_exit(int after_color = 15);
	void print_wall(int after_color = 15);
	void print_color_str(string, int, int after_color = 15);
	~Board();
};

#endif // !__BOARD_H__