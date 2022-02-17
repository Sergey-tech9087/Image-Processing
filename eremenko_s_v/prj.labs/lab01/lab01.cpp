#include <opencv2/opencv.hpp>

int main() {
  cv::Mat I_1(60, 768, CV_8UC1);
  I_1 = 0;
  double k = 2.3;
  int t = 0;

  for (int i = 0; i < 60; i++) {
    t = 0;
    for (int j = 0; j < 768; j += 3) {
      I_1.at<int8_t>(i, j) = t;
      I_1.at<int8_t>(i, j + 1) = t;
      I_1.at<int8_t>(i, j + 2) = t;
      t++;
    }
  }

  float start_01 = clock();

  cv::Mat I_2(60, 768, CV_8UC1);
  I_1.convertTo(I_2, CV_32F, 1 / 255.0);
  cv::pow(I_2, k, I_2);
  I_2.convertTo(I_2, CV_8UC1, 255);

  float time_01 = clock() - start_01;

  float start_02 = clock();

  cv::Mat I_3(60, 768, CV_8UC1);
  for (int i = 0; i < 60; i++) {
    for (int j = 0; j < 768; j++) {
      I_3.at<int8_t>(i, j) = pow((j / 3) / 255.0, k) * 255.0;
    }
  }

  float time_02 = clock() - start_02;

  cv::Mat img(180, 768, CV_8UC1);
  I_1.copyTo(img(cv::Rect2d(0, 0, 768, 60)));
  I_2.copyTo(img(cv::Rect2d(0, 60, 768, 60)));
  I_3.copyTo(img(cv::Rect2d(0, 120, 768, 60)));

  std::cout << "Time pow: " << time_01 << std::endl;
  std::cout << "Time at: " << time_02 << std::endl;

  cv::imwrite("lab01.png", img);
}
