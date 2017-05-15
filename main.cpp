#include <stdio.h>
#include <iostream>
#include "timing.h"

int main(int argc, char *argv[])
{
	int repeat = 1;
	double runtime=0.;
	for(; runtime<.1; repeat*=2) {
		timing(&wcs, &ct);
		for(r=0; r<repeat; ++r) {
			/* PUT THE KERNEL BENCHMARK LOOP HERE */
			if(CONDITION_NEVER_TRUE) dummy(a); // for the compiler
		}
		timing(&wce, &ct);
		runtime = wce-wcs;
	}
	repeat /= 2;

}
