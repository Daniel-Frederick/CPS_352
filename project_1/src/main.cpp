#include "include/events.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>

#include <opencv2/core/core.hpp>       // class structure
#include <opencv2/highgui/highgui.hpp> // provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

int main(int argc, char *argv[]) {
  const std::string WINDOW = "Dan's Window";
  cv::Mat image = cv::imread("assets/forest.jpg");
  if (image.empty()) {
    std::cout << "Could not load image!" << std::endl;
    return -1;
  }
  cv::namedWindow(WINDOW);

  // Events
  Events_NS::Events events;
  cv::setMouseCallback(WINDOW, events.drawRectangleCallback, &image);

  while (true) {
    // Create window and display img
    imshow(WINDOW, image);

    char c = cv::waitKey(1); // In milliseconds
    if (c == 27 || c == 's' || c == 'S')
      break;
  }

  return 1;
}
