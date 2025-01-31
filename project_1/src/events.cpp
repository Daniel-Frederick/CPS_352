#include "include/events.hpp"

namespace Events_NS {

// Constructor definition
Events::Events() {}

// Mouse click event handler
void Events::onMouseClick(int event, int x, int y, int flags, void *userData) {
  // boolean for when the user holds mouse button
  bool drawing = true;
  if (event == cv::EVENT_LBUTTONDOWN) {
    // Ensure userData is a valid OpenCV Mat pointer
    if (userData) {
      cv::Mat *image = static_cast<cv::Mat *>(userData);
      if (!image->empty()) {
        cv::rectangle(*image, cv::Point(x - 10, y - 10),
                      cv::Point(x + 10, y + 10), cv::Scalar(255, 0, 0), 2);
        cv::imshow("Window", *image);
      }
    }
  }
}
} // namespace Events_NS
