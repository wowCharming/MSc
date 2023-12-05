#include <stdio.h>
#include <iostream>
#include "timing.h"

int main(int argc, char *argv[])
{
	double wcTimeStart,wcTimeEnd,cpuTimeStart,cpuTimeEnd,wcTime;

	int SLICES = 1e8;
	double delta_x = 1e-8;
	double x = 0, sum = 0;
	
	timing(&wcTimeStart,&cpuTimeStart);
	
	for (int i = 0; i < SLICES; ++i)
	{
		x = (i+0.5)*delta_x;
		sum += (4.0 / (1.0 + x*x));
	}

	double Pi = sum*delta_x;
	std::cout << Pi << std::endl;
	std::cout << x << std::endl;

	timing(&wcTimeEnd,&cpuTimeEnd);
	wcTime=wcTimeEnd-wcTimeStart;
	printf("Walltime: %.3lf s\n",wcTime);

}

