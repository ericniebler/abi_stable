// This file was compiled against V1 of the Widget library. It was
// linked to the V1 library, but now it will be linked with V2, and
// it should still work!
#define VERSION() 1
#include "../ThirdParty.hpp"

#include <cstdio>

namespace ThirdParty {
  // This library for some reason exposes Widgets publicly
  library::Widget makeV1Widget() {
    return library::Widget(library::Widget::TagV1());
  }

  void consumeWidget(const library::Widget& widget) {
    // This function should work whether it is passed a V1
    // or V2 widget.
    std::printf("Consuming a widget in a third party library\n");
    widget.frobnicate(widget);
    widget.frobnicate(library::Widget());
  }
}
