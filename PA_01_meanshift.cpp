#include<iostream>
#include<opencv2\opencv.hpp>

//#include<opencv2\core\core.hpp>
using namespace cv;
using namespace std;

class Greypixel
{
public:
	Greypixel() :X(0), Y(0) {};

	Greypixel(Mat &IMG, double windowPercent, double radiusPercent) {
		windowSize = windowPercent;
		stopRadius = radiusPercent;
		img = IMG.clone();
	};

	double MeanShift(int x, int y) {

		double greyValue0 = GetGreyValue(x, y);
		int xC = 0, yC = 0, clusterNo = 0;
		int counter = 0;//counter
		double greyValueC = 0, greyValueAve = 0;

		int bandwithCol = windowSize*img.cols;//img.cols=128
		int bandwithRow = windowSize*img.rows;
		int bandwith = 256 * windowSize;

		while (true)
		{
			for (int i = -bandwithRow; i < bandwithRow; i++)
			{
				if (x + i >= 0 && x + i < img.rows)
				{
					for (int j = -bandwithCol; j < bandwithCol; j++)
					{
						if (y + j >= 0 && y + j < img.cols)
						{
							greyValue = GetGreyValue(x + i, y + j);


							if (abs(greyValue - greyValue0) <= bandwith)
							{
								xC += i;
								yC += j;
								greyValueC += greyValue;
								clusterNo++;

							}
						}
					}

				}
			}
			X = xC / clusterNo + x;
			Y = yC / clusterNo + y;
			greyValueAve = greyValueC / clusterNo;
			double a = (double)xC / clusterNo / img.rows;
			double b = (double)yC / clusterNo / img.cols;
			double c = (double)(greyValueAve - greyValue0) / 256;

			//check convergence
			double distance2 = a*a + b*b + c*c;
			
			counter++;
			if (distance2 < stopRadius*stopRadius || counter > 20) break;


		}
		
		

		return greyValueAve;

	}


	double GetGreyValue(int x, int y) {
		//uchar* data = img.ptr<uchar>(x);
		//return data[y];
		return img.ptr<uchar>(x)[y];
	}


	//~Greypixel();

private:
	int X;
	int Y;
	
	double greyValue;

	double windowSize;
	double stopRadius;
	Mat img;
};




int main() {

	//Mat a(300, 300, CV_8UC3, Scalar(0, 0, 0));
	//read image "cameraman_noisy"
	Mat img = imread("cameraman_noisy.png", CV_LOAD_IMAGE_GRAYSCALE);//void namedWindow(const string& winname, int flags=WINDOW_AUTOSIZE)
	if (img.empty())
		std::cout << "failed to open img" << std::endl;
	else
		std::cout << "img loaded OK" << std::endl;
	//creat the window
	//namedWindow("Display window", WINDOW_AUTOSIZE);//WINDOW_NORMAL
	namedWindow("Original window", WINDOW_AUTOSIZE);

	//set window & converge value
	Greypixel img3(img, 0.2, 0.01);

	//main fcn begin here
	Mat img2;
	img2 = img.clone();

	// apply mean-shift
	for (int i = 0; i < img.rows; i++) {
		uchar* data = img2.ptr<uchar>(i);
		for (int j = 0; j < img.cols; j++) {
			data[j] = (uchar)img3.MeanShift(i, j);
			
		}cout << "line: " <<i<<" complete"<< endl;
	}


	//cout << img2 << endl;
	//show the image
	cout << "denoise successful" << endl;
	imshow("Original window", img);
	namedWindow("Denoised window", WINDOW_AUTOSIZE);
	imshow("Denoised window", img2);


	waitKey(0);

	return 0;

}
