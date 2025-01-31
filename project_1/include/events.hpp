#pragma once
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>       // class structure
#include <opencv2/highgui/highgui.hpp> // provide input/output functions
#include <opencv2/imgproc/imgproc.hpp> // provide image processing functions

namespace Events_NS {
class Events {
public:
  Events();

  static void onMouseClick(int event, int x, int y, int flags, void *userData);

private:
};
} // namespace Events_NS
