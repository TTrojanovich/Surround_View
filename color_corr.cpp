
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;


void image_stats(Mat img, double &lMean, double &lStd, double &aMean, double &aStd, double &bMean, double &bStd)
{
	vector<Mat> layers;
	split(img, layers);
	Mat l = layers[0];
	Mat a = layers[1];
	Mat b = layers[2];

	Mat i_mean, i_stddev;
	meanStdDev(img, i_mean, i_stddev);

	lMean = i_mean.at<double>(0);
	lStd = i_stddev.at<double>(0);
	aMean = i_mean.at<double>(1);
	aStd = i_stddev.at<double>(1);
	bMean = i_mean.at<double>(2);
	bStd = i_stddev.at<double>(2);
}


Mat color_corr(Mat src, Mat dst)
{
	//Mat src = imread("imgs/61.png");
	//Mat dst = imread("imgs/62.png");
	
	cvtColor(src, src, COLOR_BGR2Lab);
	cvtColor(dst, dst, COLOR_BGR2Lab);

	src.convertTo(src, CV_32F);
	dst.convertTo(dst, CV_32F);
	
	double lMeanSrc=0, lStdSrc = 0, aMeanSrc = 0, aStdSrc = 0, bMeanSrc = 0, bStdSrc = 0, lMeanDst = 0, lStdDst = 0, aMeanDst = 0, aStdDst = 0, bMeanDst = 0, bStdDst = 0;
	image_stats(src, lMeanSrc, lStdSrc, aMeanSrc, aStdSrc, bMeanSrc, bStdSrc);
	image_stats(dst, lMeanDst, lStdDst, aMeanDst, aStdDst, bMeanDst, bStdDst);

	vector<Mat> layers;
	split(dst, layers);
	Mat l = layers[0];
	Mat a = layers[1];
	Mat b = layers[2];
	
	l = l - lMeanDst;
	a = a - aMeanDst;
	b = b - bMeanDst;

	l = (lStdDst / lStdSrc) * l;
	a = (aStdDst / aStdSrc) * a;
	b = (bStdDst / bStdSrc) * b;

	l = l + lMeanSrc;
	a = a + aMeanSrc;
	b = b + bMeanSrc;


	Mat transfer;
	Mat lab[3] = {l, a, b};
	merge(lab, 3, transfer);
	transfer.convertTo(transfer, CV_8U);
	cvtColor(transfer, transfer, COLOR_Lab2BGR);
	cvtColor(transfer, transfer, COLOR_BGR2BGRA);
	
	//imshow("res", transfer);
	//waitKey(0);

	return transfer;
}

