#include <stdio.h>
#include <iostream>
#include <math.h>
#include "timing.c"

void dummy(double*a, double*b, double*c, double*d){
	
};

int main(int argc, char *argv[])
{
	double wcs,wce,ct;

	//timer tmr;

	int repeat = 1;
	double runtime=0.;
	int N=0;

	//(A)
	for (int k=3; k<21; ++k)
	{
		N = (int)pow(2.5, k);
		double *a = (double *)malloc (N * sizeof(double));
		double *b = (double *)malloc (N * sizeof(double));
		double *c = (double *)malloc (N * sizeof(double));
		double *d = (double *)malloc (N * sizeof(double));

		for(int i=0; i<N; ++i)
		{
			b[i] = 3.0;
			c[i] = 2.3;
			d[i] = 1.35;
		}

		for(; runtime<.1; repeat*=2)
		{
			timing(&wcs, &ct);
			//tmr.reset();
			for(int r=0; r<repeat; ++r)
			{
				for(int i=0; i<N; ++i)
				{
					a[i] = b[i] + c[i] * d[i];
					//if(CONDITION_NEVER_TRUE) dummy(a); // for the compiler
				}
				if (a[r]<0)
				{
					dummy(a,b,c,d);
				}
				
			}
			timing(&wce, &ct);
			runtime = wce-wcs;
			//runtime = tmr.elapsed();
		}
		repeat /= 2;
		std::cout << "N = " << N << ": " << repeat << std::endl;

		free(a);
		free(b);
		free(c);
		free(d);
	}
	
	//(C)
	for (int k=10; k<23; ++k)
	{
		N = (int)pow(1.5, k);
		double *x = (double *)malloc (N * sizeof(double));
		double *y = (double *)malloc (N * sizeof(double));
		double *A = (double *)malloc (N*N * sizeof(double));

		for(int i=0; i<N; ++i)
			for(int j=0; j<N; ++j)
			{
				A[i*N + j] = 2.0;
			}
		for(int i=0; i<N; ++i)
		{
			x[i] = 3.0;
		}

		for(; runtime<.1; repeat*=2)
		{
			timing(&wcs, &ct);
			//tmr.reset();
			for(int r=0; r<repeat; ++r)
			{
				for(int i=0; i<N; ++i)
					for(int j=0; j<N; ++j)
					{
						y[i] += A[i*N + j] * x[j];
						//if(CONDITION_NEVER_TRUE) dummy(a); // for the compiler
					}
			}
			timing(&wce, &ct);
			runtime = wce-wcs;
			//runtime = tmr.elapsed();

		}
		repeat /= 2;
		std::cout << "N = " << N << ": " << repeat << std::endl;

		free(x);
		free(y);
		free(A);
	}

	system("pause");
	//timing(&wcTimeStart,&cpuTimeStart);

	//timing(&wcTimeEnd,&cpuTimeEnd);
	//wcTime=wcTimeEnd-wcTimeStart;
	//printf("Walltime: %.3lf s\n",wcTime);

}
