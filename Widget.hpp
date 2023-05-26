
#pragma once

#include <memory>
#include <cstdint>

namespace library {

  namespace detail {
    namespace v1 {
      struct VTable;
      struct Impl;
      struct Deleter {
        constexpr void operator()(Impl*) const;
      };
    }
    inline namespace v2 {
      struct VTable;
      struct Impl;
      using v1::Deleter;
    }
  }

  // A forward binary-compatible Widget class
  class Widget {
    friend detail::VTable;
    friend detail::Impl;
    // I'm taking a shortcut here, but it's not safe to assume that all
    // stdlibs have the same unique_ptr layout. We would need our own
    // unique_ptr type for use in ABI-stable interfaces.
    std::unique_ptr<detail::Impl, detail::Deleter> pimpl_;
    friend Widget makeV1Widget();
    Widget(detail::Impl*);

  public:
    Widget();
    Widget(Widget&&) noexcept = default;
    Widget(const Widget&);

    explicit Widget(int data);

    Widget& operator=(Widget&&) noexcept = default;
    Widget& operator=(const Widget&);

    ~Widget();

    void swap(Widget& otherWidget) noexcept {
      std::swap(pimpl_, otherWidget.pimpl_);
    }

    friend void swap(Widget& left, Widget& right) noexcept {
      left.swap(right);
    }

    void frob();

    /*virtual*/ void frobnicate(Widget otherWidget);

    /*virtual*/ void frobozzle(int value);
  };

  // Imagine this is exported from some third-party library
  // built with the v1 Widget type and linked with this code:
  Widget makeV1Widget();

}
