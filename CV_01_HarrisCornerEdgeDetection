#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int windowStartX = 10;
int windowStartY = 50;

int windowXoffset = 5;
int windowYoffset = 40;


void show(string name, Mat img, int x, int y)
{
    int w = img.cols + windowXoffset;
    int h = img.rows + windowYoffset;

    cv::namedWindow(name);
    cv::moveWindow(name,windowStartX + w * x,windowStartY + h * y);//moveWindow: move window to the place of screen
    cv::imshow(name, img);
}

cv::Mat harrisResponseImage(cv::Mat img)
{

    show("Input",img,0,0);


    // TODO 1.1
    // Compute the spatial derivatives in x and y
    // direction with the sobel filter: [-1,0,1]

    Mat dIdx(img.rows,img.cols,img.type(),Scalar(0));
    Mat dIdy(img.rows,img.cols,img.type(),Scalar(0));

	//Sobel(img, dIdx, CV_64F, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	//ksize – size of the extended Sobel kernel == 1
	Sobel(img, dIdx, img.depth(), 1, 0, 1);//type remains channel so depth
	Sobel(img, dIdy, CV_32F, 0, 1, 1);//1, 0);

	//Mat Sobel_x = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	//filter2D(img, dIdx, -1, Sobel_x, Point(-1, -1), 0, 0);

    show("dI/dx",abs(dIdx),0,1);
    show("dI/dy",abs(dIdy),1,1);
	waitKey();

    // TODO 1.2
    // Compute Ixx, Iyy, and Ixy with
    // Ixx = (dI/dx) * (dI/dx),
    // Iyy = (dI/dy) * (dI/dy),
    // Ixy = (dI/dx) * (dI/dy).
    // Note: The multiplication between the images is element-wise (not a matrix multiplication)!!
    Mat Ixx(img.rows,img.cols,img.type(),Scalar(0));
    Mat Iyy(img.rows,img.cols,img.type(),Scalar(0));
    Mat Ixy(img.rows,img.cols,img.type(),Scalar(0));

	for (size_t i = 0; i < img.rows; i++)
	{
		float* data_x = dIdx.ptr<float>(i);
		float* data_y = dIdy.ptr<float>(i);
		float* data_xx = Ixx.ptr<float>(i);
		float* data_xy = Ixy.ptr<float>(i);
		float* data_yy = Iyy.ptr<float>(i);
		for (size_t j = 0; j < img.cols; j++)
		{
			data_xx[j] = (float)data_x[j] * data_x[j];
			data_xy[j] = (float)data_x[j] * data_y[j];
			data_yy[j] = (float)data_y[j] * data_y[j];
			//Ixx.at<float>(i, j) = dIdx.at<float>(i, j) * dIdx.at<float>(i, j);
		}
	}
	//show("dIxx", abs(Ixx), 0, 1);
	//waitKey();

    // TODO 1.3
    // Compute the images A,B, and C by blurring the
    // images Ixx, Iyy, and Ixy with a
    // Gaussian filter of size 3x3 and standard deviation of 1.
    Mat A(img.rows,img.cols,img.type(),Scalar(0));
    Mat B(img.rows,img.cols,img.type(),Scalar(0));
    Mat C(img.rows,img.cols,img.type(),Scalar(0));
    Size kernelSize = Size(3,3);
    float sdev = 1;

	GaussianBlur(Ixx, A, kernelSize, 0, 0);
	GaussianBlur(Iyy, B, kernelSize, sdev, 0);
	GaussianBlur(Ixy, C, kernelSize, sdev, 0);


    show("A",abs(A)*5,0,2);
    show("B",abs(B)*5,1,2);
    show("C",abs(C)*5,2,2);
	waitKey();

    // TODO 1.4
    // Compute the harris response with the following formula:
    // R = Det - k * Trace*Trace
    // Det = A * B - C * C
    // Trace = A + B
    Mat response(img.rows,img.cols,img.type(),Scalar(0));
    float k = 0.06;
	Mat Det = A.mul(B) - C.mul(C);//element-wise multiplication
	Mat Trace = A + B;
	response = Det - k * Trace.mul(Trace);
	//show("Harris Response", response, 2, 1);
	//waitKey();

    // Normalize
    double min, max;
    cv::minMaxLoc(response, &min, &max);
    response *= 1.0f / max;
    Mat dbg;
    normalize( response, dbg, 0, 1, NORM_MINMAX, CV_32FC1 );
    show("Harris Response",dbg,2,1);


    return response;
}

std::vector<KeyPoint> harrisKeypoints(Mat response, float threshold = 0.1)
{
    // TODO 2.1
    // Generate a keypoint for a pixel,
    // if the response is larger than the threshold
    // and it is a local maximum.
    //
    // Don't generate keypoints at the image border.

    std::vector<KeyPoint> points;

	for (size_t i = 1; i < response.rows - 1; i++)
	{
		float* data_1 = response.ptr<float>(i-1);
		float* data = response.ptr<float>(i);
		float* data_2 = response.ptr<float>(i+1);

		for (size_t j = 1; j < response.cols - 1; j++)
		{
			if (data[j] > threshold && data[j] >= data_1[j-1]
				&& data[j] >= data_1[j]
				&& data[j] >= data_1[j + 1]
				&& data[j] >= data[j - 1]
				&& data[j] >= data[j + 1]
				&& data[j] >= data_2[j - 1]
				&& data[j] >= data_2[j]
				&& data[j] >= data_2[j + 1])
			{
				KeyPoint kp(j, i, 1);//row & col exchanged
				points.push_back(kp);
			}
		}
	}

    return points;
}

cv::Mat harrisEdges(Mat input, Mat response, float edgeThreshold = -0.01)
{
    // TODO 2.2
    // Set edge pixels to red.
    //
    // A pixel belongs to an edge, if the response is smaller than a threshold
    // and it is a minimum in x or y direction.
    //
    // Don't generate edges at the image border.

    Mat res = input.clone();
	//vector<float> min(response.rows * response.cols);
	//float min;

	////find local min that n pixels
	//int n = 1;//appearently and actually n = 1
	//for (size_t i = n; i < response.rows - n; i++)
	//{
	//	//std::cout << i << endl;
	//	float* data0 = response.ptr<float>(i);
	//	for (size_t j = n; j < response.cols - n; j++)
	//	{
	//		//min[i * response.cols + j] = response.at<float>(i, j);
	//		min = data0[j];
	//		
	//		//find local min for each pixel: i,j
	//		for (size_t x = i - n; x < i + n; x++)
	//		{
	//			float* data = response.ptr<float>(x);
	//			for (size_t y = j - n; y < j + n; y++)
	//			{
	//				//min[i * response.cols + j] = min[i * response.cols + j] < data[y] ? min[i * response.cols + j] : data[y];
	//				min = min < data[y] ? min : data[y];
	//			}
	//		}
	//		//paint to red (combined in the determination)
	//		if (data0[j] < edgeThreshold && data0[j] <= min)
	//		{
	//			res.at<Vec3b>(i, j) = Vec3b(0, 0, 255);//row & col exchanged
	//		}
	//	}
	//}

	for (size_t i = 1; i < response.rows - 1; i++)
	{
		float* data_1 = response.ptr<float>(i - 1);
		float* data = response.ptr<float>(i);
		float* data_2 = response.ptr<float>(i + 1);
		for (size_t j = 1; j < response.cols - 1; j++)
		{
			if (data[j] < edgeThreshold 
				&& (
				(data[j] < data[j - 1]
				&& data[j] < data[j + 1])
				|| (data[j] < data_1[j]
				&& data[j] < data_2[j])
				/*|| (data[j] < data_1[j - 1]
				&& data[j] < data_2[j + 1])
				|| (data[j] < data_1[j + 1]
				&& data[j] < data_2[j - 1])*///not know if need to include different directions
					)
				)
			{
				res.at<Vec3b>(i, j) = Vec3b(0, 0, 255);//row & col exchanged
			}
		}
	}
    return res;
}

int main(int argc, char** argv )
{
    Mat input_img = imread("blox.jpg");
    //convert to floating point grayscale in the range [0,1]
    Mat input_gray;
    cvtColor( input_img, input_gray, CV_BGR2GRAY );
    Mat img;
    input_gray.convertTo(img,CV_32F);
    img *= (1.0f/255.0f);

    //compute corners and edges
    Mat response = harrisResponseImage(img);
    std::vector<cv::KeyPoint> keyPoints = harrisKeypoints(response);
    Mat edges = harrisEdges(input_img,response);
    show("Harris Edges",edges,1,0);

    Mat imgKeypoints1;
    drawKeypoints( input_img, keyPoints, imgKeypoints1, Scalar(0,255,0), DrawMatchesFlags::DEFAULT );
    show("Harris Keypoints",imgKeypoints1,2,0);


    cv::waitKey();
    return 0;
}
