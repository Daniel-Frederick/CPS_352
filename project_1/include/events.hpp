#pragma once

#include "opencv2/core.hpp"
namespace Events_NS {
class Events {
public:
  Events(const string window);

  void onMouseClick(int event, int x, int y, int flags, void *userData);

private:
  const std::string m_WINDOW;
};
} // namespace Events_NS
