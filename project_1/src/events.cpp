#include "include/events.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace Events_NS {

// Constructor definition
Events::Events() {}

void Events::drawRectangleCallback(int event, int x, int y, int flags,
                                   void *userData) {
  // TODO: See rectangle on screen as you move
  // TODO: Add blur effect inside rectangle
  static bool drawing = false;
  static cv::Point sPoint;

  if (!userData)
    return;
  cv::Mat *image = static_cast<cv::Mat *>(userData);

  if (event == cv::EVENT_LBUTTONDOWN) {
    drawing = true;
    sPoint = cv::Point(x, y);
  } else if (event == cv::EVENT_LBUTTONUP) {
    if (drawing && !image->empty()) {
      cv::rectangle(*image, sPoint, cv::Point(x, y), cv::Scalar(255, 0, 0), 2);
    }
    drawing = false;
  }
}

void Events::userInput(const char *c) {
  switch (*c) {
  case 'i':
  case 'I':
    m_blurDegree++;
    break;
  case 'd':
  case 'D':
    m_blurDegree--;
    break;
  case 's':
  case 'S':
    // Save file into local system
    save();
    break;
  case 'r':
  case 'R':
    // Remove everything and start over
    remove();
    break;
  default:
    break;
  }
}

void Events::save() { std::cout << "save method called" << std::endl; }

void Events::remove() { std::cout << "remove method called" << std::endl; }
} // namespace Events_NS
