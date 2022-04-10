#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/stitching.hpp"

#include <iterator>
#include <vector>
#include <algorithm>

#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;


class SurroundView
{
public:
	SurroundView();
	void Init();
	void Run();

private:
	const int IMAGES_COUNT = 150;
	const int DATASET_IMAGE_SIZE = 1024;
	const pair<int, int> FINAL_IMAGE_SIZE = make_pair(1400, 1500);
	enum class IMAGE_TYPE { REAR, RIGHT, LEFT };

	struct CAMERA_INTRINSICS
	{
		Mat camMatrLeft;
		Mat camMatrRight;
		Mat camMatrRear;
		Mat distCoeffsLeft;
		Mat distCoeffsRight;
		Mat distCoeffsRear;
	};

	void InitCameraIntrinsics();
	void InitCarModel();
	void InitStorage();
	void CalcUndistortionData();

	void CombineFinalImage(Mat& img_re, Mat& img_le, Mat& img_ri, Mat& img_car, Mat& car_mask, Mat& dst);
	void PlaceImage(Mat& img, const int& pos_height, const int& pos_width, Mat& dst);
	Mat RemapImage(Mat& im_src, IMAGE_TYPE type);
	void ColorCorrectionCalcStats(Mat img, double& lMean, double& lStd, double& aMean, double& aStd, double& bMean, double& bStd);
	void ColorCorrection(Mat& src, Mat& ref);
	void UndistortFisheyeData(const Mat& imagesDistorted, const float angle, Mat& map_x, Mat& map_y);

	Mat undistortionData[3][2]; // [0] - left, [1] - right, [2] - rear
	CAMERA_INTRINSICS camIntrinsics;

	vector<Mat> im_names_rear;
	vector<Mat> im_names_left;
	vector<Mat> im_names_right;

	Mat carImage;
	Mat carMask;
};

extern SurroundView SurroundViewMng;
