#ifndef __LABIRYNTH_H__
#define __LABIRYNTH_H__

#define MAP_SIZE 16

enum LabirynthDirection
{
	LD_LEFT,
	LD_RIGHT,
	LD_UP,
	LD_DOWN
};

class Labirynth
{
public:
	Labirynth();


	bool CanMoveInDir( LabirynthDirection dir );	// true - podloga, false - sciana, koszt - 1
	bool TryMoveInDir( LabirynthDirection dir );	// true - udalo sie, false - nie udalo sie, koszt - 5

	bool Finished();

	void DrawLabirynth();

	bool m_map[MAP_SIZE][MAP_SIZE];

private:

	
	int m_currX;
	int m_currY;
	int m_totalCost;

private:

	void DirToOffset( LabirynthDirection dir, int& x, int &y );
	bool CanMoveInDir( int x, int y );
};

#endif
