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

// Global variables
Mat image, image_org, image_unmodified;
Point pt = Point(-1, -1);
bool drawing = false;
unsigned char *arr1D;
int m_blurDegree = 3;
std::string m_imgPath;

// Store the rectangles drawn
struct BlurRect {
  Rect rect;
  Mat original;
};
std::vector<BlurRect> blurRects;

// Function to apply mosaic effect to a region
void applyMosaicEffect(Mat &region, int blur_degree) {
  for (int y = 0; y < region.rows - blur_degree; y += blur_degree) {
    for (int x = 0; x < region.cols - blur_degree; x += blur_degree) {
      // Calculate average color for this block
      Vec3f avgColor(0, 0, 0);
      int count = 0;

      // Sum all pixels in this block
      for (int by = 0; by < blur_degree; by++) {
        for (int bx = 0; bx < blur_degree; bx++) {
          if (y + by < region.rows && x + bx < region.cols) {
            Vec3b pixel = region.at<Vec3b>(y + by, x + bx);
            avgColor[0] += pixel[0];
            avgColor[1] += pixel[1];
            avgColor[2] += pixel[2];
            count++;
          }
        }
      }

      // Calculate average
      avgColor /= count;

      // Set all pixels in this block to the average
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

// Function to reapply all blur effects
void reapplyBlurs() {
  image_unmodified.copyTo(image);
  for (const auto &blur : blurRects) {
    Mat region = image(blur.rect);
    applyMosaicEffect(region, m_blurDegree);
  }
}

void drawRectangleCallback(int event, int x, int y, int flags, void *userData) {
  if (!userData)
    return;
  Mat *image = static_cast<Mat *>(userData);

  if (event == EVENT_LBUTTONDOWN) {
    drawing = true;
    pt = Point(x, y);
    image->copyTo(image_org);
  } else if (event == EVENT_MOUSEMOVE && drawing) {
    image_org.copyTo(*image);
    rectangle(*image, pt, Point(x, y), Scalar(255, 0, 0), 2);
  } else if (event == EVENT_LBUTTONUP) {
    if (drawing) {
      Rect roi(pt, Point(x, y));
      if (roi.width > 0 && roi.height > 0) {
        // Normalize the rectangle coordinates
        roi = Rect(min(pt.x, x), min(pt.y, y), abs(x - pt.x), abs(y - pt.y));

        // Store the original region and rectangle
        BlurRect blurRect;
        blurRect.rect = roi;
        blurRect.original = image_unmodified(roi).clone();
        blurRects.push_back(blurRect);

        // Apply the mosaic effect
        Mat region = (*image)(roi);
        applyMosaicEffect(region, m_blurDegree);
      }
    }
    drawing = false;
  }
}

void save() {
  std::filesystem::path filePath(m_imgPath);
  std::string newFilename =
      filePath.stem().string() + "_mosaic" + filePath.extension().string();
  std::string savePath = filePath.parent_path().string() + "/" + newFilename;
  cout << "Saving modified file to: " << savePath << endl;
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

  image.copyTo(image_org);
  image.copyTo(image_unmodified);

  namedWindow("Mosaic Effect Tool");
  setMouseCallback("Mosaic Effect Tool", drawRectangleCallback, &image);

  cout << "Controls:" << endl;
  cout << "  - Click and drag to select region" << endl;
  cout << "  - Press 'i/I' to increase blur" << endl;
  cout << "  - Press 'd/D' to decrease blur" << endl;
  cout << "  - Press 'r/R' to reset" << endl;
  cout << "  - Press 's/S' to save" << endl;
  cout << "  - Press 'ESC' to exit" << endl;

  while (true) {
    imshow("Mosaic Effect Tool", image);
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
      m_blurDegree = std::max(m_blurDegree, 1);
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
