#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>//need to be added to do line, circle
#include <numeric>// for accumulate in isomap MDS

using namespace cv;
using namespace std;

// functions for drawing
void Draw3DManifold(Mat &dataMatrix, char const * name, int nSamplesI, int nSamplesJ);
void Draw2DManifold(Mat &dataMatrix, char const * name, int nSamplesI, int nSamplesJ);

// functions for dimensionality reduction
// first parameter: features saved in rows
// second parameter: desired # of dimensions ( here: 2)
Mat reducePCA(Mat &dataMatrix, unsigned int dim);
Mat reduceIsomap(Mat &dataMatrix, unsigned int dim);
Mat reduceLLE(Mat &dataMatrix, unsigned int dim);

//public functions
vector<map<int, double>> kNearestNeighbor(Mat &dataMatrix, int k);

int main(int argc, char** argv)
{
	// generate Data Matrix
	unsigned int nSamplesI = 10;
	unsigned int nSamplesJ = 10;
	Mat dataMatrix = Mat(nSamplesI*nSamplesJ, 3, CV_64F);
	// noise in the data
	double noiseScaling = 1000.0;

	for (int i = 0; i < nSamplesI; i++)
	{
		for (int j = 0; j < nSamplesJ; j++)
		{
			dataMatrix.at<double>(i*nSamplesJ + j, 0) = (i / (double)nSamplesI * 2.0 * 3.14 + 3.14) * cos(i / (double)nSamplesI * 2.0 * 3.14) + (rand() % 100) / noiseScaling;
			dataMatrix.at<double>(i*nSamplesJ + j, 1) = (i / (double)nSamplesI * 2.0 * 3.14 + 3.14) * sin(i / (double)nSamplesI * 2.0 * 3.14) + (rand() % 100) / noiseScaling;
			dataMatrix.at<double>(i*nSamplesJ + j, 2) = 10.0*j / (double)nSamplesJ + (rand() % 100) / noiseScaling;
		}
	}

	// Draw 3D Manifold
	Draw3DManifold(dataMatrix, "3D Points", nSamplesI, nSamplesJ);

	// PCA
	Mat dataPCA = reducePCA(dataMatrix,2);
	Draw2DManifold(dataPCA,"PCA",nSamplesI,nSamplesJ);

	// Isomap
	Mat dataIsomap = reduceIsomap(dataMatrix,2);
	Draw2DManifold(dataIsomap,"ISOMAP",nSamplesI,nSamplesJ);

	//LLE
	Mat dataLLE = reduceLLE(dataMatrix,2);
	Draw2DManifold(dataLLE,"LLE",nSamplesI,nSamplesJ);

	waitKey(0);


	return 0;
}

Mat reducePCA(Mat &dataMatrix, unsigned int dim)
{
	Mat covar, mean;
	calcCovarMatrix(dataMatrix, covar, mean, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	//cout << covar << endl;// n*3

	//calculate the eigens
	Mat eValues, eVectors;
	//PCA pt_pca(covar, Mat(), CV_PCA_DATA_AS_ROW, 0);Mat pt_eig_vals = pt_pca.eigenvalues;Mat pt_eig_vecs = pt_pca.eigenvectors;
	eigen(covar, eValues, eVectors);

	//get the first 2 eigen vectors
	Mat eVectors0 = eVectors.t();//the eigenvectors are stored as subsequent matrix rows
	Mat U = eVectors0(Rect(0, 0, 2, dataMatrix.cols)).clone();

	Mat dataMatrix0 = dataMatrix * U;//The result of the principal component mapping

	//normalize the result to draw
	normalize(dataMatrix0, dataMatrix0, -0.5, 0.5, NORM_MINMAX);

	return dataMatrix0;
}

Mat reduceLLE(Mat &dataMatrix, unsigned int dim)
{
	int k = 12;
	double distance = 0.0;
	int m = dataMatrix.rows;//number of points, here: 100	
	vector<map<int, double>> sort_result = kNearestNeighbor(dataMatrix, k);
	
	//for (int i = 0; i < sort_result.size(); i++){cout << "==============" << i << endl;
	//for (const auto &p : sort_result[i]) { cout << p.first << "="<< p.second << endl;}}

	
	//define: CmpByValue(); resort by distance from small to large
	struct CmpByValue {
		bool operator()(const pair<int, double> & lhs, const pair<int, double> & rhs)
		{
			return lhs.second < rhs.second;
		}
	};
	
	Mat Zi(k, k, CV_64F);//??????CV_8U CV_32F CV_64F
	Mat e_k(k, 1, CV_64F);
	e_k.setTo(1.0);
	Mat Wi(k, 1, CV_64F);
	Mat W(m, m, CV_64F);
	W.setTo(0.0);
	for (int i = 0; i < m; i++) {
		//cout << "==============" << i << endl;
		double* data_i = dataMatrix.ptr<double>(i);//Xi

		vector<pair<int, double>> sort_result0(sort_result[i].begin(), sort_result[i].end());
		sort(sort_result0.begin(), sort_result0.end(), CmpByValue());

		for (int x = 0; x < sort_result0.size(); ++x) {//.size=k=12
			//cout << sort_result0[x].first << "=" << sort_result0[x].second << endl;
			double* data = Zi.ptr<double>(x);
			//double* data_x = dataMatrix.ptr<double>(sort_result0[x].first);//Xx

			//Zi
			//(solved)pad part: use const 123 but dinamic vals  
			Mat A;
			for (int y = 0; y < k; y++)
			{
				//double* data_y = dataMatrix.ptr<double>(sort_result0[y].first);//Xy

				/*data[y] = (data_i[0] - data_x[0])*(data_i[0] - data_y[0]) +
						  (data_i[1] - data_x[1])*(data_i[1] - data_y[1]) +
						  (data_i[2] - data_x[2])*(data_i[2] - data_y[2]);*/
				A = (dataMatrix.rowRange(i, i + 1) - dataMatrix.rowRange(sort_result0[x].first, sort_result0[x].first + 1))*(dataMatrix.rowRange(i, i + 1) - dataMatrix.rowRange(sort_result0[y].first, sort_result0[y].first + 1)).t();
				data[y] = A.at<double>(0, 0);
			}//cout << A;
		}
		Mat I0 = Mat::eye(k, k, CV_64F);
		Zi += I0 *(float)(0.01 / k) * trace(Zi)[0];

		//Wi;
		Wi = Zi.inv(DECOMP_SVD) * e_k / (e_k.t()*Zi.inv(DECOMP_SVD)*e_k);//size: k * 1
		
		//W
		double* result = W.ptr<double>(i);
		for (int x = 0; x < sort_result0.size(); ++x) {//.size=k=12
			result[sort_result0[x].first] = Wi.at<double>(0, x);
		}
	}
	//cout << W;

	Mat I = Mat::eye(m, m, CV_64F);
	Mat M = (I - W).t() * (I - W);
	Mat eValues, eVectors;
	Mat Y;
	eigen(M, eValues, eVectors);//the eigenvectors are stored as subsequent matrix rows
	Mat Y1 = eVectors.rowRange(m - 2, m - 1).clone();
	Mat Y2 = eVectors.rowRange(m - 3, m - 2).clone();
	//cout << eVectors<<endl;
	vconcat(Y1, Y2, Y);
	//cout << eValues;
	//cout << Y;

	//normalize the result to draw
	normalize(Y, Y, -0.5, 0.5, NORM_MINMAX);

	return Y.t();
}


Mat reduceIsomap(Mat &dataMatrix, unsigned int dim)
{
	int k = 10;
	int row = dataMatrix.rows;//number of points, here: 100	
	double INF = 10000.0;
	vector<double> D(row * row, INF);// ini distance 10000.0

	//return key and val
	vector<map<int, double>> sort_result = kNearestNeighbor(dataMatrix, k);
	
	//assign vals to D
	for (int i = 0; i < sort_result.size(); i++) {
		
		for (const auto &p : sort_result[i]) {			
			D[row * i + p.first] = p.second;	
		}
		D[i*row + i] = 0.0;
	}
	/* check D
	for (int i = 0; i < 100; i++){cout<<"----------------------" << endl;
	for (int j = 0; j < 100; j++){cout << D[i*row+j]<< " ";}}*/

	//Floyd-Warshall algorithm
	vector<int> path(row*row, -1);
	int i, j, x;
	double tmp;

	for (i = 0; i < row; i++)
	{
		for (j = 0; j < row; j++)
		{
			//path[i][j] = j;
			path[i* row + j] = j;//the shortest way of "vertex i"to"vertex j" is through j
			
		}
	}
	//for (int i = 0; i < row; i++) {cout << "----------------------" << i << endl;
		//for (int j = 0; j < row; j++) { cout << path[i*row + j] << " "; }}
	
	for (x = 0; x < row; x++)
	{
		for (i = 0; i < row; i++)
		{
			for (j = 0; j < row; j++)
			{
				// update dist[i][j] & path[i][j] if shorter
				tmp = (D[i*row + x] == INF || D[x*row + j] == INF) ? INF : (D[i*row + x] + D[x*row + j]);
				if (D[i*row + j] > tmp)
				{
					// "the shortest way form i to j" be a smaller No. (through x)
					D[i*row + j] = tmp;
					// "the shortest way form i to j"，through x
					path[i*row + j] = path[i*row + x];
				}
			}
		}
	}


	// check D
	//for (int i = 0; i < 100; i++){cout<<"----------------------" << endl;
	//for (int j = 0; j < 100; j++){cout << D[i*row+j]<< " ";}}

	//MDS method  D->B->X
	vector<double> DSquare;
	for (i = 0; i < D.size(); i++)
	{
		DSquare.push_back(D[i]* D[i]);
	}
	double sum = std::accumulate(std::begin(DSquare), std::end(DSquare), 0.0);
	double mean = (float)sum / D.size();//cout << mean;
	vector<double> rowMean, colMean;
	Mat B(row, row, CV_64F);//??????CV_8U CV_32F CV_64F

	for (i = 0; i < row; i++)
	{
		double rowSum = 0.0;
		for (j = 0; j < row; j++)
		{
			rowSum += DSquare[i*row + j];
		}
		rowMean.push_back((float)rowSum / row);
		//cout << rowMean[i];
	}
	for (j = 0; j < row; j++)
	{
		double colSum = 0.0;
		for (i = 0; i < row; i++)
		{
			colSum += DSquare[i*row + j];
		}
		colMean.push_back((float)colSum / row);
	}
	for (i = 0; i < row; i++)
	{
		double* data = B.ptr<double>(i);
		for (j = 0; j < row; j++)
		{
			double value = -0.5*(DSquare[i*row + j] - rowMean[i] - colMean[j] + mean);
			data[j] = value;
		}
	}
	
	//calculate the eigens
	Mat eValues, eVectors;
	
	eigen(B, eValues, eVectors);
	//cout << eVectors;
	//get the first 2 eigen vectors
	//Mat eVectors0 = eVectors.t();//the eigenvectors are stored as subsequent matrix rows
	Mat U = eVectors(Rect(0, 0, row, 2)).clone();

	//eigenValue matrix N
	Mat	N = Mat(2, 2, CV_64F);//CV_64F
	N.setTo(0.0);

	cv::sort(eValues, eValues, CV_SORT_DESCENDING);
	//cout << U;
	double* data0 = eValues.ptr<double>(0);
	N.at<double>(0, 0) = sqrt(data0[0]);
	data0 = eValues.ptr<double>(1);
	N.at<double>(1, 1) = sqrt(data0[0]);
	
	Mat dataMatrix0 = N* U;
	
	//normalize the result to draw
	normalize(dataMatrix0, dataMatrix0, -0.5, 0.5, NORM_MINMAX);


	return dataMatrix0.t();
}

void Draw3DManifold(Mat &dataMatrix, char const * name, int nSamplesI, int nSamplesJ)
{
	Mat origImage = Mat(1000, 1000, CV_8UC3);
	origImage.setTo(0.0);
	for (int i = 0; i < nSamplesI; i++)
	{
		for (int j = 0; j < nSamplesJ; j++)
		{
			Point p1;
			p1.x = dataMatrix.at<double>(i*nSamplesJ + j, 0)*50.0 + 500.0 - dataMatrix.at<double>(i*nSamplesJ + j, 2) * 10;
			p1.y = dataMatrix.at<double>(i*nSamplesJ + j, 1) *50.0 + 500.0 - dataMatrix.at<double>(i*nSamplesJ + j, 2) * 10;
			circle(origImage, p1, 3, Scalar(255, 255, 255));

			Point p2;
			if (i < nSamplesI - 1)
			{
				p2.x = dataMatrix.at<double>((i + 1)*nSamplesJ + j, 0)*50.0 + 500.0 - dataMatrix.at<double>((i + 1)*nSamplesJ + (j), 2) * 10;
				p2.y = dataMatrix.at<double>((i + 1)*nSamplesJ + j, 1) *50.0 + 500.0 - dataMatrix.at<double>((i + 1)*nSamplesJ + (j), 2) * 10;

				line(origImage, p1, p2, Scalar(255, 255, 255), 1, 8);
			}
			if (j < nSamplesJ - 1)
			{
				p2.x = dataMatrix.at<double>((i)*nSamplesJ + j + 1, 0)*50.0 + 500.0 - dataMatrix.at<double>((i)*nSamplesJ + (j + 1), 2) * 10;
				p2.y = dataMatrix.at<double>((i)*nSamplesJ + j + 1, 1) *50.0 + 500.0 - dataMatrix.at<double>((i)*nSamplesJ + (j + 1), 2) * 10;

				line(origImage, p1, p2, Scalar(255, 255, 255), 1, 8);
			}
		}
	}


	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, origImage);
}

void Draw2DManifold(Mat &dataMatrix, char const * name, int nSamplesI, int nSamplesJ)
{
	Mat origImage = Mat(1000, 1000, CV_8UC3);
	origImage.setTo(0.0);
	for (int i = 0; i < nSamplesI; i++)
	{
		for (int j = 0; j < nSamplesJ; j++)
		{
			Point p1;
			p1.x = dataMatrix.at<double>(i*nSamplesJ + j, 0)*1000.0 + 500.0;
			p1.y = dataMatrix.at<double>(i*nSamplesJ + j, 1) *1000.0 + 500.0;
			//circle(origImage,p1,3,Scalar( 255, 255, 255 ));

			Point p2;
			if (i < nSamplesI - 1)
			{
				p2.x = dataMatrix.at<double>((i + 1)*nSamplesJ + j, 0)*1000.0 + 500.0;
				p2.y = dataMatrix.at<double>((i + 1)*nSamplesJ + j, 1) *1000.0 + 500.0;
				line(origImage, p1, p2, Scalar(255, 255, 255), 1, 8);
			}
			if (j < nSamplesJ - 1)
			{
				p2.x = dataMatrix.at<double>((i)*nSamplesJ + j + 1, 0)*1000.0 + 500.0;
				p2.y = dataMatrix.at<double>((i)*nSamplesJ + j + 1, 1) *1000.0 + 500.0;
				line(origImage, p1, p2, Scalar(255, 255, 255), 1, 8);
			}

		}
	}


	namedWindow(name, WINDOW_AUTOSIZE);
	imshow(name, origImage);
	imwrite((String(name) + ".png").c_str(), origImage);
}

//return vector of (kNearestNeighbor of each point)
vector<map<int, double>> kNearestNeighbor(Mat &dataMatrix, int k)
{
	//double distance = 0.0;
	map<int, double> result;
	vector<double> v;
	vector<int> idx;
	vector<map<int, double>> vec2D;

	for (int m = 0; m < dataMatrix.rows; m++)
	{
		v.clear();
		Mat dataMatrix1 = dataMatrix.clone();
		double* data = dataMatrix.ptr<double>(m);

		for (int i = 0; i < dataMatrix.rows; i++)
		{
			double* data1 = dataMatrix1.ptr<double>(i);
			double val = 0.0, distance = 0.0;

			for (int j = 0; j < dataMatrix.cols; j++)
			{

				val = (data1[j] - data[j]) * (data1[j] - data[j]);
				distance += val;

			}
			v.push_back(sqrt(distance));

		}
		// initialize the index
		vector<int> idx(v.size());
		//give index the value from 0 to n
		for (int a = 0; a < v.size(); a++)
		{
			idx[a] = a;
		}

		//sort the index by comparing the value of v
		sort(idx.begin(), idx.end(),
			[&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });
		/*
		cout << "----------------------------" << m <<endl;
		for (int i = 1; i < k+1; i++){	cout << idx[i] << endl;} 
		*/
		vector<int> sub(&idx[1], &idx[k+1]);//get the first k index

		for (int i = 0; i < k; i++)
		{
			result.insert(pair<int, double>(sub[i], v[sub[i]])); //make_pair
		}
		

		vec2D.push_back(result);
		result.clear();
	}
	

	return vec2D;
}
