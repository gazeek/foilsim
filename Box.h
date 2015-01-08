#ifndef BOX_H
#define BOX_H
#include <boost/ptr_container/ptr_list.hpp>

#include "Ball.h"


class Ball;

class Box
{
	private:
		double boxHeight; // height of Box
		double boxWidth; // width of Box
		
		Ball** ballList; // list of all the balls
		int nBalls; // number of Balls in Box -- currently
		int ballCapacity; // max number of Balls -- cannot be changed

		int stepCount; // counts the how many simulation steps occured
	public:



		Box (double h, double w, int ballCap = 1);
		
		Ball** getBallList() 
		{
			return ballList;
		}	
		
		double getWidth()
		{
			return boxWidth;
		}

		double getHeight()
		{
			return boxHeight;
		}

		int ballCount()
		{
			return nBalls;
		}

		void addBall(Ball* ball);

		int step(double time = 0.5);

		// Debug:
		double rewindTime;
};


#endif //BOX_H
