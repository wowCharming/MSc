#include "DensityTree.h"
#include <iostream>
#include <time.h>
#include <cmath>
using namespace cv;
using namespace std;
# define PI 3.14159265358979323846

class ResultVectors //data in vector<Mat> & vector<double>
{
public:
	//ResultVectors() : LR(0), points(0) { }
	void addDouble(double P)
	{
		points.push_back(P);
	}
	void addMat(Mat M)
	{
		LR.push_back(M);
	}
	Mat getMat(int index) 
	{
		return LR[index];
	}
	double getValue(int index)
	{
		return points[index];
	}
	vector<Mat> getMatVector()
	{
		return LR;
	}
	vector<double> getValueVector()
	{
		return points;
	}
private:
	vector<Mat> LR;
	vector<double> points;
};



//public functions
double getInfoGain(Mat whole, Mat left, Mat right);//, int countX, int countY);
double getDensity(double value, double mean, double variance);
double getMean(Mat M, int XorY);
double getVariance(Mat M, double mean, int XorY);
ResultVectors splitFunction(int index, int XorY, int n_thresholds, ResultVectors result);//vector<Mat>

DensityTree::DensityTree(unsigned int D, unsigned int n_thresholds, Mat X) 
{
    this-> D=D;//n_levels = 2, which can be modified
    this-> X=X;//X.rows = no. of samples = 1000
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

	ResultVectors result;
	result.addMat(X);

	//split fcn.
	int index = 0, XorY = 0;
	for (int i = 0; i < D - 1; i++)//
	{
		XorY = i % 2;
		if (i == 0){index = 0;}
		else{index += pow(2, i-1);}//0, 1, 1+2, 1+2+4
		
		for (int j = 0; j < pow(2, i); j++)
		{
			result = splitFunction((j + index), XorY, n_thresholds, result);//LR[j]?? : j + index //LR[j + index]
		}
	}

	points = result.getValueVector();
	//cout << "LR.size:" << result.getMatVector().size() << endl;
	//cout << "points.size:" << result.getValueVector().size() << endl;
	//cout << "index:" << index << endl;

	int index1 = 0;
	for (int a = 0; a < D-1; a++)//1,1+2,1+2+4
	{
		index1 += pow(2, a); 
	}
	//cout << "index1:" << index1 << endl;
	//cout << "=======================" << endl;

	vector<Mat> LR = result.getMatVector();

	//cout << LR.size() << endl;
	//for (int i = 0; i < LR.size(); i++) { cout << LR[i].rows << endl; }
	vector<Mat> leafNodes0;
	leafNodes0.assign(LR.begin()+index1, LR.end());//vector<Mat>  &LR[index1 + N]
	leafNodes = leafNodes0;
	//cout <<"leafNodes.size:"<< leafNodes.size() << endl;
	//for (int i = 0; i < leafNodes.size(); i++) { cout << leafNodes[i].rows << endl; }
	//store the data: means & variances
	double mean;
	for (int i = 0; i < N; i++)
	{
		/*Scalar meanXX, sdX;
		meanStdDev(leafNodes[i].col(0), meanXX, sdX);
		meansX.push_back(meanXX[0]);
		variancesX.push_back(pow(sdX[0],2));*/
		mean = getMean(leafNodes[i], 0);
		meansX.push_back(mean);
		variancesX.push_back(getVariance(leafNodes[i], mean, 0));

		mean = getMean(leafNodes[i], 1);
		meansY.push_back(mean);
		variancesY.push_back(getVariance(leafNodes[i], mean, 1));

		//sampleAmount[i] = leafNodes[i].rows;
	}
	//cout << "=======================" << endl;
	//cout << "means.size:" << meansX.size() << endl;
	//cout << "variances.size:" << variancesX.size() << endl;
	//for (int i = 0; i < meansX.size(); i++) { cout << meansX[i] << endl; }
	//for (int i = 0; i < variancesX.size(); i++) { cout << variancesX[i] << endl; }
}

Mat DensityTree::densityXY()
{
	//vector<double> result;
	//cout << "points.size:" << points.size() << endl;
	//for (int i = 0; i < points.size(); i++) { cout << points[i] << endl; }
	Mat resultX;
	
	int XorY1 = 0, index, index_p = 0;

	Mat Answer(X.rows, X.cols, CV_64F);
	for (int s = 0; s < X.rows; s++)//each sample
	{
		index = 0;
		double* data = X.ptr<double>(s);
		double* data1 = Answer.ptr<double>(s);
		for (int n = 0; n < D - 1; n++)//decision node layer
		{
			if (n == 0) { index_p = 0; }
			else { index_p += pow(2, n - 1); }

			//cout << "index_p:" << index_p << endl;
			XorY1 = n % 2;
			index *= 2;

			if (data[XorY1] <= points[index_p])//bigger to left //problem here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
				index += 1;//like binary
				index_p += n + 1; 
			}
			
			//cout << "index:" << index << endl;

		}
		double ansX = (double)(1 / sqrt(variancesX[index] * 2 * PI)) * exp(-0.5*pow(data[0] - meansX[index],2)/variancesX[index]);
		double ansY = (double)(1 / sqrt(variancesY[index] * 2 * PI)) * exp(-(data[1] - meansY[index])*(data[1] - meansY[index]) / (2 * variancesY[index]));
		data1[0] = (double)ansX *leafNodes[index].rows / X.rows;//adding The scalar which is the proportion of all training points that reach the leaf
		data1[1] = (double)ansY *leafNodes[index].rows / X.rows;
		

	}
    return Answer;
}

//core fcn. vector<Mat>
ResultVectors splitFunction(int index, int XorY, int n_thresholds, ResultVectors result)
{
	Mat input = result.getMat(index);

	//max min
	double minv = 0.0, maxv = 0.0, range = 0.0;
	double* minp = &minv;
	double* maxp = &maxv;
	Mat	X1 = input.col(XorY).clone();//X.colRange(0, 1).clone();
	minMaxIdx(X1, minp, maxp);
	range = maxv - minv;//around 32.9369

	Mat leftSet, rightSet, leftResult, rightResult;//left set; right set
	double splitPoint = 0.0, InfoGain = 0.0;
	for (int i = 0; i < n_thresholds; i++)
	{
		//srand((unsigned)time(NULL));
		double randNo = rand() / double(RAND_MAX);//double from 0 to 1
		double point = minv + range * randNo;

		//get 2 (left,right) sets, clear first
		int countX = 0, countY = 0;
		leftSet.release();
		rightSet.release();
		for (int x = 0; x < input.rows; x++)
		{
			double* data = input.ptr<double>(x);
			//cout << data[0]<<"---------------------"<< point << endl;
			if (data[0] >= point)//bigger then left
			{
				Mat L = input.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				leftSet.push_back(L);
			}
			else
			{
				Mat R = input.row(x).clone();//from 0 to N: rowRange(x, x + 1)
				rightSet.push_back(R);
			}
		}//cout << leftSet << countX <<endl;
		//cout << getInfoGain(input, leftSet, rightSet) << endl;
		//eliminate the situation that oneside has no point in fcn(getInfoGain)//leftSet.rows * rightSet.rows > 0
		if (InfoGain < getInfoGain(input, leftSet, rightSet))//todo <=0 better?
		{
			InfoGain = getInfoGain(input, leftSet, rightSet);//, countX, countY);//leftSet.rows, rightSet.rows
			leftResult = leftSet;
			rightResult = rightSet;
			splitPoint = point;
		}
	}
	//cout <<"infogain:"<< InfoGain << endl;
	//points.push_back(splitPoint);
	//vector<Mat> LR;
	//LR.push_back(leftResult);
	//LR.push_back(rightResult);

	result.addDouble(splitPoint);
	result.addMat(leftResult);
	result.addMat(rightResult);

	return result;

}

double getMean(Mat M, int XorY) 
{
	double sum = 0.0;
	for (int i = 0; i < M.rows; i++)
	{
		double* data = M.ptr<double>(i);
		sum += data[XorY];
	}
	return (double)sum / M.rows;
}

double getVariance(Mat M, double mean, int XorY)
{
	double sum = 0.0;
	for (int i = 0; i < M.rows; i++)
	{
		double* data1 = M.ptr<double>(i);
		sum += (data1[XorY] - mean) * (data1[XorY] - mean);
	}
	return (double)sum / M.rows;
}

double getDensity(double value, double mean, double variance)
{
	double ans = (float)(1 / sqrt(variance * 2 * PI)) * exp(-(value - mean)*(value - mean) / (2 * variance));
	return ans;
}



//getInfoGain; if row = 1, calcCovarMatrix will be 0,then -log will be inf
double getInfoGain(Mat whole, Mat left, Mat right)//, int countX, int countY)
{
	Mat covarWhole, covarLeft, covarRight, mean;
	double I = 0.0;
	int countX = left.rows;
	int countY = right.rows;
	if (countX*countY == 0)//eliminate the situation that oneside has no point by return 0
	{
		return 0;
	}
	calcCovarMatrix(whole, covarWhole, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	calcCovarMatrix(left, covarLeft, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	calcCovarMatrix(right, covarRight, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);

	double leftInfo = log(determinant(covarLeft));
	double rightInfo = log(determinant(covarRight));
	if (countX == 1)
	{
		leftInfo = 0.0;
	}
	if (countY == 1)
	{
		rightInfo = 0.0;
	}

	I = log(determinant(covarWhole)) - ((double)countX / whole.rows)*leftInfo -
		((double)countY / whole.rows)*rightInfo;

	return I;
}
