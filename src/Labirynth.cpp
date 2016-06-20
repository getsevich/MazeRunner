#include "Labirynth.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <Windows.h>

Labirynth::Labirynth()
	: m_currX( 0 )
	, m_currY( 0 )
	, m_totalCost( 0 )
{
	std::ifstream fin( "labirynt.txt" );

	if( !fin.is_open() )
	{
		std::cout << "Plik w zlym folderze! Albo nie istnieje." << std::endl;
		for( int i = 0; i < MAP_SIZE; ++i )
		{
			for( int j = 0; j < MAP_SIZE; ++j )
			{
				m_map[ i ][ j ] = false;
			}
		}
	}
	else
	{
		for( int i = 0; i < MAP_SIZE; ++i )
		{
			std::string line;
			std::getline( fin, line );
			if( line.size() < MAP_SIZE )
			{
				std::cout << "file corrupted at line " << i << std::endl;
			}

			for( int j = 0; j < MAP_SIZE; ++j )
			{
				m_map[ j ][ i ] = ( ( j < ( int )line.size() ) ? ( line[ j ] == 'X' ) : false );
			}
		}

		fin.close();
	}

	DrawLabirynth();
}

bool Labirynth::CanMoveInDir( LabirynthDirection dir )
{
	m_totalCost += 1;
	int x, y;
	DirToOffset( dir, x, y );
	return CanMoveInDir( x, y );
}

bool Labirynth::TryMoveInDir( LabirynthDirection dir )
{
	m_totalCost += 5;
	int x, y;
	DirToOffset( dir, x, y );

	if( CanMoveInDir( x, y ) )
	{
		m_currX += x;
		m_currY += y;

		if( Finished() )
		{
			std::cout << "REACHED IT! TOTAL COST: " << m_totalCost << std::endl;
		}

		return true;
	}

	return false;
}

bool Labirynth::Finished()
{
	return ( m_currX == 15 ) && ( m_currY == 15 );
}

void Labirynth::DrawLabirynth()
{
	system( "cls" );

	for( int i = 0; i < MAP_SIZE; ++i )
	{
		for( int j = 0; j < MAP_SIZE; ++j )
		{
			if( ( i == m_currY ) && ( j == m_currX ) )
			{
				std::cout << "@";
			}
			else if( m_map[ j ][ i ] )
			{
				std::cout << "X";
			}
			else
			{
				std::cout << ".";
			}
		}

		std::cout << std::endl;
	}

	Sleep( 100 );
}

void Labirynth::DirToOffset( LabirynthDirection dir, int& x, int &y )
{
	switch( dir )
	{
		case LD_LEFT:
			x = -1;
			y = 0;
			break;
		case LD_RIGHT:
			x = 1;
			y = 0;
			break;
		case LD_UP:
			x = 0;
			y = -1;
			break;
		case LD_DOWN:
			x = 0;
			y = 1;
			break;
	}
}

bool Labirynth::CanMoveInDir( int x, int y )
{
	int newX = x + m_currX;
	int newY = y + m_currY;

	if( ( newX < 0 ) || ( newX >= MAP_SIZE ) )
	{
		return false;
	}

	if( ( newY < 0 ) || ( newY >= MAP_SIZE ) )
	{
		return false;
	}

	return !( m_map[ newX ][ newY ] );
}

