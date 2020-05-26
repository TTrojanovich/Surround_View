
#include "main.h"

using namespace cv;


void car_init(Mat &img_car, Mat &car_mask)
{
	const double scalar = 1.15;
	img_car = imread("imgs/datasets/car_model.png", IMREAD_UNCHANGED);
	resize(img_car, img_car, Size(int(img_car.size().width * scalar), int(img_car.size().height * scalar)));

	Mat RM = getRotationMatrix2D(Point2i(img_car.size().width / 2, img_car.size().height / 2), 90, 1.0);
	Rect2f bbox = RotatedRect(Point2f(), img_car.size(), (float)90).boundingRect2f();
	RM.at<double>(0, 2) += bbox.width / 2.0 - img_car.cols / 2.0;
	RM.at<double>(1, 2) += bbox.height / 2.0 - img_car.rows / 2.0;
	warpAffine(img_car, img_car, RM, bbox.size());

	vector<Mat> layers;
	split(img_car, layers);
	Mat bgr[3] = { layers[0], layers[1], layers[2] };
	car_mask = layers[3];
	merge(bgr, 3, img_car);
}