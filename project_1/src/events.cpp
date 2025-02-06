#include "include/events.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <filesystem>

namespace Events_NS {

// Constructor definition
Events::Events(const std::string &imgPath, const cv::Mat &image)
    : m_imgPath(imgPath), m_image(image) {}

void Events::drawRectangleCallback(int event, int x, int y, int flags,
                                   void *userData) {
  // TODO: See rectangle on screen as you move
  // TODO: Add blur effect inside rectangle
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
