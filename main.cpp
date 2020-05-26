#define main_1
#ifdef main_1

#include "main.h"

using namespace std;
using namespace cv;


#define W (63 * CV_PI / 180)
#define WITH_STORAGE_1
//#define IMG_SHOW_0
//#define IMG_SHOW_1
//#define IMG_SHOW_2
//#define IMG_SHOW_3
#define PROCESS_1
#define PROCESS_2
#define PROCESS_3
//#define PROCESS_4
#define PROCESS_5


int main(int argc, char** argv)
{
	// Put images in vectors
	Mat im_rear(1024, 1024, CV_8UC1), im_left(1024, 1024, CV_8UC1), im_right(1024, 1024, CV_8UC1);
	Mat im_rear_eq(1024, 1024, CV_8UC4), im_left_eq(1024, 1024, CV_8UC4), im_right_eq(1024, 1024, CV_8UC4);
	Mat im_left_remap, im_right_remap, im_rear_remap, im_rear_eq_temp;
	Mat verdict = Mat(1400, 1500, CV_8UC3);

	string folder_rear("imgs/datasets/dataset 3/rear/*.png");
	string folder_left("imgs/datasets/dataset 3/left/*.png");
	string folder_right("imgs/datasets/dataset 3/right/*.png");

	vector<String> im_names_rear_tmp;
	vector<String> im_names_left_tmp;
	vector<String> im_names_right_tmp;

#ifdef WITH_STORAGE_1
	vector<Mat> im_names_rear;
	vector<Mat> im_names_left;
	vector<Mat> im_names_right;
	im_names_rear.reserve(200);
	im_names_left.reserve(200);
	im_names_right.reserve(200);
#endif

	glob(folder_rear, im_names_rear_tmp, false);
	glob(folder_left, im_names_left_tmp, false);
	glob(folder_right, im_names_right_tmp, false);

#ifdef WITH_STORAGE_1
	for (int i = 0; ((i != im_names_rear_tmp.size()) && (i != im_names_right_tmp.size()) && (i != im_names_left_tmp.size())); ++i)
	{
		im_names_rear.push_back(imread(im_names_rear_tmp[i], IMREAD_GRAYSCALE));
		im_names_left.push_back(imread(im_names_left_tmp[i], IMREAD_GRAYSCALE));
		im_names_right.push_back(imread(im_names_right_tmp[i], IMREAD_GRAYSCALE));
	}
#endif

	// Create and initialize variables for camera intrinsics
	Mat K_le(3, 3, CV_64F), K_rh(3, 3, CV_64F), K_re(3, 3, CV_64F);
	Mat d_le(5, 1, CV_64F), d_rh(5, 1, CV_64F), d_re(5, 1, CV_64F);
	init(K_le, d_le, K_rh, d_rh, K_re, d_re);


	// Precalculate data for undistortion
	Mat mapx_re, mapy_re, mapx_rh, mapy_rh, mapx_le, mapy_le;
	//initUndistortRectifyMap(K_re, d_re, Mat::eye(3, 3, CV_32FC1), K_re, Size(1024, 1024), CV_32FC1, mapx_re, mapy_re);
	initUndistortRectifyMap(K_rh, d_rh, Mat::eye(3, 3, CV_32FC1), K_rh, Size(1024, 1024), CV_32FC1, mapx_rh, mapy_rh);
	initUndistortRectifyMap(K_le, d_le, Mat::eye(3, 3, CV_32FC1), K_le, Size(1024, 1024), CV_32FC1, mapx_le, mapy_le);

	Mat map_x, map_y;
	undistortFishEyeData(im_rear, W, map_x, map_y);

	Mat car_mask, img_car;
	car_init(img_car, car_mask);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; ((i != im_names_rear.size()) && (i != im_names_right.size()) && (i != im_names_left.size())); ++i)
	{
		cout << "IMAGE NUM: " << i << endl;
		
#ifdef WITH_STORAGE_1
		im_rear = im_names_rear[i];
		im_left = im_names_left[i];
		im_right = im_names_right[i];
#else
		im_rear = imread(im_names_rear1[i], IMREAD_GRAYSCALE);
		im_left = imread(im_names_left1[i], IMREAD_GRAYSCALE);
		im_right = imread(im_names_right1[i], IMREAD_GRAYSCALE);
#endif

		
#ifdef IMG_SHOW_0
		imshow("Image distorted rear", im_rear);
		imshow("Image distorted left", im_left);
		imshow("Image distorted right", im_right);
#endif


		// Convert colors from Bayer to BGRA
		auto time_tag_1 = getTickCount();
#ifdef PROCESS_1
		cvtColor(im_rear, im_rear, COLOR_BayerBG2BGRA);
		cvtColor(im_left, im_left, COLOR_BayerBG2BGRA);
		cvtColor(im_right, im_right, COLOR_BayerBG2BGRA);
#endif
		auto time_tag_2 = getTickCount();
		cout << "cvtColor TIME: " << to_string((time_tag_2 - time_tag_1) / getTickFrequency()) << endl;


#ifdef IMG_SHOW_1
		imshow("Image distorted rear", im_rear);
		imshow("Image distorted left", im_left);
		imshow("Image distorted right", im_right);
#endif
		

		// Fisheye undistortion
		auto time_tag_3 = getTickCount();
#ifdef PROCESS_2
		//remap(im_rear, im_rear_eq, mapx_re, mapy_re, INTER_LINEAR, BORDER_CONSTANT);
		remap(im_rear, im_rear_eq, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT);
		remap(im_right, im_right_eq, mapx_rh, mapy_rh, INTER_LINEAR, BORDER_CONSTANT);
		remap(im_left, im_left_eq, mapx_le, mapy_le, INTER_LINEAR, BORDER_CONSTANT);
#endif
		auto time_tag_4 = getTickCount();
		cout << "undistort TIME: " << to_string((time_tag_4 - time_tag_3) / getTickFrequency()) << endl << endl;

		
#ifdef IMG_SHOW_2
		imshow("Image undistorted rear", im_rear_eq);
		imshow("Image undistorted left", im_left_eq);
		imshow("Image undistorted right", im_right_eq);
#endif
		

#ifdef PROCESS_4
		cvtColor(im_rear_eq, im_rear_eq_temp, COLOR_BGR2Lab);
	    color_corr(im_left_eq, im_rear_eq_temp);
		color_corr(im_right_eq, im_rear_eq_temp);
#endif


		// Top view remapping
#ifdef PROCESS_3
		im_rear_remap = image_remap_auto(im_rear_eq, img_type::REAR);
		im_right_remap = image_remap_auto(im_right_eq, img_type::RIGHT);
		im_left_remap = image_remap_auto(im_left_eq, img_type::LEFT);
#endif


#ifdef IMG_SHOW_3
		imshow("Image remapped rear", im_rear_remap);
		imshow("Image remapped left", im_left_remap);
		imshow("Image remapped right", im_right_remap);
#endif
		

		// Merge 3 images in 1
#ifdef PROCESS_5
		combine(im_rear_remap, im_left_remap, im_right_remap, img_car, car_mask, verdict);
		namedWindow("Combined image", WINDOW_NORMAL);
		resizeWindow("Combined image", int(1500 * 0.8), int(1400 * 0.8));
		imshow("Combined image", verdict);
		//waitKey(0);
		//imwrite("example.png", verdict);
#endif
		
		int key = waitKey(1);
		if (key == 27) break;
	}
}

#endif



