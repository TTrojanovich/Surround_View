
#include "main.h"

using namespace std;
using namespace cv;



Mat image_remap_auto(Mat im_src, int q)
{
	vector<Point2f> pts_dst, pts_src;

	// lu = left up , ru = right up , etc...
	Point2f pts_lu, pts_ru, pts_rb, pts_lb;
	int w = 0, h = 0, displace_l = 0, displace_r = 0;

	// Set points in region of interest

	if (q == 2) // rear
	{
		pts_lu = Point2f(0, 620);
		pts_ru = Point2f(1024, 620);
		pts_rb = Point2f(1024, 1000);
		pts_lb = Point2f(0, 1000);

		w = 900; h = 400;
		displace_l = 300; displace_r = 300;
	}
	else if (q == 1) // right
	{
		pts_lu = Point2f(200, 700);
		pts_ru = Point2f(1023, 650);
		pts_rb = Point2f(1023, 1023);
		pts_lb = Point2f(0, 880);

		w = 900; h = 300;
		displace_l = 0; displace_r = 200;
	}
	else if (q == 0) // left
	{
		pts_lu = Point2f(0, 650);
		pts_ru = Point2f(780, 700);
		pts_rb = Point2f(1023, 830);
		pts_lb = Point2f(0, 1023);

		w = 900; h = 300;
		displace_l = 200; displace_r = 0;
	}

	Size size(w, h);
	Mat im_dst(size, CV_8UC4, Scalar(0, 0, 0, 0));

	pts_src.push_back(pts_lu);
	pts_src.push_back(pts_ru);
	pts_src.push_back(pts_rb);
	pts_src.push_back(pts_lb);

	if (q == 2) // destination points for rear view
	{
		pts_dst.push_back(Point2f(0.0f, 200.0f));
		pts_dst.push_back(Point2f(size.width - 1.0f, 200.0f));
		pts_dst.push_back(Point2f(float(size.width - displace_r), float(size.height - 1)));
		pts_dst.push_back(Point2f(float(displace_l), float(size.height - 1)));
	}
	else // destination points for left and right view
	{
		pts_dst.push_back(Point2f(0.0f, 0.0f));
		pts_dst.push_back(Point2f(size.width - 1.0f, 0.0f));
		pts_dst.push_back(Point2f(float(size.width - displace_r), float(size.height - 1)));
		pts_dst.push_back(Point2f(float(displace_l), float(size.height - 1)));
	}
	

	Mat homogr = findHomography(pts_src, pts_dst);
	warpPerspective(im_src, im_dst, homogr, size);

	return im_dst;
}




