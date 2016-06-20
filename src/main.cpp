#include <string>
#include <iostream>
#include <list>
#include <vector>
#include "Labirynth.h"
#include "MazeScout.h"

using namespace std;

int main()
{
	Labirynth* maze = new Labirynth();
	Labirynth* fakeMaze = new Labirynth();
	MazeScout* scout = new MazeScout(fakeMaze);
	vector<LabirynthDirection> path = scout->getEscapePath();

	getline(cin, string());

	bool success = true;

	while (success && path.size() > 0)
	{
		LabirynthDirection curDirection = path.back();
		path.pop_back();
		success = maze->TryMoveInDir(curDirection);

		if (path.size())
		{
			maze->DrawLabirynth();
		}
	}

	getline(cin, string());

	delete maze;
	delete fakeMaze;
	delete scout;
}








//
//
// TODO: clean up code
// TODO: avoid range iterations
// TODO: don't use vector for maze map, it's already an linked list
// TODO: fix bug in the findCrossroad function
// TODO: investigate pain of deleting element from the vector; mb list is better?
// iteraror http://stackoverflow.com/questions/3779227/why-is-this-vector-iterator-not-incrementable
// remove-erase https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
//frontLine.erase(std::remove(frontLine.begin(), frontLine.end(), targetDestination), frontLine.end());
/*
std::vector<cell*>::iterator positionToErase = std::find(frontLine.begin(), frontLine.end(), targetDestination);
if (positionToErase != frontLine.end())
{
positionToErase = frontLine.erase(positionToErase);
}
*/