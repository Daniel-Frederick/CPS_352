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

// Function to reapply all blur effects
void reapplyBlurs() {
  image_unmodified.copyTo(image);
  for (const auto &blur : blurRects) {
    Mat blurred_region;
    GaussianBlur(blur.original, blurred_region,
                 Size(2 * m_blurDegree + 1, 2 * m_blurDegree + 1), 0);
    blurred_region.copyTo(image(blur.rect));
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

        // Apply the current blur
        Mat blurred_region;
        GaussianBlur(blurRect.original, blurred_region,
                     Size(2 * m_blurDegree + 1, 2 * m_blurDegree + 1), 0);
        blurred_region.copyTo((*image)(roi));
      }
    }
    drawing = false;
  }
}

void save() {
  std::filesystem::path filePath(m_imgPath);
  std::string newFilename =
      filePath.stem().string() + "2" + filePath.extension().string();
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
    cout << "Include an image path!" << endl;
    exit(EXIT_FAILURE);
  }

  m_imgPath = argv[1];
  image = imread(m_imgPath);
  image.copyTo(image_org);
  image.copyTo(image_unmodified);

  namedWindow("My Window");
  setMouseCallback("My Window", drawRectangleCallback, &image);

  while (1) {
    imshow("My Window", image);
    char c = waitKey(100);

    switch (c) {
    case 'i':
    case 'I':
      if (m_blurDegree < 5)
        m_blurDegree++;
      else
        m_blurDegree += 5;
      cout << "m_blurDegree increased to " << m_blurDegree << endl;
      reapplyBlurs();
      break;

    case 'd':
    case 'D':
      if (m_blurDegree <= 5 && m_blurDegree > 1)
        m_blurDegree--;
      else
        m_blurDegree -= 5;
      m_blurDegree = std::max(m_blurDegree, 0);
      cout << "m_blurDegree decreased to " << m_blurDegree << endl;
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
