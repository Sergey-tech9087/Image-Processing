#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat Im(300, 450, CV_32FC1);

	for (int i = 0; i < 300; i++) {
		if (i < 150) {
			for (int j = 0; j < 300; j++) {
				if (j < 150) {
					Im.at<float>(i, j) = 0;
				}
				else {
					Im.at<float>(i, j) = 127 / 255.0;
					Im.at<float>(j, i) = 127 / 255.0;
				}
			}
		}
		else {
			for (int j = 150; j < 450; j++) {
				if (j >= 150 && j < 300) {
					Im.at<float>(i, j) = 255 / 255.0;
					Im.at<float>(i - 150, j + 150) = 255 / 255.0;
				}
				if (j >= 300) {
					Im.at<float>(i, j) = 0;
				}
			}
		}
	}

	cv::circle(Im, cv::Point(75, 75), 60, 255 / 255.0, -1);
	cv::circle(Im, cv::Point(375, 225), 60, 200 / 255.0, -1);

	cv::circle(Im, cv::Point(225, 75), 60, 180 / 255.0, -1);
	cv::circle(Im, cv::Point(75, 225), 60, 100 / 255.0, -1);

	cv::circle(Im, cv::Point(225, 225), 60, 50 / 255.0, -1);
	cv::circle(Im, cv::Point(375, 75), 60, 0 / 255.0, -1);

	cv::Mat I_1(2, 2, CV_32S);
	I_1.at<int>(0, 0) = 1;
	I_1.at<int>(0, 1) = 0;
	I_1.at<int>(1, 0) = 0;
	I_1.at<int>(1, 1) = -1;

	cv::Mat I_2(2, 2, CV_32S);
	I_2.at<int>(0, 0) = 0;
	I_2.at<int>(0, 1) = 1;
	I_2.at<int>(1, 0) = -1;
	I_2.at<int>(1, 1) = 0;

	cv::Mat I_1_filt = Im.clone();
	I_1_filt = 0;
	cv::filter2D(Im, I_1_filt, -1, I_1, cv::Point(0, 0));

	cv::Mat I_2_filt = Im.clone();
	I_2_filt = 0;
	cv::filter2D(Im, I_2_filt, -1, I_2, cv::Point(0, 0));

	cv::Mat Medium(Im.size(), CV_32FC1);
	for (int i = 0; i < I_1_filt.rows; i++) {
		for (int j = 0; j < I_1_filt.cols; j++) {
			Medium.at<float>(i, j) = std::sqrt(I_1_filt.at<float>(i, j) * I_1_filt.at<float>(i, j) + I_2_filt.at<float>(i, j) * I_2_filt.at<float>(i, j));
		}
	}

	I_1_filt = (I_1_filt + 1) / 2;
	I_2_filt = (I_2_filt + 1) / 2;

	cv::waitKey(0);

	cv::imwrite("Image.png", Im*255);
	cv::imwrite("I_1.png", I_1_filt*255);
	cv::imwrite("I_2.png", I_2_filt*255);
	cv::imwrite("Medium.png", Medium*255);
}