// This is a third-party library that was compiled against V1 of Widget

#pragma once

#include "Widget.hpp"

namespace ThirdParty {

  // Imagine this is exported from some third-party library
  // built with the v1 Widget type and linked with this code:
  library::Widget makeV1Widget();
  void consumeWidget(const library::Widget&);

}
