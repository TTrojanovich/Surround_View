
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


void color_corr(Mat &src, Mat &ref)
{
	//src = imread("imgs/61.png");
	//ref = imread("imgs/62.png");

	//imshow("original", src);
	//imshow("reference", ref);
	
	cvtColor(src, src, COLOR_BGR2Lab);

	double lMeanSrc, lStdSrc, aMeanSrc, aStdSrc, bMeanSrc, bStdSrc, lMeanRef, lStdRef, aMeanRef, aStdRef, bMeanRef, bStdRef;
	image_stats(src, lMeanSrc, lStdSrc, aMeanSrc, aStdSrc, bMeanSrc, bStdSrc);
	image_stats(ref, lMeanRef, lStdRef, aMeanRef, aStdRef, bMeanRef, bStdRef);

	vector<Mat> layers;
	split(src, layers);
	Mat l = layers[0];
	Mat a = layers[1];
	Mat b = layers[2];
	
	
	l = (lStdRef / lStdSrc) * (l - lMeanSrc) + lMeanRef;
	a = (aStdRef / aStdSrc) * (a - aMeanSrc) + aMeanRef;
	b = (bStdRef / bStdSrc) * (b - bMeanSrc) + bMeanRef;


	Mat lab[3] = { l, a, b };
	merge(lab, 3, src);
	cvtColor(src, src, COLOR_Lab2BGR);
	cvtColor(src, src, COLOR_BGR2BGRA);
	
	//imshow("res", adjustment);
	//waitKey(0);
}

