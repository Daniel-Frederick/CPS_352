#include "include/events.hpp"

namespace Events_NS {

Events_NS::Events(const std::string window) : m_WINDOW(window) {}
void Events::onMouseClick(int event, int x, int y, int flags, void *userData) {
  if (event == cv::EVENT_LBUTTONDBLCLK) {
    std::cout << "left Press" << std::endl;
    // cv::rectangle();
  }
}
} // namespace Events_NS
