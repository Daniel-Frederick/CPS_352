#include <opencv2/core/core.hpp> // class structure

#include <opencv2/highgui/highgui.hpp> // provide input/output functions

#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

int main (int argc, char* argc[]) {
  Mat img; // matrix is considered as an image holder

  // Load image file into RAM and store it as "img" variable
  img = imread("scene.png");

  while (true) {
    // OpenCV create a window
    imshow("window", img);

    char c = waitKey(1);
    if (c == 27) break; // ASCII code for ESC key
    if (c == 's' || c == 'S') break; // Custom close key 
  }
  
  return 1;
}