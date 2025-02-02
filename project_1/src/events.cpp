#include "include/events.hpp"

namespace Events_NS {

// Constructor definition
Events::Events() {}

// TODO: Make it so the rectangle moves with mouse hold
void Events::drawRectangleCallback(int event, int x, int y, int flags,
                                   void *userData) {
  if (event == cv::EVENT_LBUTTONDOWN) {
    std::cout << "onMouseClick works" << std::endl;
    // Ensure userData is a valid OpenCV Mat pointer
    if (userData) {
      cv::Mat *image = static_cast<cv::Mat *>(userData);
      if (!image->empty()) {
        cv::rectangle(*image, cv::Point(x - 10, y - 10),
                      cv::Point(x + 10, y + 10), cv::Scalar(255, 0, 0), 2);
      }
    }
  }
}
} // namespace Events_NS
