#include <opencv2/opencv.hpp>

int main() {
  std::string name = "../../../data/cross_0256x0256.png";
  std::string name_jpeg = "./cross_0256x0256_025.jpg";

  cv::Mat img = cv::imread(name);

  cv::imwrite("cross_0256x0256_025.jpg", img, { cv::IMWRITE_JPEG_QUALITY, 25 });

  cv::Mat img_jpeg = cv::imread(name_jpeg);

  cv::Mat bgr[3];
  cv::Mat bgr_jpeg[3];
  cv::Mat img_channels(img.rows * 2, img.cols * 2, CV_8UC3);
  cv::Mat img_channels_jpeg(img_jpeg.rows * 2, img_jpeg.cols * 2, CV_8UC3);
  cv::Mat zeros = cv::Mat::zeros(cv::Size(img.rows, img.cols), CV_8UC1);

  cv::split(img, bgr);
  cv::split(img_jpeg, bgr_jpeg);

  cv::Mat blue_channel = bgr[0];
  cv::Mat green_channel = bgr[1];
  cv::Mat red_channel = bgr[2];
  cv::Mat blue_channel_jpeg = bgr_jpeg[0];
  cv::Mat green_channel_jpeg = bgr_jpeg[1];
  cv::Mat red_channel_jpeg = bgr_jpeg[2];

  cv::Mat img_bgr[3];
  cv::Mat img_bgr_jpeg[3];

  cv::merge(std::vector<cv::Mat>({ bgr[0], zeros, zeros }), img_bgr[0]);
  cv::merge(std::vector<cv::Mat>({ zeros, bgr[1], zeros }), img_bgr[1]);
  cv::merge(std::vector<cv::Mat>({ zeros, zeros, bgr[2] }), img_bgr[2]);

  img.copyTo(img_channels(cv::Rect(0, 0, 256, 256)));
  img_bgr[0].copyTo(img_channels(cv::Rect(256, 256, 256, 256)));
  img_bgr[1].copyTo(img_channels(cv::Rect(0, 256, 256, 256)));
  img_bgr[2].copyTo(img_channels(cv::Rect(256, 0, 256, 256)));

  cv::imwrite("cross_0256x0256_png_channels.png", img_channels);

  cv::merge(std::vector<cv::Mat>({ bgr_jpeg[0], zeros, zeros }), img_bgr_jpeg[0]);
  cv::merge(std::vector<cv::Mat>({ zeros, bgr_jpeg[1], zeros }), img_bgr_jpeg[1]);
  cv::merge(std::vector<cv::Mat>({ zeros, zeros, bgr_jpeg[2] }), img_bgr_jpeg[2]);

  img_jpeg.copyTo(img_channels_jpeg(cv::Rect(0, 0, 256, 256)));
  img_bgr_jpeg[0].copyTo(img_channels_jpeg(cv::Rect(256, 256, 256, 256)));
  img_bgr_jpeg[1].copyTo(img_channels_jpeg(cv::Rect(0, 256, 256, 256)));
  img_bgr_jpeg[2].copyTo(img_channels_jpeg(cv::Rect(256, 0, 256, 256)));

  cv::imwrite("cross_0256x0256_jpg_channels.png", img_channels_jpeg);

  bool uniform = true, accumulate = false;

  int hist_width = 256;
  const int hist_height = 256;

  int hist_width_jpeg = hist_width;
  const int hist_height_jpeg = 768;

  float range[] = { 0, 256 };
  const float* hist_range[] = { range };

  cv::Mat blue_hist = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat green_hist = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat red_hist = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat bgr_hist = cv::Mat::zeros(hist_height, hist_width, CV_8UC1);

  cv::Mat blue_hist_img = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat green_hist_img = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat red_hist_img = cv::Mat::zeros(hist_height, hist_width, img.type());
  cv::Mat bgr_hist_img = cv::Mat::zeros(hist_height, hist_width, CV_8UC1);

  cv::Mat blue_hist_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat green_hist_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat red_hist_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat bgr_hist_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, CV_8UC1);

  cv::Mat blue_hist_img_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat green_hist_img_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat red_hist_img_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, img_jpeg.type());
  cv::Mat bgr_hist_img_jpeg = cv::Mat::zeros(hist_height_jpeg, hist_width_jpeg, CV_8UC1);

  cv::Mat gray_img(img.rows, img.cols, CV_8UC1);
  cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

  cv::Mat gray_img_jpeg(img_jpeg.rows, img_jpeg.cols, CV_8UC1);
  cv::cvtColor(img_jpeg, gray_img_jpeg, cv::COLOR_BGR2GRAY);

  cv::calcHist(&blue_channel, 1, 0, cv::Mat(), blue_hist, 1, &hist_width, hist_range, uniform, accumulate);
  cv::calcHist(&green_channel, 1, 0, cv::Mat(), green_hist, 1, &hist_width, hist_range, uniform, accumulate);
  cv::calcHist(&red_channel, 1, 0, cv::Mat(), red_hist, 1, &hist_width, hist_range, uniform, accumulate);
  cv::calcHist(&gray_img, 1, 0, cv::Mat(), bgr_hist, 1, &hist_width, hist_range, uniform, accumulate);

  cv::calcHist(&blue_channel_jpeg, 1, 0, cv::Mat(), blue_hist_jpeg, 1, &hist_width_jpeg, hist_range, uniform, accumulate);
  cv::calcHist(&green_channel_jpeg, 1, 0, cv::Mat(), green_hist_jpeg, 1, &hist_width_jpeg, hist_range, uniform, accumulate);
  cv::calcHist(&red_channel_jpeg, 1, 0, cv::Mat(), red_hist_jpeg, 1, &hist_width_jpeg, hist_range, uniform, accumulate);
  cv::calcHist(&gray_img_jpeg, 1, 0, cv::Mat(), bgr_hist_jpeg, 1, &hist_width_jpeg, hist_range, uniform, accumulate);

  double max_value = 0;
  cv::minMaxLoc(blue_hist, 0, &max_value);

  for (int i = 0; i < hist_width; i++) {
    float bin_val = blue_hist.at<float>(i);
    int height = cvRound(bin_val * hist_height / max_value);
    cv::line(blue_hist_img, cv::Point(i, hist_height - height), cv::Point(i, hist_height), cv::Scalar(255, 0, 0));
  }

  cv::minMaxLoc(green_hist, 0, &max_value);

  for (int i = 0; i < hist_width; i++) {
    float bin_val = green_hist.at<float>(i);
    int height = cvRound(bin_val * hist_height / max_value);
    cv::line(green_hist_img, cv::Point(i, hist_height - height), cv::Point(i, hist_height), cv::Scalar(0, 255, 0));
  }

  cv::minMaxLoc(red_hist, 0, &max_value);

  for (int i = 0; i < hist_width; i++) {
    float bin_val = red_hist.at<float>(i);
    int height = cvRound(bin_val * hist_height / max_value);
    cv::line(red_hist_img, cv::Point(i, hist_height - height), cv::Point(i, hist_height), cv::Scalar(0, 0, 255));
  }

  cv::minMaxLoc(bgr_hist, 0, &max_value);

  for (int i = 0; i < hist_width; i++) {
    float bin_val = bgr_hist.at<float>(i);
    int height = cvRound(bin_val * hist_height / max_value);
    cv::line(bgr_hist_img, cv::Point(i, hist_height - height), cv::Point(i, hist_height), cv::Scalar(1, 1, 1));
  }

  cv::minMaxLoc(blue_hist_jpeg, 0, &max_value);

  for (int i = 0; i < hist_width_jpeg; i++) {
    float bin_val = blue_hist_jpeg.at<float>(i);
    int height = cvRound(bin_val * hist_height_jpeg / max_value);
    cv::line(blue_hist_img_jpeg, cv::Point(i, hist_height_jpeg - height), cv::Point(i, hist_height_jpeg), cv::Scalar(255, 0, 0));
  }

  cv::minMaxLoc(green_hist_jpeg, 0, &max_value);

  for (int i = 0; i < hist_width_jpeg; i++) {
    float bin_val = green_hist_jpeg.at<float>(i);
    int height = cvRound(bin_val * hist_height_jpeg / max_value);
    cv::line(green_hist_img_jpeg, cv::Point(i, hist_height_jpeg - height), cv::Point(i, hist_height_jpeg), cv::Scalar(0, 255, 0));
  }

  cv::minMaxLoc(red_hist_jpeg, 0, &max_value);

  for (int i = 0; i < hist_width_jpeg; i++) {
    float bin_val = red_hist_jpeg.at<float>(i);
    int height = cvRound(bin_val * hist_height_jpeg / max_value);
    cv::line(red_hist_img_jpeg, cv::Point(i, hist_height_jpeg - height), cv::Point(i, hist_height_jpeg), cv::Scalar(0, 0, 255));
  }

  cv::minMaxLoc(bgr_hist_jpeg, 0, &max_value);

  for (int i = 0; i < hist_width_jpeg; i++) {
    float bin_val = bgr_hist_jpeg.at<float>(i);
    int height = cvRound(bin_val * hist_height_jpeg / max_value);
    cv::line(bgr_hist_img_jpeg, cv::Point(i, hist_height_jpeg - height), cv::Point(i, hist_height_jpeg), cv::Scalar(1, 1, 1));
  }

  cv::Mat hists_img(img.cols * 2, img.rows * 2, img.type());
  hists_img = 0;
  cv::cvtColor(bgr_hist_img, bgr_hist_img, cv::COLOR_GRAY2BGR);
  cv::Rect2d rect(0, 0, img.cols, img.rows);
  bgr_hist_img.copyTo(hists_img(rect));
  rect.y += rect.height;
  green_hist_img.copyTo(hists_img(rect));
  rect.x += rect.width;
  red_hist_img.copyTo(hists_img(rect));
  rect.y -= rect.height;
  blue_hist_img.copyTo(hists_img(rect));

  cv::Mat hists_img_jpeg(img_jpeg.rows * 3, img_jpeg.cols * 4 + 3, CV_8UC3);
  hists_img_jpeg = 0;
  cv::cvtColor(bgr_hist_img_jpeg, bgr_hist_img_jpeg, cv::COLOR_GRAY2BGR);
  bgr_hist_img_jpeg.copyTo(hists_img_jpeg(cv::Rect(0, 0, hist_width_jpeg, hist_height_jpeg)));
  green_hist_img_jpeg.copyTo(hists_img_jpeg(cv::Rect(hist_width_jpeg + 1, 0, hist_width_jpeg, hist_height_jpeg)));
  red_hist_img_jpeg.copyTo(hists_img_jpeg(cv::Rect(hist_width_jpeg * 2 + 2, 0, hist_width_jpeg, hist_height_jpeg)));
  blue_hist_img_jpeg.copyTo(hists_img_jpeg(cv::Rect(hist_width_jpeg * 3 + 3, 0, hist_width_jpeg, hist_height_jpeg)));

  for (int i = 0; i < hists_img.rows; i++) {
    for (int j = 0; j < hists_img.cols; j++) {
      cv::Vec3b intension = hists_img.at<cv::Vec3b>(i, j);
      if (intension[0] == 0 && intension[1] == 0 && intension[2] == 0) {
        hists_img.at<cv::Vec3b>(i, j) = { 255, 255, 255 };
      }
    }
  }

  for (int i = 0; i < hists_img_jpeg.rows; i++) {
    for (int j = 0; j < hists_img_jpeg.cols; j++) {
      cv::Vec3b intension = hists_img_jpeg.at<cv::Vec3b>(i, j);
      if (intension[0] == 0 && intension[1] == 0 && intension[2] == 0) {
        hists_img_jpeg.at<cv::Vec3b>(i, j) = { 255, 255, 255 };
      }
    }
  }

  cv::imwrite("cross_0256x0256_hists.png", hists_img);
  cv::imwrite("cross_0256x0256_hists_jpg.png", hists_img_jpeg);
  return 0;
}