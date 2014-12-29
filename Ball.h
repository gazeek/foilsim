#ifndef BALL_H
#define BALL_H

#include "Box.h"

// Ball class is the single class
//
//

class Box;

class Ball {
	private:
		Box* box;
	
	
	public:
		double x;
		double y;
		double vx;
		double vy;

		double radius;

		Ball (double ix, double iy, double ivx, double ivy, double r = 0.5);
		
		void setBox(Box* b)
		{
			box = b;
		}
		
		void step(double steptime);
		
};

#endif 
