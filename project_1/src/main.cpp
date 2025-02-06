#include "include/events.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cinttypes>
#include <cstdlib>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>       // class structure
#include <opencv2/highgui/highgui.hpp> // provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Include an image path!" << std::endl;
    exit(EXIT_FAILURE);
  }

  const std::string WINDOW = "Dan's Window";
  // cv::Mat image =
  // cv::imread("/home/frederickd/projects/CPS_352/project_1/assets/forest.jpg");

  std::string imgPath = argv[1];
  cv::Mat image = cv::imread(imgPath);
  if (image.empty()) {
    std::cout << "Could not load image!" << std::endl;
    return -1;
  }
  cv::namedWindow(WINDOW);

  // Events
  Events_NS::Events events(imgPath, image);
  cv::setMouseCallback(WINDOW, events.drawRectangleCallback, &image);

  while (true) {
    imshow(WINDOW, image);
    char c = cv::waitKey(1);
    events.userInput(&c);
    if (c == 27)
      break;
  }

  return 1;
}
