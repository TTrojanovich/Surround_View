
#include "main.h"

using namespace std;
using namespace cv;



Mat combine(Mat img_re, Mat img_le, Mat img_ri)
{
	Size i_size = img_ri.size();
	int w = i_size.width, h = i_size.height;
	
	// Rotate left and right images 90 degrees

	Point2i center(w / 2, h / 2);

	Mat RM1 = getRotationMatrix2D(center, 90, 1.0);
	Rect2f bbox1 = RotatedRect(Point2f(), i_size, (float)90).boundingRect2f();

	Mat RM2 = getRotationMatrix2D(center, -90, 1.0);
	Rect2f bbox2 = RotatedRect(Point2f(), i_size, (float)(-90)).boundingRect2f();

	RM1.at<double>(0, 2) += bbox1.width / 2.0 - img_ri.cols / 2.0;
	RM1.at<double>(1, 2) += bbox1.height / 2.0 - img_ri.rows / 2.0;

	RM2.at<double>(0, 2) += bbox2.width / 2.0 - img_ri.cols / 2.0;
	RM2.at<double>(1, 2) += bbox2.height / 2.0 - img_ri.rows / 2.0;

	warpAffine(img_ri, img_ri, RM1, bbox1.size());
	warpAffine(img_le, img_le, RM2, bbox2.size());


	// Create destination image

	Mat dst = Mat(1200, 1500, CV_8UC3);

	//resize(img_ri, img_ri, Size(200, 600));
	//resize(img_le, img_le, Size(200, 600));
	//resize(img_re, img_re, Size(667, 267));

	
	Mat mask;
	vector<Mat> layers;
	int he = 230;
	int wi1 = 330;
	int wi2 = 930;

	// Split 4 channels, merge BGR channels and setting the alpha channel as a mask

	split(img_ri, layers);
	Mat rgb1[3] = { layers[0],layers[1],layers[2] };
	mask = layers[3];
	merge(rgb1, 3, img_ri);
	img_ri.copyTo(dst.rowRange(he, he + img_ri.rows).colRange(wi1, wi1 + img_ri.cols), mask);

	split(img_le, layers);
	Mat rgb2[3] = { layers[0],layers[1],layers[2] };
	mask = layers[3];
	merge(rgb2, 3, img_le);
	img_le.copyTo(dst.rowRange(he, he + img_le.rows).colRange(wi2, wi2 + img_le.cols), mask);

	split(img_re, layers);
	Mat rgb3[3] = { layers[0],layers[1],layers[2] };
	mask = layers[3];
	merge(rgb3, 3, img_re);
	img_re.copyTo(dst.rowRange(30, 30 + img_re.rows).colRange(330, 330 + img_re.cols), mask);
	
	return dst;
}




