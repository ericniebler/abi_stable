#include "Widget.hpp"

int main() {
  library::Widget v2_widget(42);
  library::Widget v1_widget = library::makeV1Widget();

  v1_widget.frob();
  v2_widget.frob();

  v2_widget.frobnicate(v1_widget);
  v2_widget.frobnicate(v2_widget);

  v1_widget.frobnicate(v1_widget);
  v1_widget.frobnicate(v2_widget);

  v1_widget.frobozzle(42);
  v2_widget.frobozzle(42);
}
