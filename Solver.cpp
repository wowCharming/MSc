#include <iostream>
#include "Matrix.h"
#include "Vector.h"
#include "Stencil.h"
#include <chrono>

#define PI 3.141592653589793
using namespace chrono;

const size_t number = 129;

template<typename T, typename MatrixImpl, size_t numPoints>
void solve (const MatrixLike<T, MatrixImpl, numPoints, numPoints>& A, const Vector<T, numPoints>& b, Vector<T, numPoints>& u) {//Matrix<T, rows, cols> , numPoints, numPoints
	const size_t numGridPoints = u.size( );

	double initRes = (b - A * u).l2Norm( ); // determine the initial residual
	double curRes = initRes;
	std::cout << "Initial residual:\t\t" << initRes << std::endl;

	unsigned int curIt = 0; // store the current iteration index

	while (curRes > 1.e-5 * initRes) { // solve until the residual is reduced by a certain amount
		++curIt;

		u += A.inverseDiagonal( ) * (b - A * u); // Jacobi step

		curRes = (b - A * u).l2Norm( ); // update the residual

		if (0 == curIt % 500) // print some info every few steps
			std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl;
	}

	std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl << std::endl; // print the final number of iterations and the final residual
}

void testFullMatrix (const int numGridPoints) {
	const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting full matrix solver for " << numGridPoints << " grid points" << std::endl;

	Matrix<double, number, number> A(0.);
	Vector<double, number> u(0.);
	Vector<double, number> b(0.);

	A(0, 0) = 1.;
	for (int x = 1; x < numGridPoints - 1; ++x) {
		A(x, x - 1) = 1. / hxSq;
		A(x, x) = -2. / hxSq;
		A(x, x + 1) = 1. / hxSq;
	}
	A(numGridPoints - 1, numGridPoints - 1) = 1.;

	for (int x = 0; x < numGridPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numGridPoints - 1)));
	}

	std::cout << "Initialization complete\n";

	//start timing
	auto start = system_clock::now();

	solve(A, b, u);
	//end timing and print elapsed time
	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << "It spends: "
		 << double(duration.count()) * microseconds::period::num / microseconds::period::den
		 << "s" << endl;
	
	cout << A << endl;
	cout << "===========" << endl;
	cout << b << endl;
	cout << "===========" << endl;
	cout << u << endl;
	cout << "===========" << endl;
}

void testStencil (const int numGridPoints) {
	// TODO: add stencil code
	// the stencil can be set up using
	//		Stencil<double> ASten({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });
	const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting full matrix solver for " << numGridPoints << " grid points" << std::endl;

	Vector<double, number> u(0.);
	Vector<double, number> b(0.);

	Stencil<double, number, number> A({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });

	for (int x = 0; x < numGridPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numGridPoints - 1)));
	}

	std::cout << "Initialization complete\n";

	//start timing
	auto start = system_clock::now();

	solve(A, b, u);
	//end timing and print elapsed time
	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << "It spends: "
		 << double(duration.count()) * microseconds::period::num / microseconds::period::den
		 << "s" << endl;

	cout << "===========" << endl;
	cout << b << endl;
	cout << "===========" << endl;
	cout << u << endl;
	cout << "===========" << endl;
}

int main(int argc, char** argv) {
	testFullMatrix(number);
	testStencil(number);
	
	getchar();
}
