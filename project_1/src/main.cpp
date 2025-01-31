#include "include/events.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>

#include <opencv2/core/core.hpp>       // class structure
#include <opencv2/highgui/highgui.hpp> // provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

int main(int argc, char *argv[]) {
  const std::string WINDOW = "Dan's Window";
  Events_NS::Events events;
  cv::Mat img; // matrix is considered as an image holder

  // Load image file into RAM and store it as "img" variable
  img = cv::imread("forest.jpg");
  cv::namedWindow(WINDOW);

  while (true) {
    // Create window and display img
    imshow(WINDOW, img);

    // Events
    cv::setMouseCallback(WINDOW, events.onMouseClick);

    char c = cv::waitKey(1); // In milliseconds
    if (c == 27)
      break; // ASCII code for ESC key
    if (c == 's' || c == 'S')
      break; // Custom close key
  }

  return 1;
}
