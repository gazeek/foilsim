#include "Ball.h"

Ball::Ball (double ix, double iy, double ivx, double ivy, double r /* = 0.5*/)
{
	x = ix;
	y = iy;
	vx = ivx;
	vy = ivy;

	radius = r;
}

void Ball::step(double steptime)
{
	x += vx * steptime;
	y += vy * steptime;
}
