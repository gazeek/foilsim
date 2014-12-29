#ifndef BOX_H
#define BOX_H
#include <boost/ptr_container/ptr_list.hpp>

#include "Ball.h"


class Ball;

class Box
{
	private:
		Ball** ballList;
		int nBalls; // number of Balls in Box -- currently
		int ballCapacity; // max number of Balls -- cannot be changed
		
		double boxHeight;
		double boxWidth;
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
		int drawBox (int height);
};


#endif //BOX_H
