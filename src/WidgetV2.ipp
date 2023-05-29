#pragma once

#include "../Widget.hpp"
#include "WidgetV1.ipp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {
  template <>
  struct Widget::Impl<1> : Widget::Impl<0> {
    struct VTable : Widget::Impl<0>::VTable {
      void (*const frobozzle_)(Impl<1>*, int); // ADDED IN V2!
    };
    static const VTable vtable_;
    static Impl<1> make(int data = 0);
    static Impl<1> copy(const Impl<0>&);

    // Data members follow:
    int data_; // ADDED IN V2!
  };
} // namespace library
