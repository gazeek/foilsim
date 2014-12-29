#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include <cmath>

#include "Box.h"


Box::Box (double h, double w, int ballCap /* = 1*/)
{
	ballList = new Ball*[ballCap];
	ballCapacity  = ballCap;
	nBalls = 0;


	boxHeight = h;
	boxWidth = w;

}
//addBall adds the ball to the list of Balls in the Box, if the number of Balls is exceeded than it does nothing.
//
//
//
//
void Box::addBall (Ball* ball)
{
	if (nBalls < ballCapacity)
	{
		ballList[nBalls] = ball;
		
		ball->setBox(this);

		nBalls++;
	}
}



int Box::drawBox(int height)
{
	bool flag;

	int ballsDrawn = 0;

	clear();

	for (int j = 0; j < boxWidth + 2; j++)
		printw("X");

	printw("\n");


	for (int i = 0; i < boxHeight; i++)
	{
		printw("X");
		for (int j = 0; j < boxWidth; j++)
		{
			
			
			for ( int r = 0; r < nBalls; r++)
			{
				flag = false;

				if ( (floor((ballList[r])->x) == j) && (floor((ballList[r])->y) == height - i - 1))

				{
					printw("O");
					ballsDrawn++;
					r = nBalls;
					flag = true; // checking off, that we actually placed the blob here, debug...
				}
			}
			
			if (flag == false)
				printw(" ");

		}
		printw("X\n");
	}


	for (int j = 0; j < boxWidth + 2; j++)
		printw("X");
	refresh();

	return ballsDrawn;
}

