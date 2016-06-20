#pragma once

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include "ReverseAdapter.h"
#include "Labirynth.h"

using namespace std;

enum ENavigationResult
{
	NR_OK,
	NR_DEAD_END,
	NR_EXIT_FOUND
};

struct unitVector;

struct cell
{
	cell() : x(0), y(0), stepIdx(0), right(NULL), left(NULL), up(NULL), down(NULL)  {}

	int x;
	int y;
	int stepIdx;

	// neighbors
	cell* right;
	cell* left;
	cell* up;
	cell* down;

	cell* getNeighbor(LabirynthDirection direction)
	{
		switch( direction )
		{
		case LD_LEFT:
			return left;
			break;
		case LD_RIGHT:
			return right;
			break;
		case LD_UP:
			return up;
			break;
		case LD_DOWN:
			return down;
			break;
		}
		return NULL;
	}

	LabirynthDirection getOptimalNeighbor(cell*&  neighbor)
	{
		int weight = -1;
		LabirynthDirection res;

		if (right && (right->stepIdx < weight || weight == -1))
		{
			weight = right->stepIdx;
			neighbor = right;
			res = LD_RIGHT;
		}
		if (left && (left->stepIdx < weight || weight == -1))
		{
			weight = left->stepIdx;
			neighbor = left;
			res = LD_LEFT;
		}
		if (up && (up->stepIdx < weight || weight == -1))
		{
			weight = up->stepIdx;
			neighbor = up;
			res = LD_UP;
		}
		if (down && (down->stepIdx < weight || weight == -1))
		{
			weight = down->stepIdx;
			neighbor = down;
			res = LD_DOWN;
		}

		return res;
	}

	// path to the entrance of the maze
	vector<unitVector> pathBack;
};

struct unitVector
{
	unitVector() : x(0), y(0), value(NULL) {}

	int x;
	int y;
	LabirynthDirection diraction;
	cell* value;	
};

class MazeScout
{
public:
	MazeScout(Labirynth *mazeRef);
	~MazeScout();

	vector<LabirynthDirection> getEscapePath(); // scanning
	void escapeTheMaze(Labirynth* maze, vector<unitVector> mazepath); // escaping

private:
	Labirynth *maze;
	cell* cursorPoint;	
	vector<cell*> frontLine;
	vector<cell*> mazeMap;

	ENavigationResult gotoCell(cell *targetDestination, vector<cell*> *newFrontLine);
	ENavigationResult createCell(LabirynthDirection dir, vector<cell*> *newFrontLine);
	vector<LabirynthDirection> getForwardPathTo(cell *targetDestination);
	bool findCrossroad(cell *targetDestination);	
	bool followThePath(vector<LabirynthDirection> path);
	bool TryMoveInDir(LabirynthDirection dir);
	LabirynthDirection getOppositeDirection(LabirynthDirection dir);
	void DrawLabirynth();
	cell* GetCellByPositon(int j, int i);
};
