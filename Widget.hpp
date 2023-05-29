
#pragma once

#include <cstdint>
#include <utility>

#ifndef VERSION
#define VERSION() 2
#endif

namespace library {
  class Widget;

  // A forward binary-compatible Widget class
  class Widget {
    template <std::uint8_t> struct Impl;
    Impl<0>* pimpl_;

  public:
    Widget();
    Widget(Widget&& other) noexcept;
    Widget(const Widget&);

#if VERSION() == 2
    // This is a new constructor added in V2
    explicit Widget(int data);
#endif

    Widget& operator=(Widget&& other) noexcept;
    Widget& operator=(const Widget&);

    ~Widget();

    void swap(Widget& otherWidget) noexcept;
    friend void swap(Widget& left, Widget& right) noexcept {
      left.swap(right);
    }

    /*virtual*/ void frobnicate(const Widget& otherWidget) const;

#if VERSION() == 2
    void frob();

    /*virtual*/ void frobozzle(int value);
#endif

    // This was a public feature in V1, but it's inaccessible in V2
    // We have to still support it in V2, or else libs linked to V1
    // will not link to V2.
#if VERSION() == 2
  private:
#endif
    struct TagV1 {};
    Widget(TagV1); // Make a V1 widget
  };

  // These members won't ever change, so it's ok to define them inline
  inline Widget::Widget(Widget&& other) noexcept
    : pimpl_(std::exchange(other.pimpl_, nullptr))
  {}

  inline Widget& Widget::operator=(Widget&& other) noexcept {
    pimpl_ = std::exchange(other.pimpl_, nullptr);
    return *this;
  }

  inline void Widget::swap(Widget& other) noexcept {
    std::swap(pimpl_, other.pimpl_);
  }
}
