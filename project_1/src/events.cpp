#include "include/events.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <filesystem>

namespace Events_NS {
unsigned char *Events::arr1D = nullptr;
cv::Mat Events::tempImage;
bool Events::drawing = false;
cv::Point Events::startPoint(-1, -1);
// Constructor definition
Events::Events(const std::string &imgPath, const cv::Mat &image)
    : m_imgPath(imgPath), m_image(image) {
  // Allocate memory for the dynamic array
  arr1D = new unsigned char[image.rows * image.cols * 3];
  // Copy initial image data to array
  memcpy(arr1D, image.data, image.rows * image.cols * 3);
}

Events::~Events() {
  if (arr1D) {
    delete[] arr1D;
    arr1D = nullptr;
  }
}

void Events::drawRectangleCallback(int event, int x, int y, int flags,
                                   void *userData) {
  if (!userData)
    return;

  cv::Mat *image = static_cast<cv::Mat *>(userData);

  if (event == cv::EVENT_LBUTTONDOWN) {
    drawing = true;
    startPoint = cv::Point(x, y);
    // Create a copy of the original image
    image->copyTo(tempImage);
    // Update the dynamic array with current image data
    memcpy(arr1D, image->data, image->rows * image->cols * 3);
  } else if (event == cv::EVENT_MOUSEMOVE && drawing) {
    // Create a copy for display
    tempImage.copyTo(*image);

    // Draw rectangle
    cv::rectangle(*image, startPoint, cv::Point(x, y), cv::Scalar(255, 0, 0),
                  2);

    // Calculate region bounds
    int minX = std::max(0, std::min(startPoint.x, x));
    int maxX = std::min(image->cols, std::max(startPoint.x, x));
    int minY = std::max(0, std::min(startPoint.y, y));
    int maxY = std::min(image->rows, std::max(startPoint.y, y));

    // Apply blur effect using the dynamic array
    for (int j = minY; j < maxY; j++) {
      for (int i = minX; i < maxX; i++) {
        int idx = (j * image->cols + i) * 3;

        // Simple blur effect by averaging neighboring pixels
        if (i > 0 && i < image->cols - 1 && j > 0 && j < image->rows - 1) {
          for (int c = 0; c < 3; c++) {
            int sum = 0;
            for (int dy = -1; dy <= 1; dy++) {
              for (int dx = -1; dx <= 1; dx++) {
                int pidx = ((j + dy) * image->cols + (i + dx)) * 3 + c;
                sum += arr1D[pidx];
              }
            }
            image->data[idx + c] = sum / 9;
          }
        }
      }
    }
  } else if (event == cv::EVENT_LBUTTONUP) {
    if (drawing && !image->empty()) {
      // Calculate final region bounds
      int minX = std::max(0, std::min(startPoint.x, x));
      int maxX = std::min(image->cols, std::max(startPoint.x, x));
      int minY = std::max(0, std::min(startPoint.y, y));
      int maxY = std::min(image->rows, std::max(startPoint.y, y));

      // Apply final blur effect
      for (int j = minY; j < maxY; j++) {
        for (int i = minX; i < maxX; i++) {
          int idx = (j * image->cols + i) * 3;

          // Apply blur using the same technique as during movement
          if (i > 0 && i < image->cols - 1 && j > 0 && j < image->rows - 1) {
            for (int c = 0; c < 3; c++) {
              int sum = 0;
              for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                  int pidx = ((j + dy) * image->cols + (i + dx)) * 3 + c;
                  sum += arr1D[pidx];
                }
              }
              image->data[idx + c] = sum / 9;
            }
          }
        }
      }

      // Draw final rectangle
      cv::rectangle(*image, startPoint, cv::Point(x, y), cv::Scalar(255, 0, 0),
                    2);

      // Update the dynamic array with the modified image
      memcpy(arr1D, image->data, image->rows * image->cols * 3);
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

void Events::save() {
  // TODO: What I need to do is create a new image when I create a rectangle
  // Currently, the original image is being passed through m_imgPath and will
  // save the original img Extract filename from the input path
  std::filesystem::path filePath(m_imgPath);
  std::string filename = filePath.filename().string(); // e.g., "forest.jpg"

  // TODO: This has to work on Windows, not linux :(
  // Get the user's Downloads folder
  std::string homeDir = std::getenv("HOME");
  std::string downloadPath = homeDir + "/Downloads/";

  // Create the new filenames
  std::string originalSavePath = downloadPath + filename;
  std::string newFilename =
      filePath.stem().string() + "2" + filePath.extension().string();
  std::string newSavePath = downloadPath + newFilename;

  // Print paths for debugging
  std::cout << "Saving original file to: " << originalSavePath << std::endl;
  std::cout << "Saving new file to: " << newSavePath << std::endl;

  // Load image (assuming m_image is already loaded; otherwise, use cv::imread)
  cv::Mat m_image = cv::imread(m_imgPath);
  if (m_image.empty()) {
    std::cerr << "Error: Unable to load image." << std::endl;
    return;
  }

  // Save the images
  if (cv::imwrite(originalSavePath, m_image)) {
    std::cout << "Original image saved successfully." << std::endl;
  } else {
    std::cerr << "Failed to save original image." << std::endl;
  }

  if (cv::imwrite(newSavePath, m_image)) {
    std::cout << "Modified image saved successfully." << std::endl;
  } else {
    std::cerr << "Failed to save modified image." << std::endl;
  }
}

void Events::remove() {
  std::cout << "remove method called" << std::endl;
  cv::Mat image = cv::imread(m_imgPath);
}
} // namespace Events_NS
