#include<iostream>
#include <opencv2/opencv.hpp>

void ColRed(cv::Mat &img, uchar div = 64) {
  for (int i = 0; i < img.rows; i++) {
    uchar *data = img.ptr(i);

    for (int j = 0; j < img.cols * img.channels(); j++){
      data[j] = data[j] - data[j] % div + div / 2;
    }
  }
}

void Bin(cv::Mat &img, uchar t) {
  cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
  cv::threshold(img, img, t, 255, cv::THRESH_BINARY_INV);
}

int main() {
  cv::VideoCapture v("C:/sand/eremenko_s_v/data/vid02.mp4");

  cv::Mat cadr[3];
  int cadr_n = v.get(cv::CAP_PROP_FRAME_COUNT);
  
  for (int i = 0; i < cadr_n; i++) {
    if (i == ((cadr_n / 5) * 2)) {
      v >> cadr[0];
    }

    if (i == ((cadr_n / 5) * 3)) {
      v >> cadr[1];
    }

    if (i == ((cadr_n / 5) * 4)) {
      v >> cadr[2];
    }

    v.grab();
  }

  for (int i = 0; i < 3; i++) {
    cv::imwrite("cadr_" + std::to_string(i) + ".png", cadr[i]);
  }

  cv::Mat mask;

  for (int i = 0; i < 3; i++) {
    ColRed(cadr[i]);
    cv::imwrite("cadr_" + std::to_string(i) + "cr.png", cadr[i]);

    Bin(cadr[i], 150);
    cv::imwrite("cadr_" + std::to_string(i) + "bn.png", cadr[i]);

    cv::morphologyEx(cadr[i], mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(40, 40)));
    cv::imwrite("cadr_" + std::to_string(i) + "mr.png", mask);

    cv::Mat centr, stats, res;
    int n = cv::connectedComponentsWithStats(mask, res, stats, centr);

    std::vector<cv::Vec3b> l(n);
    int max = 0, imax = 0;

    for (int i = 1; i < n; i++) {
      if (max < stats.at<int>(i, cv::CC_STAT_AREA)) {
        max = stats.at<int>(i, cv::CC_STAT_AREA);
        imax = i;
      }
    }

    for (int i = 0; i < n; i++) {
      l[i] = cv::Vec3b(0, 0, 0);
    }
    l[imax] = cv::Vec3b(255, 255, 255);
    cv::Mat exit(mask.rows, mask.cols, CV_8UC3);

    for (int i = 0; i < exit.rows; i++) {
      for (int j = 0; j < exit.cols; j++) {
        int label = res.at<int>(i, j);
        cv::Vec3b &pixel = exit.at<cv::Vec3b>(i, j);
        pixel = l[label];
      }
    }

    cv::imwrite("cadr_" + std::to_string(i) + "conected.png", exit);
  }

  return 0;
}