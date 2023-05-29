#include "../Widget.hpp"
#include "WidgetV1.ipp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {
  // This is the virtual table corresponding to the V1 implementation
  // of the Widget type:
  constexpr Widget::Impl<0>::VTable Widget::Impl<0>::vtable_ = {
    (std::uint8_t)0,
    [](const Impl<0>* pimpl) -> void {
      assert(pimpl->vptr_->version_ == 0);
      std::printf("deleting a v1 Widget\n");
      delete pimpl;
    },
    [](const Impl<0>* pimpl, Widget other) -> void {
      assert(pimpl->vptr_->version_ == 0);
      std::printf("frobnicating a v1 Widget\n");
    }
  };

  Widget::Impl<0> Widget::Impl<0>::make() {
    return {&Impl<0>::vtable_};
  }

  Widget::Widget(TagV1)
    : pimpl_(new auto(Impl<0>::make()))
  {}

/////////////////////////////////////////////////////////
// These all now have V2 implementations in WidgetV2.cpp:

  // Widget::Widget()
  //   : pimpl_(new auto(Impl<0>::make()))
  // {}

  // Widget::Widget(const Widget& other)
  //   : pimpl_(new auto(*other.pimpl_))
  // {}

  // Widget::~Widget() {
  //   pimpl_->vptr_->delete_(pimpl_);
  // }

  // Widget& Widget::operator=(const Widget& other)
  // {
  //   // Copy/swap idiom is strongly exception-safe
  //   Widget newWidget(other);
  //   swap(newWidget);
  //   return *this;
  // }

  // // Implementation of a non-virtual:
  // void Widget::frob() {
  //   std::printf("frob a v%d Widget\n", (int)pimpl_->vptr_->version_ + 1);
  // }

  // // Dispatching to a virtual that has been there since v1
  // void Widget::frobnicate(const Widget& otherWidget) const {
  //   pimpl_->vptr_->frobnicate_(pimpl_, otherWidget);
  // }
} // namespace library
