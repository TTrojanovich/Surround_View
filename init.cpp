
#include "main.h"

using namespace cv;


void init(Mat &K_le, Mat& d_le, Mat& K_rh, Mat& d_rh, Mat& K_re, Mat& d_re)
{


	double K_t_le[3][3] = { {412.1236, 0, 485.1900}, {0, 411.7222, 506.7645}, {0, 0, 1} };
	std::memcpy(K_le.data, K_t_le, 3 * 3 * sizeof(double));
	double d_t_le[5] = { -0.3108, 0.0903, 8.6202e-05, 0.0026, -0.0110 };
	std::memcpy(d_le.data, d_t_le, 5 * 1 * sizeof(double));


	double K_t_rh[3][3] = { {418.0526, 0, 497.4132}, {0, 418.5237, 493.0046}, {0, 0, 1} };
	std::memcpy(K_rh.data, K_t_rh, 3 * 3 * sizeof(double));
	double d_t_rh[5] = { -0.2844, 0.0678, -6.3529e-04, 5.1284e-04, -0.0066 };
	std::memcpy(d_rh.data, d_t_rh, 5 * 1 * sizeof(double));


	double K_t_re[3][3] = { {444.0274, 0, 545.5978}, {0, 445.6961, 462.2449}, {0, 0, 1} };
	std::memcpy(K_re.data, K_t_re, 3 * 3 * sizeof(double));
	double d_t_re[5] = { -0.2727, 0.0528, 0.0052, -0.0038, -0.0036 };
	std::memcpy(d_re.data, d_t_re, 5 * 1 * sizeof(double));


}