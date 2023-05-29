#pragma once

#include "../Widget.hpp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {
  template <>
  struct Widget::Impl<0> {
    struct VTable {
      // A class version number is first, and is incremented with every
      // ABI-breaking change to the type.
      std::uint8_t const version_;

      // pointers to implementation functions go here:
      void (*const delete_)(const Impl<0>*);
      void (*const frobnicate_)(const Impl<0>*, Widget);
    };
    static const VTable vtable_;
    static Impl<0> make();

    // The virtual table ptr is first:
    const VTable *const vptr_;
  };
} // namespace library
