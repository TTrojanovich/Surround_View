
#include "main.h"

using namespace cv;


void car_init(Mat &img_car, Mat &car_mask)
{
	img_car = imread("imgs/datasets/car_model.png", IMREAD_UNCHANGED);
	resize(img_car, img_car, Size(int(745 * 1.15), int(400 * 1.15)));
	Size car_size = img_car.size();

	Mat RM3 = getRotationMatrix2D(Point2i(car_size.width / 2, car_size.height / 2), 90, 1.0);
	Rect2f bbox3 = RotatedRect(Point2f(), car_size, (float)90).boundingRect2f();
	RM3.at<double>(0, 2) += bbox3.width / 2.0 - img_car.cols / 2.0;
	RM3.at<double>(1, 2) += bbox3.height / 2.0 - img_car.rows / 2.0;
	warpAffine(img_car, img_car, RM3, bbox3.size());

	vector<Mat> layers;
	split(img_car, layers);
	Mat rgb4[3] = { layers[0],layers[1],layers[2] };
	car_mask = layers[3];
	merge(rgb4, 3, img_car);
}