#include "../Widget.hpp"
#include "WidgetV2.ipp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {
  // This is the virtual table corresponding to the V1 implementation
  // of the Widget type:
  constexpr Widget::Impl<1>::VTable Widget::Impl<1>::vtable_ = {
    {
      (std::uint8_t)1,
      [](const Impl<0>* pimpl) -> void {
        assert(pimpl->vptr_->version_ == 1);
        std::printf("deleting a v2 Widget\n");
        delete static_cast<const Impl<1>*>(pimpl);
      },
      [](const Impl<0>* pimpl, Widget other) -> void {
        assert(pimpl->vptr_->version_ == 1);
        std::printf(
          "frobnicating a v2 Widget with a v%d Widget\n",
          (int)other.pimpl_->vptr_->version_ + 1);
      }
    },
    [](Impl<1>* pimpl, int v) -> void {
      std::printf("frobozzle a v2 Widget\n");
    }
  };

  Widget::Impl<1> Widget::Impl<1>::make(int data) {
    return {{&Impl<1>::vtable_}, data};
  }

  Widget::Impl<1> Widget::Impl<1>::copy(const Impl<0>& other) {
    // If `other` is a V2 widget, we should copy the extra member;
    // otherwise, it's not there so we can't access it.
    if (other.vptr_->version_ == 1) {
      return make(static_cast<const Impl<1>&>(other).data_);
    }
    return make();
  }

  Widget::Widget()
    : pimpl_(new auto(Impl<1>::make()))
  {}

  Widget::Widget(const Widget& other)
    : pimpl_(new auto(Impl<1>::copy(*other.pimpl_)))
  {}

  Widget::~Widget() {
    pimpl_->vptr_->delete_(pimpl_);
  }

  Widget& Widget::operator=(const Widget& other)
  {
    // Copy/swap idiom is strongly exception-safe
    Widget newWidget(other);
    swap(newWidget);
    return *this;
  }

  Widget::Widget(int data)
    : pimpl_(new auto(Impl<1>::make(data)))
  {}

  // Implementation of a non-virtual:
  void Widget::frob() {
    std::printf("frob a v%d Widget\n", (int)pimpl_->vptr_->version_ + 1);
  }

  // Dispatching to a virtual that has been there since v1
  void Widget::frobnicate(const Widget& otherWidget) const {
    pimpl_->vptr_->frobnicate_(pimpl_, otherWidget);
  }

  // Dispatching to a virtual that was added in v2
  void Widget::frobozzle(int value) {
    if (pimpl_->vptr_->version_ == 1) {
      auto vptr = static_cast<const Impl<1>::VTable*>(pimpl_->vptr_);
      vptr->frobozzle_(static_cast<Impl<1>*>(pimpl_), value);
    }
    else {
      // use fallback frobozzle action
      std::printf("In fallback frobozzle implementation for v1 Widgets.\n");
    }
  }
} // namespace library

