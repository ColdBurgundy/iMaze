#pragma once
#ifndef __BOARD_H__
#define __BOARD_H__
#define RandomMod 100

#include <iostream>
#include <queue>
#include <map>
#include <climits>
#include <vector>
#include <algorithm>
#include <ctime>
#include <Windows.h>
#include <random>

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
	Board(int size, int tmp);
	int bfs(int row, int col);
	void Draw(bool empty, int curRow = 0, int curCol = 0);
	int getEndRow();
	int getEndCol();
	void ini_for_search();
	void findPathWrapper(int startRow, int startCol); 
	int getWidth() const { return _size; }
	int getHeight() const { return _size; }
	int getCell(int x, int y) const;
	~Board();
};

#endif // !__BOARD_H__