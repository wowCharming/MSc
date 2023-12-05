#include<opencv2\opencv.hpp>

#include <vector>
#include <stack>
#include <iostream>
#include <cmath>

using namespace cv;

// Distance used in Mean Shift
inline int spatial_distance(const CvPoint& q, const CvPoint& p)
{
	int a = q.x - p.x, b = q.y - p.y;
	return a*a + b*b;
}
inline int getLabel(std::vector<int>& unionfind, int l)
{
	int r = unionfind[l];
	if (unionfind[r] == r)
		return r;
	else
	{
		unionfind[l] = getLabel(unionfind, unionfind[r]);
		return unionfind[l];
	}
}
inline int getLabel2(std::vector<int>& unionfind, int l)
{
	int r = unionfind[l];
	if (r<0)
		return r;
	else
	{
		unionfind[l] = getLabel2(unionfind, r);
		return unionfind[l];
	}
}

const int spatial_radius = 10;

int MeanShift(const IplImage* img, int **labels);
