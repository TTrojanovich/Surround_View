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


enum class img_type { REAR, RIGHT, LEFT };

void init(Mat& K_le, Mat& d_le, Mat& K_rh, Mat& d_rh, Mat& K_re, Mat& d_re);

void combine(Mat &img_re, Mat &img_le, Mat &img_ri, Mat &img_car, Mat &car_mask, Mat &dst);

Mat image_remap_auto(Mat &im_src, img_type type);

void image_stats(Mat img, double& lMean, double& lStd, double& aMean, double& aStd, double& bMean, double& bStd);

void color_corr(Mat &src, Mat &ref);

void undistortFishEyeData(const Mat& distorted, const double w, Mat& map_x, Mat& map_y);

void car_init(Mat &img_car, Mat &car_mask);
