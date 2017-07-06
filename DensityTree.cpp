#include "DensityTree.h"
#include <iostream>
#include <time.h>
#include <cmath>
using namespace cv;
using namespace std;
# define PI 3.14159265358979323846

//public functions
double getInfoGain(Mat whole, Mat left, Mat right);//, int countX, int countY);
double getDensity(double value, double mean, double variance);
double getMean(Mat M);
double getVariance(Mat M, double mean);


DensityTree::DensityTree(unsigned int D, unsigned int n_thresholds, Mat X) 
{
    this-> D=D;//n_levels = 2, which can be modified
    this-> X=X;//X.rows = no. of samples
    this-> n_thresholds=n_thresholds;//n_thresholds = 50
	//this-> N= pow(2, D - 1);//no. of leaf nodes
}

void DensityTree::train()
{
	int N = pow(2, D - 1);//no. of leaf nodes

	////bagging: boostrap aggregation.  without index
	//Mat samplingSet;
	//for (int i = 0; i < N; i++)
	//{
	//	int random = rand() % N;
	//	Mat R = X.rowRange(random, random + 1).clone();//from 0 to N
	//	samplingSet.push_back(R);
	//}

	//max min
	double minv = 0.0, maxv = 0.0, range = 0.0;
	double* minp = &minv;
	double* maxp = &maxv;
	Mat	X1 = X.col(0).clone();//X.colRange(0, 1).clone();
	minMaxIdx(X1, minp, maxp);
	range = maxv - minv; //around 32.9369
	//cout << minv << endl;

	Mat leftSet, rightSet;// , leftResult, rightResult;//left set; right set
	double splitPoint, InfoGain = 0.0;
	for (int i = 0; i < n_thresholds; i++)
	{
		InfoGain = 0.0;
		//srand((unsigned)time(NULL));
		double randNo = rand() / double(RAND_MAX);//double from 0 to 1
		double point = minv + range * randNo;

		//get 2 (left,right) sets, clear first
		int countX = 0, countY = 0;
		leftSet.release();
		rightSet.release();
		for (int x = 0; x < X.rows; x++)
		{
			double* data = X.ptr<double>(x);
			//cout << data[0]<<"---------------------"<< point << endl;
			if (data[0] <= point)
			{
				Mat L = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				leftSet.push_back(L);
				//countX++;//leftSet.rows
			}
			else
			{
				Mat R = X.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				rightSet.push_back(R);
				//countY++;
			}
		}//cout << leftSet << countX <<endl;


		if (InfoGain < getInfoGain(X, leftSet, rightSet))
		{
			InfoGain = getInfoGain(X, leftSet, rightSet);//, countX, countY);//leftSet.rows, rightSet.rows
			leftResult = leftSet;
			rightResult = rightSet;
			splitPoint = point;
		}
		
	}

	cout << "=======================" << endl;


	//store the data
	mean = getMean(leftResult);
	means.push_back(mean);
	variances.push_back(getVariance(leftResult, mean));

	mean = getMean(rightResult);
	means.push_back(mean);
	variances.push_back(getVariance(rightResult, mean));

    cout << "Not implemented " << means.size()<< endl;//Temporla
}
Mat DensityTree::densityXY()
{
	vector<double> result;
	for (int i = 0; i < leftResult.rows; i++)
	{
		double* data = leftResult.ptr<double>(i);
		double ans = getDensity(data[0], mean, getVariance(rightResult, mean));//×óÓÒ
		//double ans = (float)(1 / sqrt(variance * 2 * PI)) * exp(-(data[0] - mean)*(data[0] - mean) / (2 * variance));

		result.push_back(ans);
	}
	//double test=0.0;
	for (int i = 0; i < result.size(); i++) { cout << result[i] << endl; }//not 1, test += result[i];
	//cout << "=======================" << endl;
	//cout << test << endl;

	//return result;
    return X;//Temporal
}

double getMean(Mat M) 
{
	double sum = 0.0;
	for (int i = 0; i < M.rows; i++)
	{
		double* data = M.ptr<double>(i);
		sum += data[0];
	}
	return (double)sum / M.rows;
}

double getVariance(Mat M, double mean)
{
	double sum = 0.0;
	for (int i = 0; i < M.rows; i++)
	{
		double* data1 = M.ptr<double>(i);
		sum += (data1[0] - mean) * (data1[0] - mean);
	}
	return (double)sum / M.rows;
}

double getDensity(double value, double mean, double variance)
{
	double ans = (float)(1 / sqrt(variance * 2 * PI)) * exp(-(value - mean)*(value - mean) / (2 * variance));
	return ans;
}



//getInfoGain
double getInfoGain(Mat whole, Mat left, Mat right)//, int countX, int countY)
{
	Mat covarWhole, covarLeft, covarRight, mean;
	int countX = left.rows;
	int countY = right.rows;
	calcCovarMatrix(whole, covarWhole, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	calcCovarMatrix(left, covarLeft, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	calcCovarMatrix(right, covarRight, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);

	/*Mat eValues, eVectors;
	eigen(covarWhole, eValues, eVectors);*/

	double I = log(determinant(covarWhole)) - ((double)countX / whole.rows)*log(determinant(covarLeft)) -
		((double)countY / whole.rows)*log(determinant(covarRight));

	return I;
}