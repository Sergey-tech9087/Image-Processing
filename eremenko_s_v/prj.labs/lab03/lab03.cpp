#include <opencv2/opencv.hpp>
#include <cmath>

int func(const int col){
  return static_cast<int>(col*col + 2*col + 3);
}

int main() {
  std::string name = "../../../data/cross_0256x0256.png";
  cv::Mat img = cv::imread(name);
  cv::Mat grayscale = cv::imread(name, cv::IMREAD_GRAYSCALE);

  cv::imwrite("lab03_rgb.png", img);

  cv::Mat LookUpTable(1, 256, CV_8UC1);

  for (int i = 0; i < 256; i++){
    LookUpTable.at<uchar>(0, i) = func(i);
  }
  cv::Mat m(180, 768, CV_8UC1);

  m = 0;
  for (int i = 0; i < 180; i++) {
    for (int j = 0; j < 768; j++) {
      m.at<uchar>(i, j) = j / 3;
    }
  }

  cv::Mat a;
  cv::Mat b;
  cv::Mat c;

  cv::LUT(img, LookUpTable, a);
  cv::LUT(grayscale, LookUpTable, b);
  cv::LUT(m, LookUpTable, c);
    
  cv::imwrite("lab03_gre_res.png", a);
  cv::imwrite("lab03_rgb_res.png", b);
  cv::imwrite("lab03_checker.png", c);

  cv::Mat FunctionVisualization(512, 512, CV_8UC1, 255);

  for (int i = 0; i < 512; i += 2) {
    FunctionVisualization.at<uchar>(512 - 2 * LookUpTable.at<uchar>(0, i / 2) - 1, i) = 0;
  }

  cv::imwrite("lab03_viz_func.png", FunctionVisualization);

  return 0;
}