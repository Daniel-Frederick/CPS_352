#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// Global variables
Mat image;
Mat image2;
Mat image_org;
Point pt = Point(-1, -1);
bool drawing = false;
unsigned char *arr1D; // 1D array to hold image data

int m_blurDegree = 5; // You can adjust this degree for stronger/weaker blur
string m_imgPath;     // To store the image path

// Mouse callback function to handle the rectangle drawing and blur effect
void drawRectangleCallback(int event, int x, int y, int flags, void *userData) {
  if (!userData)
    return;

  Mat *image = static_cast<Mat *>(userData);
  int kernelSize = 2 * m_blurDegree + 1;
  int kernelOffset = kernelSize / 2;

  if (event == EVENT_LBUTTONDOWN) {
    drawing = true;
    pt = Point(x, y);
    image->copyTo(image_org); // Keep a copy of the original image for restoring
    std::memcpy(arr1D, image->data, image->rows * image->cols * 3);
  } else if (event == EVENT_MOUSEMOVE && drawing) {
    image_org.copyTo(*image); // Restore image from the original copy
    rectangle(*image, pt, Point(x, y), Scalar(255, 0, 0),
              2); // Draw the rectangle

    int minX = std::max(kernelOffset, std::min(pt.x, x));
    int maxX = std::min(image->cols - kernelOffset, std::max(pt.x, x));
    int minY = std::max(kernelOffset, std::min(pt.y, y));
    int maxY = std::min(image->rows - kernelOffset, std::max(pt.y, y));

    // Apply blur effect
    for (int j = minY; j < maxY; j++) {
      for (int i = minX; i < maxX; i++) {
        int idx = (j * image->cols + i) * 3;
        for (int c = 0; c < 3; c++) {
          int sum = 0;
          for (int dy = -kernelOffset; dy <= kernelOffset; dy++) {
            for (int dx = -kernelOffset; dx <= kernelOffset; dx++) {
              int pidx = ((j + dy) * image->cols + (i + dx)) * 3 + c;
              sum += arr1D[pidx];
            }
          }
          image->data[idx + c] = sum / (kernelSize * kernelSize);
        }
      }
    }
  } else if (event == EVENT_LBUTTONUP) {
    if (drawing && !image->empty()) {
      int minX = std::max(kernelOffset, std::min(pt.x, x));
      int maxX = std::min(image->cols - kernelOffset, std::max(pt.x, x));
      int minY = std::max(kernelOffset, std::min(pt.y, y));
      int maxY = std::min(image->rows - kernelOffset, std::max(pt.y, y));

      // Apply blur effect after mouse release
      for (int j = minY; j < maxY; j++) {
        for (int i = minX; i < maxX; i++) {
          int idx = (j * image->cols + i) * 3;
          for (int c = 0; c < 3; c++) {
            int sum = 0;
            for (int dy = -kernelOffset; dy <= kernelOffset; dy++) {
              for (int dx = -kernelOffset; dx <= kernelOffset; dx++) {
                int pidx = ((j + dy) * image->cols + (i + dx)) * 3 + c;
                sum += arr1D[pidx];
              }
            }
            image->data[idx + c] = sum / (kernelSize * kernelSize);
          }
        }
      }

      rectangle(*image, pt, Point(x, y), Scalar(255, 0, 0), 2);
      std::memcpy(arr1D, image->data, image->rows * image->cols * 3);
    }
    drawing = false;
  }
}

// Save the modified image to a new location with "2" appended to the filename
void save() {
  if (m_imgPath.empty()) {
    std::cerr << "Error: Image path is empty." << std::endl;
    return;
  }

  std::filesystem::path filePath(m_imgPath);
  std::string filename = filePath.filename().string();

  // Construct the save path with "2" appended to the filename
  std::string homeDir = std::getenv("HOME");
  std::string downloadPath = homeDir + "/Downloads/";
  std::string newFilename =
      filePath.stem().string() + "2" + filePath.extension().string();
  std::string newSavePath = downloadPath + newFilename;

  // Create a modified image from arr1D
  Mat modifiedImage(image.rows, image.cols, image.type());
  std::memcpy(modifiedImage.data, arr1D, image.rows * image.cols * 3);

  // Save the modified image only
  imwrite(newSavePath, modifiedImage);
}

// Reset image to original
void reset() {
  if (!image_org.empty()) {
    image2.copyTo(image);
  } else {
    std::cerr << "Error: Original image not available." << std::endl;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Include an image path!" << endl;
    exit(EXIT_FAILURE);
  }

  m_imgPath = argv[1];
  image = imread(m_imgPath);  // Load image
  image2 = imread(m_imgPath); // Load image
  image.copyTo(image_org);    // Copy image for restoration later

  cout << "Image size: (" << image.rows << ", " << image.cols << ")\n";

  // Create a 1D array and assign values from image
  arr1D = new unsigned char[image.rows * image.cols * 3];
  std::memcpy(arr1D, image.data, image.rows * image.cols * 3);

  namedWindow("My Window");
  setMouseCallback("My Window", drawRectangleCallback, &image);

  while (true) {
    imshow("My Window", image);

    char c = waitKey(100);
    switch (c) {
    case 'i':
    case 'I':
      if (m_blurDegree < 5)
        m_blurDegree++;
      else
        m_blurDegree += 5;
      cout << "working i" << m_blurDegree << endl;
      break;
    case 'd':
    case 'D':
      if (m_blurDegree <= 5 && m_blurDegree > 1)
        m_blurDegree--;
      else
        m_blurDegree -= 5;
      cout << "m_blurDegree" << m_blurDegree << endl;
      break;
    case 's':
    case 'S':
      save();
      break;
    case 'r':
    case 'R':
      reset();
      break;
    default:
      break;
    }
    if (c == 27) {
      break; // ESC key to exit
    }
  }

  // Clean up
  delete[] arr1D;

  return 0;
}
