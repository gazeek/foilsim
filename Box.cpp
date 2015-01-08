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
	stepCount = 0;

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

//step performs one step in the simulation.
// First I assume the balls didn't hit anything and advance them accordingly and then I check if they actually hit something
// and adjust them accordingly
//
int Box::step(double time)
{
	Ball *ball;

	Ball *ball1, *ball2;
	
	// Moving balls forward
	for (int i = 0; i < nBalls; i++)
	{
		(ballList[i])->step(time);
	}	

	// Checking wheter any balls are too close to the walls
	// TODO implement Rewind Time methodology
	for (int i = 0; i < nBalls; i++)
	{
		ball = ballList[i];


		if ( (int) ball->x >  this->getWidth() - ball->radius )
		{
			ball->x = (this->getWidth() - ball->radius) - (ball->x - (this->getWidth() - ball->radius));
			ball->vx = -ball->vx;
		}

		if ( (int) ball->x < ball->radius)
		{
			ball->x = ball->radius + (ball->radius - ball->x);
			ball->vx = -ball->vx;
		}


		if ( (int) ball->y >= this->getHeight() - ball->radius)
		{
			ball->y = (this->getHeight() - ball->radius) - (ball->y - (this->getHeight() - ball->radius));
			ball->vy = -ball->vy;
		}
		
		if ( (int) ball->y < ball->radius )
		{
			ball->y = ball->radius + (ball->radius -ball->y);
			ball->vy = -ball->vy;
		}	
	}

	// Checking whether 2 balls hit each other
	// TODO what if the hit each other close to the border? Does the order matter? (It might)
	
	for (int i = 0; i < nBalls; i++)
	{
		for (int j = 0; j < nBalls; j++)
		{
			if ( j != i)
			{
				ball1 = ballList[i];
				ball2 = ballList[j];
				
				// Detecting collision
				// (x1 - x2) ^ 2 + (y1 - y2) ^ 2 < (radius1 + radius2)  ^ 2
				if ( pow(ball1->x - ball2->x, 2) + pow(ball1->y - ball2->y,2) < pow(ball1->radius + ball2->radius,2) )
				{
					// Adjusting to calculate ball deflection

					// First calculating the time when the hit took place, 
					const double drx = ball1->x - ball2->x;
					const double dry = ball1->y - ball2->y;
					const double dvx = ball1->vx - ball2->vx;
					const double dvy = ball2->vy - ball2->vy;
					const double radSum = ball1->radius + ball2->radius;
					
					// Creating a parameters for a quadratic equations
					// a * t^2 + b* t + c = 0
					// TODO High Priority, make sure the solution is good, ie a != 0, dela > 0.
					const double a = (dvx * dvx) + (dvy * dvy);
					const double b = 2. * ((dvx * drx) + (dvy * dry));
					const double c = (drx * drx) + (dry * dry) - (radSum * radSum);

					const double delta = (b * b) - 4 * a * c;

					const double time1 = 0.5 *  ( -b / a + sqrt(delta / (a * a)));
					const double time2 = 0.5 *  ( -b / a - sqrt(delta / (a * a)));
						
			       		// We are looking for the negative time
					//double rewindTime;

					if (time1 < 0)
						rewindTime = time1;
					else 
						rewindTime = time2;
					
					// Rewinding simulation time to the time of the hit.
					ball1->step(rewindTime);
					ball2->step(rewindTime);
					
					// Calculating ball deflection velocities
					// Changing velocities to the frame of reference cennected with ball2 (it stops moving).`
					// We create 2 new velocities associated with ball2 frame of reference.
					// Therefroe u2 == 0, u1 is the speed of ball1 in ball2 reference frame.
					const double u1x = ball1->vx - ball2->vx;
					const double u1y = ball1->vy - ball2->vy;
					
					// position2 - position1 is the vector direction that ball2 will 
					// travel on in the reference frame associated with ball2
					// |u2p| = |u1| * (cos (phi) * cos (alpha) + sin (phi) * sin (aplha))
					// |u2p| = cos (phi) * u1x + sin (phi) * u1y
					const double dp2p1x = ball2->x - ball1->x;
					const double dp2p1y = ball2->y - ball1->y;
					
					// cos (phi) = dp2p1x / radSum
					// sin (phi) = dp2p1y / radSum
					const double u2px = dp2p1x / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
					const double u2py = dp2p1y / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
					
					// Masses are equal so:
					// u1p + u2p = u1
				       	const double u1px = u1x - u2px;
					const double u1py = u1y - u2py;

					// Adjusting to original frame of reference
					ball1->vx = u1px + ball2->vx;
					ball1->vy = u1py + ball2->vy;

					ball2->vx = u2px + ball2->vx;
					ball2->vy = u2py + ball2->vy;

					// Moving time forward
					ball1->step( -rewindTime);
					ball2->step( -rewindTime);
				}
			}

		}


	}


	stepCount++;

	return stepCount;
}

