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


void init(Mat& K_le, Mat& d_le, Mat& K_rh, Mat& d_rh, Mat& K_re, Mat& d_re);

Mat combine(Mat img_re, Mat img_le, Mat img_ri);

Mat image_remap_auto(Mat im_src, int q);

void image_stats(Mat img, double& lMean, double& lStd, double& aMean, double& aStd, double& bMean, double& bStd);

Mat color_corr(Mat src, Mat dst);

void undistortFishEyeData(const Mat& distorted, const double w, Mat& map_x, Mat& map_y);

Mat undistortFishEye(const Mat& distorted, const double w, Mat map_x, Mat map_y);
