#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <boost/timer.hpp>
#include <cmath>
#include <ncurses.h>
#include "Ball.h"
#include "Box.h"

#define HEIGHT 30.
#define WIDTH 80.
#define TIME 0.5
#define RADIUS 0.5
#define BLOBNUMBER 2

int drawBlob (int x, int y);// drawBlob - draws the blob in the console
int drawBox (Box* box, int stepNo = 0); 


int main()
{
	double blobx =  WIDTH / 2, bloby =  HEIGHT / 2;
	double blobvx = -1.0, blobvy = -1.0 ;
	int flag = 100;
	int stepNo = 0;
	char c;
	
	Box *box = new Box(HEIGHT, WIDTH, BLOBNUMBER);
	
	Ball *blob1 = new Ball (30, 20, 0.5, 0.0, RADIUS);
	Ball *blob2 = new Ball (42, 20, -0.5, 0.0);

	box->addBall(blob1);
	box->addBall(blob2);
	
	//Ncurses initiation
	initscr();

	while (flag)
	{
		
		usleep(150000);

		blobx += TIME * blobvx;
		bloby += TIME * blobvy;

//		blob1->step(TIME);
		
		stepNo = box->step(TIME);

		drawBox(box,stepNo);
		
	//	getchar();

	/*		
		printf("\nblobx: %g, bloby: %g\n",blob1->x, blob1->y);	
		printf("End?\n");
		scanf(" %c",&c);
		if( c == 'y')
		{
			flag = 0;
		}
	*/
	}
	
	// Ncurses close
	endwin();

	return 0;
}

//-------------------------------------------------------------------------------------/

//drawBlob - draws the blob in the console
// (0,0) is in bottom left corner. Like regular (x,y). Counted from (0,0).

int drawBlob(int x, int y)
{
	Ball *ball = new Ball (x, y, 0., 0.);
	Box *box = new Box(HEIGHT, WIDTH);
	box->addBall(ball);
	return drawBox(box);
}

// drawBox draws the blobs on the consoles
//
//

int drawBox (Box* box, int stepNo)
{

	bool flag = true;
	int ballsDrawn = 0;
	
	Ball** ballList = box->getBallList();

	//nCurses clear
	clear();

	for (int j = 0; j < box->getWidth() + 2; j++)
		printw("X");

	printw("\n");


	for (int i = 0; i < box->getHeight(); i++)
	{
		printw("X");
		for (int j = 0; j < box->getWidth(); j++)
		{
			for (int k = 0; k< box->ballCount(); k++)
			{
				flag = false;	
				
				if ( (floor(ballList[k]->x) == j) && (floor(ballList[k]->y) == box->getHeight() - i - 1))
				{
					printw("O");
					
					ballsDrawn++;
					
					flag = true; // checking off, that we actually placed the blob somewhere, debug...

					k = box->ballCount();
				}
			}
			if (flag  == false)
				printw(" ");
		}
		printw("X\n");
	}


	for (int j = 0; j < box->getWidth() + 2; j++)
		printw("X");
	
	printw("\n Step Number: %d", stepNo);

	// Debug
	for (int i = 0; i < box->ballCount(); i++)
	{
		printw("\nBall no %d: x = %f, y = %f, ", i + 1, ballList[i]->x, ballList[i]->y);
		printw("vx = %f, vy = %f", ballList[i]->vx, ballList[i]->vy);
		printw(", rad = %f", ballList[i]->radius);
	}
	printw("\n I like it");
	
	printw("\nRewind Time = %f", box->rewindTime);	
	// nCurses refresh.
	refresh();
		
	return flag;
}
//
