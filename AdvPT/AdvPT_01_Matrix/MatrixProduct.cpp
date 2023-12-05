#include "Matrix.h"
#include <iostream>
#include <cassert>
#include <cmath>

using std::cout;
using std::endl;


int main(int argc, char** argv)
{
	int s1, s2, s3;
	cout << "please input three integral numbers" << endl;
	cin >> s1 >> s2 >> s3;
	if (!s1 * s2 * s3 > 0) cout << "please check the input numbers" << endl;
	
	Matrix m1(s1, s2);
	Matrix m2(s2, s3);
	Matrix m3(s1, s3);
	double temp;

	cout << "please input menbers of m1" << endl;
	cin >> m1;
	/*for (int i = 0; i < s1; i++)
	{
		for (int j = 0; j < s2; j++)
		{
			cout << "input m1(" << i << "," << j << ")" << endl;
			cin >> temp;
			m1(i, j) = temp;
		}

	}*/

	cout << "please input menbers of m2" << endl;
	cin >> m2;
	/*for (int i = 0; i < s2; i++)
	{
		for (int j = 0; j < s3; j++)
		{
			cout << "input m2(" << i << "," << j << ")" << endl;
			cin >> temp;
			m2(i, j) = temp;
		}

	}*/

	m3 = m1 * m2;
	
	cout << "here is m1: " << endl << m1 << endl;
	cout << "here is m2: " << endl << m2 << endl;
	cout << "here is the answer m3:" << endl;
	cout << m3 << endl;
	getchar();
	cout << "end" << endl;
	getchar();
	/* TEST PART
	Matrix m1(3, 2, 1);
	Matrix m2(2, 3, 1);
	//Matrix m3(1, 2, 1);
	Matrix m3 = m1 * m2;
	//Matrix m1 *= m2;
	
	cout << m1 << endl;
	cout << "===========" << endl;
	cout << m2 << endl;
	cout << "===========" << endl;
	cout << m3 << endl;
	cout << "===========" << endl;
	m1 *= m2;
	cout << m1 << endl;
	cout << "===========" << endl;
	getchar();*/
}
