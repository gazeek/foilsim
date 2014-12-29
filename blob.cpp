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
int drawBox (Box* box, int stepNo); 


int main()
{
	double blobx =  WIDTH / 2, bloby =  HEIGHT / 2;
	double blobvx = -1.0, blobvy = -1.0 ;
	int flag = 100;
	int stepNo = 0;
	char c;
	
	Box *box = new Box(HEIGHT, WIDTH, BLOBNUMBER);
	
	Ball *blob1 = new Ball(blobx, bloby, blobvx, blobvy, RADIUS);
	Ball *blob2 = new Ball (10,20,1,1);

	box->addBall(blob1);
	box->addBall(blob2);
	//Ncurses initiation
	initscr();

	while (flag)
	{
		
		usleep(150000);

		blobx += TIME * blobvx;
		bloby += TIME * blobvy;

		blob1->step(TIME);
		
		stepNo++;

		//checking boundaries;

		if ( (int) blob1->x >  box->getWidth() - blob1->radius )
		{
			blob1->x = (box->getWidth() - blob1->radius) - (blob1->x - (box->getWidth() - blob1->radius));
			blob1->vx = -blob1->vx;
		}

		if ( (int) blob1->x < blob1->radius)
		{
			blob1->x = blob1->radius + (blob1->radius - blob1->x);
			blob1->vx = -blob1->vx;
		}


		if ( (int) blob1->y >= box->getHeight() - blob1->radius)
		{
			blob1->y = (box->getHeight() - blob1->radius) - (blob1->y - (box->getHeight() - blob1->radius));
			blob1->vy = -blob1->vy;
		}
		
		if ( (int) blob1->y < blob1->radius )
		{
			blob1->y = blob1->radius + (blob1->radius -blob1->y);
			blob1->vy = -blob1->vy;
		}
		


	
///		clear();	

		
		drawBox(box,stepNo);
		
//		refresh();
		//printf("I like this: %d", stepNo);		
//		drawBox(blob1, stepNo);

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

	endwin();

	return 0;
}

//-------------------------------------------------------------------------------------/

//drawBlob - draws the blob in the console
// (0,0) is in bottom left corner. Like regular (x,y). Counted from (0,0).

int drawBlob(int x, int y)
{
	Ball *ball = new Ball (x, y, 0., 0.);

	return 0;//drawBox(ball);
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

	printw("\n I like it");
	
	// nCurses refresh.
	refresh();
		
	return flag;
}
//
