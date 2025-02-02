#pragma once
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Events_NS {
class Events {
public:
  Events();

  static void drawRectangleCallback(int event, int x, int y, int flags,
                                    void *userData);

private:
};
} // namespace Events_NS
