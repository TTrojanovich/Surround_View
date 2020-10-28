
#include "main.h"

using namespace std;
using namespace cv;


void position(Mat &img, const int &pos_height, const int &pos_width, Mat &dst)
{
	Mat mask;
	vector<Mat> layers; 
	layers.reserve(3);
	split(img, layers);
	Mat bgr[3] = { layers[0], layers[1], layers[2] };
	mask = layers[3];
	merge(bgr, 3, img);
	img.copyTo(dst.rowRange(pos_height, pos_height + img.rows).colRange(pos_width, pos_width + img.cols), mask);
}


void combine(Mat &img_re, Mat &img_le, Mat &img_ri, Mat &img_car, Mat &car_mask, Mat &dst)
{
	Point2i center(img_ri.size().width / 2, img_ri.size().height / 2);

	// Rotate left and right images 90 degrees
	Mat RM1 = getRotationMatrix2D(center, 90, 1.0);
	Rect2f bbox1 = RotatedRect(Point2f(), img_ri.size(), (float)90).boundingRect2f();

	Mat RM2 = getRotationMatrix2D(center, -90, 1.0);
	Rect2f bbox2 = RotatedRect(Point2f(), img_ri.size(), (float)(-90)).boundingRect2f();
	
	RM1.at<double>(0, 2) += bbox1.width / 2.0 - img_ri.cols / 2.0;
	RM1.at<double>(1, 2) += bbox1.height / 2.0 - img_ri.rows / 2.0;

	RM2.at<double>(0, 2) += bbox2.width / 2.0 - img_ri.cols / 2.0;
	RM2.at<double>(1, 2) += bbox2.height / 2.0 - img_ri.rows / 2.0;

	warpAffine(img_ri, img_ri, RM1, bbox1.size());
	warpAffine(img_le, img_le, RM2, bbox2.size());

	const int pos_height_side = 30;
	const int pos_width_right = 330;
	const int pos_width_left = 980;
	const int pos_height_rear = 30;
	const int pos_height_car = pos_height_rear - 15 + img_re.rows;
	const int pos_width_car = pos_width_right - 80 + img_re.cols / 3;
	
	position(img_ri, pos_height_side, pos_width_right, dst);
	position(img_le, pos_height_side, pos_width_left, dst);
	position(img_re, pos_height_rear, pos_width_right, dst);
	
	img_car.copyTo(dst.rowRange(pos_height_car, pos_height_car + img_car.rows).colRange(pos_width_car, pos_width_car + img_car.cols), car_mask);

}




