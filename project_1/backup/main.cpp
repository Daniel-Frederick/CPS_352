#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat image, image_org, image_unmodified;
Point pt = Point(-1, -1);
bool drawing = false;
unsigned char *arr1D;
int m_blurDegree = 3;
std::string m_imgPath;

// Structure to store blur rectangles
struct BlurRect {
  Rect rect;    // OpenCV rectangle coordinates
  Mat original; // Original image data for this region
};
std::vector<BlurRect> blurRects; // Collection of all blur regions

void applyMosaicEffect(Mat &region, int blur_degree) {
  // Iterate through the region in block-sized steps
  for (int y = 0; y < region.rows - blur_degree; y += blur_degree) {
    for (int x = 0; x < region.cols - blur_degree; x += blur_degree) {
      Vec3f avgColor(0, 0, 0); // Floating point vector for color averaging
      int count = 0;

      // Calculate average color in current block
      for (int by = 0; by < blur_degree; by++) {
        for (int bx = 0; bx < blur_degree; bx++) {
          if (y + by < region.rows && x + bx < region.cols) {
            Vec3b pixel = region.at<Vec3b>(y + by, x + bx);
            avgColor[0] += pixel[0]; // Blue channel
            avgColor[1] += pixel[1]; // Green channel
            avgColor[2] += pixel[2]; // Red channel
            count++;
          }
        }
      }

      avgColor /= count; // Compute average color

      // Apply average color to all pixels in the block
      for (int by = 0; by < blur_degree; by++) {
        for (int bx = 0; bx < blur_degree; bx++) {
          if (y + by < region.rows && x + bx < region.cols) {
            region.at<Vec3b>(y + by, x + bx) =
                Vec3b(avgColor[0], avgColor[1], avgColor[2]);
          }
        }
      }
    }
  }
}

void reapplyBlurs() {
  // Restore original image
  image_unmodified.copyTo(image);

  // Apply all stored mosaic effects
  for (const auto &blur : blurRects) {
    Mat region = image(blur.rect);
    applyMosaicEffect(region, m_blurDegree);
  }

  // Redraw all rectangle outlines
  for (const auto &blur : blurRects) {
    rectangle(image, blur.rect, Scalar(255, 0, 0), 2);
  }
}

void drawRectangleCallback(int event, int x, int y, int flags, void *userData) {
  if (!userData)
    return;
  Mat *image = static_cast<Mat *>(userData);

  switch (event) {
  case EVENT_LBUTTONDOWN:
    drawing = true;
    pt = Point(x, y);
    image->copyTo(image_org); // Save current state
    break;

  case EVENT_MOUSEMOVE:
    if (drawing) {
      image_org.copyTo(*image);
      // Redraw existing rectangles
      for (const auto &blur : blurRects) {
        rectangle(*image, blur.rect, Scalar(255, 0, 0), 2);
      }
      // Draw current temporary rectangle
      rectangle(*image, pt, Point(x, y), Scalar(255, 0, 0), 2);
    }
    break;

  case EVENT_LBUTTONUP:
    if (drawing) {
      Rect roi(pt, Point(x, y));
      if (roi.width > 0 && roi.height > 0) {
        // Normalize coordinates (handle any drawing direction)
        roi = Rect(min(pt.x, x), min(pt.y, y), abs(x - pt.x), abs(y - pt.y));

        // Store rectangle and original pixels
        BlurRect blurRect;
        blurRect.rect = roi;
        blurRect.original = image_unmodified(roi).clone();
        blurRects.push_back(blurRect);

        // Apply mosaic to new region
        Mat region = (*image)(roi);
        applyMosaicEffect(region, m_blurDegree);

        // Draw outline
        rectangle(*image, roi, Scalar(255, 0, 0), 2);
      }
      drawing = false;
    }
    break;
  }
}

void save() {
  filesystem::path filePath(m_imgPath);
  string newFilename =
      filePath.stem().string() + "_mosaic" + filePath.extension().string();
  string savePath = filePath.parent_path().string() + "/" + newFilename;
  imwrite(savePath, image);
}

void reset() {
  image_unmodified.copyTo(image);
  blurRects.clear();
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <image_path>" << endl;
    exit(EXIT_FAILURE);
  }

  m_imgPath = argv[1];
  image = imread(m_imgPath);

  if (image.empty()) {
    cout << "Error: Could not read image file: " << m_imgPath << endl;
    exit(EXIT_FAILURE);
  }

  // Create backup copies of the original image
  image.copyTo(image_org);
  image.copyTo(image_unmodified);

  const string WINDOW = "Dan's Window";
  namedWindow(WINDOW);
  setMouseCallback(WINDOW, drawRectangleCallback, &image);

  while (true) {
    imshow(WINDOW, image);
    char c = waitKey(100);

    switch (c) {
    case 'i':
    case 'I':
      if (m_blurDegree < 5)
        m_blurDegree++;
      else
        m_blurDegree += 5;
      reapplyBlurs();
      break;

    case 'd':
    case 'D':
      if (m_blurDegree <= 5 && m_blurDegree > 1)
        m_blurDegree--;
      else if (m_blurDegree > 5)
        m_blurDegree -= 5;
      m_blurDegree = max(m_blurDegree, 1);
      reapplyBlurs();
      break;

    case 's':
    case 'S':
      save();
      break;

    case 'r':
    case 'R':
      reset();
      break;

    case 27:
      return 0;
    }
  }
}
