#include<iostream>
#include<opencv2\opencv.hpp>
//#include<opencv2\core\core.hpp>
using namespace cv;

int main() {

	//Mat a(300, 300, CV_8UC3, Scalar(0, 0, 0));
	//read image "cameraman_noisy"
	Mat img = imread("cameraman_noisy.png", CV_LOAD_IMAGE_GRAYSCALE);//void namedWindow(const string& winname, int flags=WINDOW_AUTOSIZE)
	if (img.empty())
		std::cout << "failed to open img" << std::endl;
	else
		std::cout << "img loaded OK" << std::endl;
		//creat the window
	namedWindow("Display window", WINDOW_AUTOSIZE);//WINDOW_NORMAL
		//show the image
		imshow("Display window", img);//void imshow(const string& winname, InputArray mat)
	
	
	waitKey(0);

	return 0;

}
