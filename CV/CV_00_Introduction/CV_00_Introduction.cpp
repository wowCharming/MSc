#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

	// TODO 3.1
	// - Load Image
	// - Show it on screen until a key is pressed
	String file = "img.png";
	Mat image;
	image = imread(file, CV_LOAD_IMAGE_COLOR);//void namedWindow(const string& winname, int flags=WINDOW_AUTOSIZE)
	imshow("Original image", image);
	waitKey(10000);
	imwrite("./img.jpg", image);
	//getchar();

	// TODO 3.2
	// - Resize Image by a factor of 0.5
	// - Show it on screen
	// - save as small.jpg
	Mat smallImage;
	resize(image, smallImage, Size(round(0.5*image.cols), round(0.5*image.rows)));
	imshow("Small image", smallImage);
	waitKey(10000);
	imwrite("./small.png", smallImage);


	// TODO 3.3
	// - Create 3 Mats of the same size and type as the image.
	// - Split the image into its RGB channels.
	// - Display the channels on screen.
	Mat imageR, imageG, imageB;
	//ini important!!
	imageB = Mat::zeros(image.rows, image.cols, CV_8UC3);
	imageG = Mat::zeros(image.rows, image.cols, CV_8UC3);
	imageR = Mat::zeros(image.rows, image.cols, CV_8UC3);

	for (size_t i = 0; i < image.rows; i++)
	{
		for (size_t j = 0; j <image.cols; j++)
		{
			Vec3b e = image.at<Vec3b>(i, j);
			// Note : OpenCV images are stored in BGR order ! 
			imageB.at<Vec3b>(i, j) = e[0];
			imageG.at<Vec3b>(i, j) = Vec3b(0, e[1], 0);
			imageR.at<Vec3b>(i, j) = Vec3b(0, 0, e[2]);
		}
	}
	imshow("Red channel", imageR);
	imshow("Green channel", imageG);
	imshow("Blue channel", imageB);
	waitKey(10000);
	return 0;
}

