#include "include/events.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>       // class structure
#include <opencv2/highgui/highgui.hpp> // provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

int main(int argc, char *argv[]) {
  const std::string WINDOW = "Dan's Window";
  cv::Mat image = cv::imread(
      "/home/frederickd/projects/CPS_352/project_1/assets/forest.jpg");
  if (image.empty()) {
    std::cout << "Could not load image!" << std::endl;
    return -1;
  }
  cv::namedWindow(WINDOW);

  int blur_degree = 5;
  // Events
  Events_NS::Events events;
  cv::setMouseCallback(WINDOW, events.drawRectangleCallback, &image);

  while (true) {
    // Create window and display img
    imshow(WINDOW, image);

    char c = cv::waitKey(1); // In milliseconds
    // I might want to move this switch to the event class
    // I might also want to move blur_degree to event as well
    // I would only have to call a single function here if I get it to work
    switch (c) {
    case 'i':
    case 'I':
      std::cout << "i or I pressed" << std::endl;
      blur_degree++;
      break;
    case 'd':
    case 'D':
      std::cout << "d or D pressed" << std::endl;
      blur_degree--;
      break;
    case 's':
    case 'S':
      std::cout << "s or S pressed" << std::endl;
      // Save file into local system
      // events.save();
      break;
    case 'r':
    case 'R':
      std::cout << "r or R pressed" << std::endl;
      // Remove everything and start over
      // events.remove();
      break;
    default:
      break;
    }
    if (c == 27)
      break;
  }

  return 1;
}
