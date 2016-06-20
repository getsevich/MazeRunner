#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <Windows.h>
#include "MazeScout.h"
#include "Labirynth.h"

using namespace std;

// - public

MazeScout::MazeScout(Labirynth *mazeRef)
{
	maze = mazeRef;
	cursorPoint = new cell();
	frontLine.push_back(cursorPoint);
}

MazeScout::~MazeScout()
{
	delete cursorPoint;

	// WARNING: memory leak ??
	// TODO: recursive cells cleanup
}

vector<LabirynthDirection> MazeScout::getEscapePath()
{
	vector<LabirynthDirection> finalPath;
	ENavigationResult navResult = NR_OK;

	// start

	while (navResult == NR_OK)
	{
		vector<cell*> tempFrontLine;

		for (auto currentCell : frontLine)
		{
			navResult = gotoCell(currentCell, &tempFrontLine);
		}

		frontLine = tempFrontLine;
		DrawLabirynth();
	}

	if (navResult == NR_DEAD_END)
	{
		cout<<endl<<"Fatal maze navigation failure..."<<endl;
		return finalPath;
	}	

	// we did it! go A*!

	cout <<endl <<"the path: ";

	cell* newPos = NULL;
	cell* endPos = GetCellByPositon(MAP_SIZE - 1, MAP_SIZE - 1);
	while (endPos->x != 0 || endPos->y != 0)
	{
		LabirynthDirection dir = endPos->getOptimalNeighbor(newPos);
		if (newPos)
		{
			finalPath.push_back(getOppositeDirection(dir));
			endPos = newPos;

			cout << "-> "<< newPos->x<< " " << newPos->y <<" (" << newPos->stepIdx << ") ";
		}
	}

	cout <<endl;

	return finalPath;
}


// - under the hood

ENavigationResult MazeScout::gotoCell(cell *targetDestination, vector<cell*> *newFrontLine)
{
	if (targetDestination != cursorPoint)
	{	
		bool movementResult = findCrossroad(targetDestination);

		if (!movementResult)
		{
			return NR_DEAD_END;
		}

		vector<LabirynthDirection> forwardPath = getForwardPathTo(targetDestination);
		movementResult = followThePath(forwardPath);

		if (!movementResult)
		{
			return NR_DEAD_END;
		}
	}

	ENavigationResult creationRes;
	
	creationRes = createCell(LD_UP, newFrontLine);
	if (creationRes == NR_EXIT_FOUND) return NR_EXIT_FOUND;

	creationRes = createCell(LD_DOWN, newFrontLine);
	if (creationRes == NR_EXIT_FOUND) return NR_EXIT_FOUND;

	creationRes = createCell(LD_RIGHT, newFrontLine);
	if (creationRes == NR_EXIT_FOUND) return NR_EXIT_FOUND;

	creationRes = createCell(LD_LEFT, newFrontLine);
	if (creationRes == NR_EXIT_FOUND) return NR_EXIT_FOUND;

	return NR_OK;
}
	
ENavigationResult MazeScout::createCell(LabirynthDirection dir, vector<cell*> *newFrontLine)
{
	if (maze->CanMoveInDir(dir))
	{
		cell* newCell = NULL;
		LabirynthDirection oppositDiraction;
		int curX, curY;

		switch (dir)
		{
			case LD_LEFT:
				if (!cursorPoint->left)
				{
					curX = cursorPoint->x - 1;
					curY = cursorPoint->y;
					newCell = GetCellByPositon(curX, curY);

					if (!newCell)
					{
						newCell = new cell();
						newCell->x = curX;
						newCell->y = curY;
					}

					newCell->right = cursorPoint;
					cursorPoint->left = newCell;
				}
				oppositDiraction = LD_RIGHT;
				break;

			case LD_RIGHT:
				if (!cursorPoint->right)
				{
					curX = cursorPoint->x + 1;
					curY = cursorPoint->y;
					newCell = GetCellByPositon(curX, curY);

					if (!newCell)
					{
						newCell = new cell();
						newCell->x = curX;
						newCell->y = curY;
					}

					newCell->left = cursorPoint;
					cursorPoint->right = newCell;
				}
				oppositDiraction = LD_LEFT;
				break;

			case LD_UP:
				if (!cursorPoint->up)
				{
					curX = cursorPoint->x;
					curY = cursorPoint->y-1;
					newCell = GetCellByPositon(curX, curY);

					if (!newCell)
					{
						newCell = new cell();
						newCell->x = curX;
						newCell->y = curY;
					}

					newCell->down = cursorPoint;
					cursorPoint->up = newCell;
				}
				oppositDiraction = LD_DOWN;
				break;

			case LD_DOWN:
				if (!cursorPoint->down)
				{
					curX = cursorPoint->x;
					curY = cursorPoint->y+1;
					newCell = GetCellByPositon(curX, curY);

					if (!newCell)
					{
						newCell = new cell();
						newCell->x = curX;
						newCell->y = curY;
					}

					newCell->up = cursorPoint;
					cursorPoint->down = newCell;
				}
				oppositDiraction = LD_UP;
				break;

			default:
				break;
		}

		if (!newCell)
		{
			return NR_DEAD_END;
		}
		else
		{
			unitVector newUnit;

			newUnit.diraction = oppositDiraction;
			newUnit.value = cursorPoint;
			newUnit.x = cursorPoint->x;
			newUnit.y = cursorPoint->y;

			vector<unitVector> newPathBack = cursorPoint->pathBack;
			newPathBack.push_back(newUnit);

			newCell->pathBack = newPathBack;
			newCell->stepIdx = cursorPoint->stepIdx + 1;

			mazeMap.push_back(newCell);
			newFrontLine->push_back(newCell);

			//cout << cursorPoint->x << ":" << cursorPoint->y << " ---> " << "(" << newCell->x << ":" << newCell->y << "); " << endl;
			//Sleep(500);
		}

		if (newCell->x == MAP_SIZE-1 && newCell->y == MAP_SIZE-1)
		{
			// can't touch this
			return NR_EXIT_FOUND;
		}
	}
	else
	{
		return NR_DEAD_END;
	}

	return NR_OK;
}

// trying to find a crossroad to turn to *targetDestination cell
// in worst case crossroad is our start point [0, 0]
// in best case we'll find *targetDestination in the pathBack list
bool MazeScout::findCrossroad(cell *targetDestination)
{
	if (cursorPoint == targetDestination)
	{
		return true;
	}

	/* SHAAAME... BUUUG HERE

	for (const unitVector &actualPathPoint : MakeReverse(cursorPoint->pathBack))
	{
		TryMoveInDir(actualPathPoint.diraction);

		if (actualPathPoint.value == targetDestination)
		{
			return true;
		}

		for (const unitVector &targetPathPoint : MakeReverse(targetDestination->pathBack))
		{
			if (actualPathPoint.value == targetPathPoint.value)
			{
				return true;
			}
		}
	}
	*/

	//
	// tmp solution, just go to the beginning :( 

	vector <unitVector> tmp = cursorPoint->pathBack;

	for (const unitVector &actualPathPoint : MakeReverse(tmp))
	{
		TryMoveInDir(actualPathPoint.diraction);
	}

	return true;
}

bool MazeScout::TryMoveInDir(LabirynthDirection dir)
{
	if (maze->CanMoveInDir(dir))
	{
		cell* newCursor = cursorPoint->getNeighbor(dir);

		if (newCursor)
		{
			cursorPoint = newCursor;
		}

		maze->TryMoveInDir(dir);

		return true;
	}

	return false;
}

bool MazeScout::followThePath(vector<LabirynthDirection> path)
{
	bool success = true;

	while (success && path.size() > 0)
	{
		LabirynthDirection curDirection = path.back();
		path.pop_back();
		success = TryMoveInDir(curDirection);
	}	

	return success;
}

vector<LabirynthDirection> MazeScout::getForwardPathTo(cell *targetDestination)
{
	vector<LabirynthDirection> finalPath;

	for (const unitVector &pathPoint : MakeReverse(targetDestination->pathBack))
	{
		finalPath.push_back(getOppositeDirection(pathPoint.diraction));

		if (cursorPoint == pathPoint.value)
		{
			return finalPath;
		}
	}

	return finalPath;
}

LabirynthDirection MazeScout::getOppositeDirection(LabirynthDirection dir)
{
	switch (dir)
	{
		case LD_UP:
			return LD_DOWN;
			break;
		case LD_DOWN:
			return LD_UP;
			break;
		case LD_RIGHT:
			return LD_LEFT;
			break;
		case LD_LEFT:
			return LD_RIGHT;
			break;
		default:
			break;
	}

	return LD_UP; // missing default value
}

cell* MazeScout::GetCellByPositon(int j, int i)
{
	for (auto currentCell : mazeMap)
	{
		if (currentCell->x == j && currentCell->y == i)
		{
			return currentCell;
			break;
		}
	}

	return NULL;
}

void MazeScout::escapeTheMaze(Labirynth* maze, vector<unitVector> mazepath)
{
	// del
}

void MazeScout::DrawLabirynth()
{
	system("cls");

	for (int i = 0; i < MAP_SIZE; ++i)
	{
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			cell* cl = GetCellByPositon(j, i);
			
			if (cl)
			{
				if (cl->stepIdx < 10)
				{
					std::cout << cl->stepIdx << "  ";
				}
				else
				{
					std::cout << cl->stepIdx << " ";
				}
			}
			else
			{
				std::cout << ".  ";
			}
		}

		std::cout << std::endl;
	}

	Sleep(50);

	cout << endl;
}

