#include "main.h"

SurroundView SurroundViewMng;


SurroundView::SurroundView()
{
	camIntrinsics.camMatrLeft = Mat(3, 3, CV_32F);
	camIntrinsics.camMatrRight = Mat(3, 3, CV_32F);
	camIntrinsics.camMatrRear = Mat(3, 3, CV_32F);

	camIntrinsics.distCoeffsLeft = Mat(5, 1, CV_32F);
	camIntrinsics.distCoeffsRight = Mat(5, 1, CV_32F);
	camIntrinsics.distCoeffsRear = Mat(5, 1, CV_32F);

}


void SurroundView::UndistortFisheyeData(const Mat& imagesDistorted, const float angle, Mat& map_x, Mat& map_y)
{
	map_x.create(imagesDistorted.size(), CV_32FC1);
	map_y.create(imagesDistorted.size(), CV_32FC1);

	float cX = imagesDistorted.cols * 0.5f;
	float cY = imagesDistorted.rows * 0.5f;

	for (float x = -1.0f; x < 1.0f; x += 1.0f / cX) {
		for (float y = -1.0f; y < 1.0f; y += 1.0f / cY) {
			float ru = sqrt(x * x + y * y);
			float rd = (1.0f / angle) * atan(2.0f * ru * tan(angle * 0.5f));

			map_x.at<float>(int(y * cY + cY), int(x * cX + cX)) = float(rd / ru * x * cX + cX);
			map_y.at<float>(int(y * cY + cY), int(x * cX + cX)) = float(rd / ru * y * cY + cY);
		}
	}
}


void SurroundView::InitCameraIntrinsics()
{
	float dataMatrLeft[3][3] = { {412.1236f, 0.f, 485.19f}, {0.f, 411.7222f, 506.7645f}, {0.f, 0.f, 1.f} };
	memcpy(camIntrinsics.camMatrLeft.data, dataMatrLeft, 9 * sizeof(float));
	float dataCoeffsLeft[] = { -0.3108f, 0.0903f, 8.6202e-05f, 0.0026f, -0.011f };
	memcpy(camIntrinsics.distCoeffsLeft.data, dataCoeffsLeft, 5 * sizeof(float));

	float dataMatrRight[3][3] = { {418.0526f, 0.f, 497.4132f}, {0.f, 418.5237f, 493.0046f}, {0.f, 0.f, 1.f} };
	memcpy(camIntrinsics.camMatrRight.data, dataMatrRight, 9 * sizeof(float));
	float dataCoeffsRight[] = { -0.2844f, 0.0678f, -6.3529e-04f, 5.1284e-04f, -0.0066f };
	memcpy(camIntrinsics.distCoeffsRight.data, dataCoeffsRight, 5 * sizeof(float));

	float dataMatrRear[3][3] = { {444.0274f, 0.f, 545.5978f}, {0.f, 445.6961f, 462.2449f}, {0.f, 0.f, 1.f} };
	memcpy(camIntrinsics.camMatrRear.data, dataMatrRear, 9 * sizeof(float));
	float dataCoeffsRear[] = { -0.2727f, 0.0528f, 0.0052f, -0.0038f, -0.0036f };
	memcpy(camIntrinsics.distCoeffsRear.data, dataCoeffsRear, 5 * sizeof(float));
}


void SurroundView::InitCarModel()
{
	const float sizeScale = 1.15f;
	carImage = imread("./imgs/datasets/car_model.png", IMREAD_UNCHANGED);
	resize(carImage, carImage, Size(int(carImage.size().width * sizeScale), int(carImage.size().height * sizeScale)));

	const double rotationAngle = 90;
	Mat rotMatr = getRotationMatrix2D(Point2f(carImage.size().width * 0.5f, carImage.size().height * 0.5f), rotationAngle, 1.0);

	Rect2f bbox = RotatedRect(Point2f(), carImage.size(), (float)rotationAngle).boundingRect2f();
	rotMatr.at<double>(0, 2) += bbox.width * 0.5f - carImage.cols * 0.5f;
	rotMatr.at<double>(1, 2) += bbox.height * 0.5f - carImage.rows * 0.5f;
	warpAffine(carImage, carImage, rotMatr, bbox.size());

	Mat layers[4];
	split(carImage, layers);
	carMask = layers[3];
	merge(layers, 3, carImage);
}


void SurroundView::CalcUndistortionData()
{
	InputArray R = Mat::eye(3, 3, CV_32FC1);
	Size size(1024, 1024);
	initUndistortRectifyMap(camIntrinsics.camMatrLeft, camIntrinsics.distCoeffsLeft, R, camIntrinsics.camMatrLeft, size, CV_32FC1, undistortionData[0][0], undistortionData[0][1]);
	initUndistortRectifyMap(camIntrinsics.camMatrRight, camIntrinsics.distCoeffsRight, R, camIntrinsics.camMatrRight, size, CV_32FC1, undistortionData[1][0], undistortionData[1][1]);
	initUndistortRectifyMap(camIntrinsics.camMatrRear, camIntrinsics.distCoeffsRear, R, camIntrinsics.camMatrRear, size, CV_32FC1, undistortionData[2][0], undistortionData[2][1]);
}


void SurroundView::ColorCorrectionCalcStats(Mat img, double& lMean, double& lStd, double& aMean, double& aStd, double& bMean, double& bStd)
{
	Mat i_mean, i_stddev;
	meanStdDev(img, i_mean, i_stddev);

	lMean = i_mean.at<double>(0);
	lStd = i_stddev.at<double>(0);
	aMean = i_mean.at<double>(1);
	aStd = i_stddev.at<double>(1);
	bMean = i_mean.at<double>(2);
	bStd = i_stddev.at<double>(2);
}


void SurroundView::ColorCorrection(Mat& src, Mat& ref)
{
	cvtColor(src, src, COLOR_BGR2Lab);

	double lMeanSrc, lStdSrc, aMeanSrc, aStdSrc, bMeanSrc, bStdSrc, lMeanRef, lStdRef, aMeanRef, aStdRef, bMeanRef, bStdRef;
	ColorCorrectionCalcStats(src, lMeanSrc, lStdSrc, aMeanSrc, aStdSrc, bMeanSrc, bStdSrc);
	ColorCorrectionCalcStats(ref, lMeanRef, lStdRef, aMeanRef, aStdRef, bMeanRef, bStdRef);

	Mat lab[3];
	split(src, lab);
	
	lab[0] = (lStdRef / lStdSrc) * (lab[0] - lMeanSrc) + lMeanRef;
	lab[1] = (aStdRef / aStdSrc) * (lab[1] - aMeanSrc) + aMeanRef;
	lab[2] = (bStdRef / bStdSrc) * (lab[2] - bMeanSrc) + bMeanRef;

	merge(lab, 3, src);
	cvtColor(src, src, COLOR_Lab2BGR);
	cvtColor(src, src, COLOR_BGR2BGRA);
}


void SurroundView::PlaceImage(Mat& img, const int& pos_height, const int& pos_width, Mat& dst)
{
	Mat layers[4];
	split(img, layers);
	merge(layers, 3, img);
	img.copyTo(dst.rowRange(pos_height, pos_height + img.rows).colRange(pos_width, pos_width + img.cols), layers[3]);
}


void SurroundView::CombineFinalImage(Mat& img_re, Mat& img_le, Mat& img_ri, Mat& img_car, Mat& car_mask, Mat& dst)
{
	Point2f center(img_ri.size().width * 0.5f, img_ri.size().height * 0.5f);

	// Rotate left and right images 90 degrees
	Mat rotMatr1 = getRotationMatrix2D(center, 90, 1.0);
	Rect2f bbox1 = RotatedRect(Point2f(), img_ri.size(), (float)90).boundingRect2f();

	Mat rotMatr2 = getRotationMatrix2D(center, -90, 1.0);
	Rect2f bbox2 = RotatedRect(Point2f(), img_ri.size(), (float)(-90)).boundingRect2f();

	rotMatr1.at<double>(0, 2) += bbox1.width * 0.5f - img_ri.cols * 0.5f;
	rotMatr1.at<double>(1, 2) += bbox1.height * 0.5f - img_ri.rows * 0.5f;

	rotMatr2.at<double>(0, 2) += bbox2.width * 0.5f - img_ri.cols * 0.5f;
	rotMatr2.at<double>(1, 2) += bbox2.height * 0.5f - img_ri.rows * 0.5f;

	warpAffine(img_ri, img_ri, rotMatr1, bbox1.size());
	warpAffine(img_le, img_le, rotMatr2, bbox2.size());

	const int pos_height = 30;
	const int pos_width_right = 330;
	const int pos_width_left = 980;
	const int car_offset_height = 15;
	const int car_offset_width = 80;
	const int pos_height_car = pos_height + img_re.rows - car_offset_height;
	const int pos_width_car = pos_width_right + img_re.cols / 3 - car_offset_width;

	PlaceImage(img_ri, pos_height, pos_width_right, dst);
	PlaceImage(img_le, pos_height, pos_width_left, dst);
	PlaceImage(img_re, pos_height, pos_width_right, dst);

	img_car.copyTo(dst.rowRange(pos_height_car, pos_height_car + img_car.rows).colRange(pos_width_car, pos_width_car + img_car.cols), car_mask);
}


Mat SurroundView::RemapImage(Mat& im_src, IMAGE_TYPE type)
{
	vector<Point2f> pts_dst, pts_src;

	// lu = left up , ru = right up , etc...
	Point2f pts_lu, pts_ru, pts_rb, pts_lb;
	int w = 0, h = 0, displace_l = 0, displace_r = 0;

	// Set points in region of interest

	if (type == IMAGE_TYPE::REAR) {
		pts_lu = Point2f(0, 620);
		pts_ru = Point2f(1023, 620);
		pts_rb = Point2f(1023, 870);
		pts_lb = Point2f(0, 870);

		w = 900; h = 300;
		displace_l = 250; displace_r = 250;
	}
	else if (type == IMAGE_TYPE::RIGHT)	{
		pts_lu = Point2f(200, 620);
		pts_ru = Point2f(1023, 520);
		pts_rb = Point2f(1023, 930);
		pts_lb = Point2f(0, 800);

		w = 1200; h = 250;
		displace_l = 300; displace_r = 300;
	}
	else if (type == IMAGE_TYPE::LEFT) {
		pts_lu = Point2f(0, 530);
		pts_ru = Point2f(850, 640);
		pts_rb = Point2f(1023, 790);
		pts_lb = Point2f(0, 880);

		w = 1200; h = 250;
		displace_l = 300; displace_r = 300;
	}

	Size size(w, h);
	Mat im_dst(size, CV_8UC4, Scalar(0, 0, 0, 0));
	Mat homogr;

	pts_src.push_back(pts_lu);
	pts_src.push_back(pts_ru);
	pts_src.push_back(pts_rb);
	pts_src.push_back(pts_lb);

	// destination points for rear, left, right view
	pts_dst.push_back(Point2f(0.0f, 0.0f));
	pts_dst.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_dst.push_back(Point2f(float(size.width - 1), float(size.height - 1)));
	pts_dst.push_back(Point2f(0.0f, float(size.height - 1)));

	homogr = findHomography(pts_src, pts_dst);
	warpPerspective(im_src, im_src, homogr, size);

	pts_src.clear();
	pts_dst.clear();

	pts_src.push_back(Point2f(0.0f, 0.0f));
	pts_src.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_src.push_back(Point2f(float(size.width - 1), float(size.height - 1)));
	pts_src.push_back(Point2f(0.0f, float(size.height - 1)));

	pts_dst.push_back(Point2f(0.0f, 0.0f));
	pts_dst.push_back(Point2f(size.width - 1.0f, 0.0f));
	pts_dst.push_back(Point2f(float(size.width - displace_r), float(size.height - 1)));
	pts_dst.push_back(Point2f(float(displace_l), float(size.height - 1)));

	homogr = findHomography(pts_src, pts_dst);
	warpPerspective(im_src, im_dst, homogr, size);

	return im_dst;
}


void SurroundView::InitStorage()
{
	string folder_rear("./imgs/datasets/dataset 3/rear/*.png");
	string folder_left("./imgs/datasets/dataset 3/left/*.png");
	string folder_right("./imgs/datasets/dataset 3/right/*.png");

	vector<String> im_names_rear_tmp;
	vector<String> im_names_left_tmp;
	vector<String> im_names_right_tmp;

	im_names_rear.reserve(IMAGES_COUNT);
	im_names_left.reserve(IMAGES_COUNT);
	im_names_right.reserve(IMAGES_COUNT);

	glob(folder_rear, im_names_rear_tmp, false);
	glob(folder_left, im_names_left_tmp, false);
	glob(folder_right, im_names_right_tmp, false);

	for (int i = 0; ((i != im_names_rear_tmp.size()) && (i != im_names_right_tmp.size()) && (i != im_names_left_tmp.size())); ++i)
	{
		im_names_rear.emplace_back(imread(im_names_rear_tmp[i], IMREAD_GRAYSCALE));
		im_names_left.emplace_back(imread(im_names_left_tmp[i], IMREAD_GRAYSCALE));
		im_names_right.emplace_back(imread(im_names_right_tmp[i], IMREAD_GRAYSCALE));
	}
}


void SurroundView::Init()
{
	InitCameraIntrinsics();
	InitCarModel();
	InitStorage();
	CalcUndistortionData();
}


void SurroundView::Run()
{
	int type = CV_8UC1;
	Mat im_rear(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);
	Mat im_left(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);
	Mat im_right(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);

	type = CV_8UC4;
	Mat im_rear_eq(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);
	Mat im_left_eq(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);
	Mat im_right_eq(DATASET_IMAGE_SIZE, DATASET_IMAGE_SIZE, type);

	Mat im_left_remap, im_right_remap, im_rear_remap, im_rear_eq_temp;

	Mat finalImage(FINAL_IMAGE_SIZE.first, FINAL_IMAGE_SIZE.second, CV_8UC3);

	for (int i = 0; i < IMAGES_COUNT; ++i)
	{
		im_rear = im_names_rear[i];
		im_left = im_names_left[i];
		im_right = im_names_right[i];

		// Convert colors from Bayer to BGRA
		cvtColor(im_rear, im_rear, COLOR_BayerBG2BGRA);
		cvtColor(im_left, im_left, COLOR_BayerBG2BGRA);
		cvtColor(im_right, im_right, COLOR_BayerBG2BGRA);

		// Fisheye undistortion
		remap(im_rear, im_rear_eq, undistortionData[2][0], undistortionData[2][1], INTER_LINEAR, BORDER_CONSTANT);
		remap(im_right, im_right_eq, undistortionData[1][0], undistortionData[1][1], INTER_LINEAR, BORDER_CONSTANT);
		remap(im_left, im_left_eq, undistortionData[0][0], undistortionData[0][1], INTER_LINEAR, BORDER_CONSTANT);

		// Color correction. Based on rear image color
		cvtColor(im_rear_eq, im_rear_eq_temp, COLOR_BGR2Lab);
		SurroundViewMng.ColorCorrection(im_left_eq, im_rear_eq_temp);
		SurroundViewMng.ColorCorrection(im_right_eq, im_rear_eq_temp);

		// Top view remapping
		im_rear_remap = SurroundViewMng.RemapImage(im_rear_eq, SurroundView::IMAGE_TYPE::REAR);
		im_right_remap = SurroundViewMng.RemapImage(im_right_eq, SurroundView::IMAGE_TYPE::RIGHT);
		im_left_remap = SurroundViewMng.RemapImage(im_left_eq, SurroundView::IMAGE_TYPE::LEFT);

		// Merge 3 images in 1
		SurroundViewMng.CombineFinalImage(im_rear_remap, im_left_remap, im_right_remap, carImage, carMask, finalImage);
		namedWindow("Combined image", WINDOW_NORMAL);
		const float scale = 0.8f;
		resizeWindow("Combined image", int(FINAL_IMAGE_SIZE.second * scale), int(FINAL_IMAGE_SIZE.first * scale));
		imshow("Combined image", finalImage);

		if (waitKey(1) == 27) break;
	}
}


int main()
{
	SurroundViewMng.Init();
	SurroundViewMng.Run();
}



