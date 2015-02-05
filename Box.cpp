#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
#include <cmath>

#include "Box.h"
#include "Misc.h"

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
	// TODO what if the hit each other close5 to the border? Does the order matter? (It might)
	
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
					double drx = ball1->x - ball2->x;
					double dry = ball1->y - ball2->y;
					double dvx = ball1->vx - ball2->vx;
					double dvy = ball2->vy - ball2->vy;
					double radSum = ball1->radius + ball2->radius;
					
					// Creating a parameters for a quadratic equations
					// a * t^2 + b* t + c = 0
					// TODO High Priority, make sure the solution is good, ie a != 0, dela > 0.
					double a = (dvx * dvx) + (dvy * dvy);
					double b = 2. * ((dvx * drx) + (dvy * dry));
					double c = (drx * drx) + (dry * dry) - (radSum * radSum);

					double delta = (b * b) - 4 * a * c;

					double time1 = 0.5 *  ( -b / a + sqrt(delta / (a * a)));
					double time2 = 0.5 *  ( -b / a - sqrt(delta / (a * a)));
						
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
					double u1x = ball1->vx - ball2->vx;
					double u1y = ball1->vy - ball2->vy;
					
					// position2 - position1 is the vector direction that ball2 will 
					// travel on in the reference frame associated with ball2
					// |u2p| = |u1| * (cos (phi) * cos (alpha) + sin (phi) * sin (aplha))
					// |u2p| = cos (phi) * u1x + sin (phi) * u1y
					double dp2p1x = ball2->x - ball1->x;
					double dp2p1y = ball2->y - ball1->y;
					
					// cos (phi) = dp2p1x / radSum
					// sin (phi) = dp2p1y / radSum
					double u2px = dp2p1x / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
					double u2py = dp2p1y / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
					
					// Masses are equal so:
					// u1p + u2p = u1
				       	double u1px = u1x - u2px;
					double u1py = u1y - u2py;
					 
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


// double timeOfBallHit (Ball ball1, Ball ball2) gives the time when ball1 will hit with ball2.

double Box::timeOfBallHit(Ball *ball1, Ball *ball2)
{
	double timeOfHit;
	
	// First calculating the time when the hit took place, 
	double drx = ball1->x - ball2->x;
	double dry = ball1->y - ball2->y;
	double dvx = ball1->vx - ball2->vx;
	double dvy = ball2->vy - ball2->vy;
	double radSum = ball1->radius + ball2->radius;
			
	// Creating a parameters for a quadratic equations
	// a * t^2 + b* t + c = 0
	// TODO High Priority, make sure the solution is good, ie a != 0, dela > 0.
	double a = (dvx * dvx) + (dvy * dvy);
	double b = 2. * ((dvx * drx) + (dvy * dry));
	double c = (drx * drx) + (dry * dry) - (radSum * radSum);

	double delta = (b * b) - 4 * a * c;

	double time1 = 0.5 *  ( -b / a + sqrt(delta / (a * a)));
	double time2 = 0.5 *  ( -b / a - sqrt(delta / (a * a)));
		
      	// We are looking for when we will hit the second ball.
	// If we won't do that in the future timeOfHit = -1;
	timeOfHit = nonNegativeMin(time1, time2);

	if (timeOfHit == 0)
	{
		// If one time is negative, the hit happened in the past, 
		// If both are 0, the balls are tangent
		if( (time1 == 0 && time2 <= 0) || (time1 <= 0 && time2 == 0) )
			timeOfHit = -1.0;
		// Otherwise the hit is happening right now, so since timeOfHit == 0
		// we got the right answer.

	}

	return timeOfHit;
	/*
	// Rewinding simulation time to the time of the hit.
	ball1->step(rewindTime);
	ball2->step(rewindTime);
	
	// Calculating ball deflection velocities
	// Changing velocities to the frame of reference cennected with ball2 (it stops moving).`
	// We create 2 new velocities associated with ball2 frame of reference.
	// Therefroe u2 == 0, u1 is the speed of ball1 in ball2 reference frame.
	double u1x = ball1->vx - ball2->vx;
	double u1y = ball1->vy - ball2->vy;
	
	// position2 - position1 is the vector direction that ball2 will 
	// travel on in the reference frame associated with ball2
	// |u2p| = |u1| * (cos (phi) * cos (alpha) + sin (phi) * sin (aplha))
	// |u2p| = cos (phi) * u1x + sin (phi) * u1y
	double dp2p1x = ball2->x - ball1->x;
	double dp2p1y = ball2->y - ball1->y;
	
	// cos (phi) = dp2p1x / radSum
	// sin (phi) = dp2p1y / radSum
	double u2px = dp2p1x / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
	double u2py = dp2p1y / radSum * (dp2p1x / radSum * u1x + dp2p1y / radSum * u1y);
	
	// Masses are equal so:
	// u1p + u2p = u1
       	double u1px = u1x - u2px;
	double u1py = u1y - u2py;
	 
	// Adjusting to original frame of reference
	ball1->vx = u1px + ball2->vx;
	ball1->vy = u1py + ball2->vy;

	ball2->vx = u2px + ball2->vx;
	ball2->vy = u2py + ball2->vy;

	// Moving time forward
	ball1->step( -rewindTime);
	ball2->step( -rewindTime);
	*/
		
}	

// Returns the time of the first time a ball hits a wall
// Returns 0 if it is hittting the wall right now, that is it's distance is zero and it is moving towards it.
// Returns -1 if it will never hit a wall, most likely an error occured.

double Box::timeOfWallHit(Ball *ball)
{
	double timeOfHit;


	// Checking when ball will hit a wall
	
	// Looking for the minimum time 
	
	// Checking which wall ball will hit in this order:
	//
	//    3_
	//   |  |
	// 4 |__|2
	//    1
	//
	
	double tempTime1 = (ball->y - ball->radius) / ball->vy;
	double tempTime2 = (this->getWidth() - ball->radius - ball->x) / ball->vx;
	double tempTime3 = (this->getHeight() - ball->radius - ball->y) / ball->vy;
	double tempTime4 = (ball->x - ball->radius) / ball->vx;
	
	// Checking whether the ball is against a wall and is going towards that wall. 
	// If yes, the ball is hitting the wall, timeOfHit = 0;
	if ( (tempTime1 == 0 && ball->vy < 0) /
		(tempTime2 == 0 && ball->vx > 0) /
		(tempTime3 == 0 && ball->vy >0) /
		(tempTime4 == 0 && ball->vx < 0) ) timeOfHit = 0;
	else // It is not against a wall and moving towards it.
	{
		timeOfHit = positiveMin(tempTime1, tempTime2);
		timeOfHit = positiveMin(timeOfHit, tempTime3);
		timeOfHit = positiveMin(timeOfHit, tempTime4);
		
	}
	return timeOfHit;	
}
