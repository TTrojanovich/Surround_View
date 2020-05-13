
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
		pts_lu = Point2f(0, 620); // 0 620
		pts_ru = Point2f(1023, 620); // 1023 620
		pts_rb = Point2f(1023, 870); // 1023 870
		pts_lb = Point2f(0, 870); // 0 870

		w = 900; h = 300;
		displace_l = 250; displace_r = 250;
	}
	else if (q == 1) // right
	{
		pts_lu = Point2f(200, 620); // 200 620
		pts_ru = Point2f(1023, 520); // 1023 520
		pts_rb = Point2f(1023, 930); // 1023 930
		pts_lb = Point2f(0, 800); // 0 800

		w = 1200; h = 250;
		displace_l = 300; displace_r = 300;
	}
	else if (q == 0) // left
	{
		pts_lu = Point2f(0, 530); // 0 530
		pts_ru = Point2f(850, 640); // 850 640
		pts_rb = Point2f(1023, 790); // 1023 790
		pts_lb = Point2f(0, 880); // 0 880

		w = 1200; h = 250;
		displace_l = 300; displace_r = 300;
	}

	Size size(w, h);
	Mat im_dst(size, CV_8UC4, Scalar(0, 0, 0, 0));
	Mat homogr;

	pts_src.push_back(pts_lu);
	pts_src.push_back(pts_ru);
	pts_src.push_back(pts_rb);
	pts_src.push_back(pts_lb);

	// destination points for rear, left, right view
	pts_dst.push_back(Point2f(0.0f, 0.0f));
	pts_dst.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_dst.push_back(Point2f(float(size.width - 1), float(size.height - 1)));
	pts_dst.push_back(Point2f(0.0f, float(size.height - 1)));

	homogr = findHomography(pts_src, pts_dst);
	warpPerspective(im_src, im_src, homogr, size);

	pts_src.clear();
	pts_dst.clear();

	pts_src.push_back(Point2f(0.0f, 0.0f));
	pts_src.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_src.push_back(Point2f(float(size.width - 1), float(size.height - 1)));
	pts_src.push_back(Point2f(0.0f, float(size.height - 1)));
	
	pts_dst.push_back(Point2f(0.0f, 0.0f));
	pts_dst.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_dst.push_back(Point2f(float(size.width - displace_r), float(size.height - 1)));
	pts_dst.push_back(Point2f(float(displace_l), float(size.height - 1)));
	
	homogr = findHomography(pts_src, pts_dst);
	warpPerspective(im_src, im_dst, homogr, size);

	return im_dst;
}




