#pragma once
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Events_NS {
class Events {
public:
  Events(const std::string &imgPath, const cv::Mat &image);
  ~Events();

  static void drawRectangleCallback(int event, int x, int y, int flags,
                                    void *userData);
  void userInput(const char *c);

private:
  void save();
  void remove();

  int m_blurDegree = 5;
  std::string m_imgPath;
  cv::Mat m_image;
  static cv::Mat m_tempImage;
  static unsigned char *arr1D; // Dynamic array to hold image data
  static cv::Mat tempImage;    // For storing temporary image while drawing
  static bool drawing;
  static cv::Point startPoint;
};
} // namespace Events_NS
