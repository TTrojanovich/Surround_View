#define main_1
#ifdef main_1

#include "main.h"

using namespace std;
using namespace cv;



int main(int argc, char** argv)
{
	// Put images in vectors
	Mat im_rear, im_left, im_right;

	string folder_rear("imgs/dataset/rear/*.png");
	string folder_left("imgs/dataset/left/*.png");
	string folder_right("imgs/dataset/right/*.png");

	vector<cv::String> im_names_rear1;
	vector<cv::String> im_names_left1;
	vector<cv::String> im_names_right1;

	vector<cv::Mat> im_names_rear;
	vector<cv::Mat> im_names_left;
	vector<cv::Mat> im_names_right;

	glob(folder_rear, im_names_rear1, false);
	glob(folder_left, im_names_left1, false);
	glob(folder_right, im_names_right1, false);

	for (size_t k = 0; k < im_names_rear1.size(); k++)
	{
		im_names_rear.push_back(imread(im_names_rear1[k], IMREAD_GRAYSCALE));
		im_names_left.push_back(imread(im_names_left1[k], IMREAD_GRAYSCALE));
		im_names_right.push_back(imread(im_names_right1[k], IMREAD_GRAYSCALE));
	}

	// Create and initialize variables for camera intrinsics
	cv::Mat K_le(3, 3, CV_64F), K_rh(3, 3, CV_64F), K_re(3, 3, CV_64F);
	cv::Mat d_le(5, 1, CV_64F), d_rh(5, 1, CV_64F), d_re(5, 1, CV_64F);
	init(K_le, d_le, K_rh, d_rh, K_re, d_re);


	// Precalculate data for undistortion
	Mat mapx_re, mapy_re, mapx_rh, mapy_rh, mapx_le, mapy_le;
	cv::initUndistortRectifyMap(K_re, d_re, Mat::eye(3, 3, CV_32FC1), K_re, cv::Size(1024, 1024), CV_32FC1, mapx_re, mapy_re);
	cv::initUndistortRectifyMap(K_rh, d_rh, Mat::eye(3, 3, CV_32FC1), K_rh, cv::Size(1024, 1024), CV_32FC1, mapx_rh, mapy_rh);
	cv::initUndistortRectifyMap(K_le, d_le, Mat::eye(3, 3, CV_32FC1), K_le, cv::Size(1024, 1024), CV_32FC1, mapx_le, mapy_le);

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < im_names_rear.size(); i++)
	{
		cout << "IMAGE NUM: " << i << endl;

		im_rear = im_names_rear[i];
		im_left = im_names_left[i];
		im_right = im_names_right[i];

#define show_
#ifdef show_0
		imshow("Image distorted rear", im_rear);
		imshow("Image distorted left", im_left);
		imshow("Image distorted right", im_right);
#endif

		// Convert colors from Bayer to BGRA
		auto aa1 = getTickCount();
#define process_1
#ifdef process_1
		cvtColor(im_rear, im_rear, COLOR_BayerBG2BGRA);
		cvtColor(im_left, im_left, COLOR_BayerBG2BGRA);
		cvtColor(im_right, im_right, COLOR_BayerBG2BGRA);
#endif
		auto aa2 = getTickCount();
		cout << "cvtColor TIME: " << to_string((aa2 - aa1) / getTickFrequency()) << endl;


#ifdef show_1
		imshow("Image distorted rear", im_rear);
		imshow("Image distorted left", im_left);
		imshow("Image distorted right", im_right);
#endif

		// Fisheye undistortion
		auto aa3 = getTickCount();
#define process_2
#ifdef process_2
		Mat im_rear_eq, im_right_eq, im_left_eq;
		remap(im_rear, im_rear_eq, mapx_re, mapy_re, INTER_LINEAR, BORDER_CONSTANT);
		remap(im_right, im_right_eq, mapx_rh, mapy_rh, INTER_LINEAR, BORDER_CONSTANT);
		remap(im_left, im_left_eq, mapx_le, mapy_le, INTER_LINEAR, BORDER_CONSTANT);
#endif
		auto aa4 = getTickCount();
		cout << "undistort TIME: " << to_string((aa4 - aa3) / getTickFrequency()) << endl << endl;



#ifdef show_2
		imshow("Image undistorted rear", im_rear_eq);
		imshow("Image undistorted left", im_left_eq);
		imshow("Image undistorted right", im_right_eq);
#endif

		// Top view remapping
#define process_3
#ifdef process_3
		Mat im_rear_remap = image_remap_auto(im_rear_eq, 2);
		Mat im_right_remap = image_remap_auto(im_right_eq, 1);
		Mat im_left_remap = image_remap_auto(im_left_eq, 0);
#endif

#ifdef show_3
		imshow("Image remapped rear", im_rear_remap);
		imshow("Image remapped left", im_left_remap);
		imshow("Image remapped right", im_right_remap);
#endif

		// Merge 3 images in 1
#define process_4
#ifdef process_4
		Mat temp = combine(im_rear_remap, im_left_remap, im_right_remap);
		imshow("Combined image", temp);
#endif


		int key = waitKey(1);
		if (key == 27) break;
	}
}

#endif



