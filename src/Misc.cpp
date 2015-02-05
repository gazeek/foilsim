#include "Misc.h"
// positiveMin returns a positive number that is the smaller of 'a' and 'b'
// Returns -1 if both are non-positive.


double positiveMin(double a, double b)
{
	double temp;
	
	if ( a > 0)
	{
		if (b > 0)
		{
			if (a < b)
				temp = a;
			else
				temp = b;
		}
		else
			temp = a;
	}
	else if (b > 0)
		temp  = b;
	else
		temp = -1;

	return temp;
}


// Same as positive Min, but returns 0 on 0;
double nonNegativeMin(double a, double b)
{
	if (a == 0 || b == 0) 
		return 0;
	else
		return positiveMin(a,b);
}
