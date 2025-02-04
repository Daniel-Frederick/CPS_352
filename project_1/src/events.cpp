#include "include/events.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

namespace Events_NS {

// Constructor definition
Events::Events() {}

void Events::drawRectangleCallback(int event, int x, int y, int flags,
                                   void *userData) {
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
} // namespace Events_NS
