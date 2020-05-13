
#include "main.h"

using namespace cv;


void undistortFishEyeData(const Mat& distorted, const double w, Mat &map_x, Mat &map_y)
{
	map_x.create(distorted.size(), CV_32FC1);
	map_y.create(distorted.size(), CV_32FC1);

	double Cx = distorted.cols / 2.0;
	double Cy = distorted.rows / 2.0;


	for (double x = -1.0; x < 1.0; x += 1.0 / Cx)
	{
		for (double y = -1.0; y < 1.0; y += 1.0 / Cy)
		{
			double ru = sqrt(x * x + y * y);
			double rd = (1.0 / w) * atan(2.0 * ru * tan(w / 2.0));

			map_x.at<float>(int(y * Cy + Cy), int(x * Cx + Cx)) = float(rd / ru * x * Cx + Cx);
			map_y.at<float>(int(y * Cy + Cy), int(x * Cx + Cx)) = float(rd / ru * y * Cy + Cy);
		}
	}
}



Mat undistortFishEye(const Mat& distorted, const double w, Mat map_x, Mat map_y)
{
	Mat undistorted;
	remap(distorted, undistorted, map_x, map_y, INTER_LINEAR);

	return undistorted;
}

