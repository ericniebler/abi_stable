#include "Widget.hpp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {

  namespace detail {
    namespace v1 {
      struct VTable {
        // A class version number is first, and is incremented with every
        // ABI-breaking change to the type.
        std::uint8_t const version_;

        // pointers to implementation functions go here:
        using deleteFun_t = void (Impl* pimpl);
        static deleteFun_t deleteImpl_v2;
        deleteFun_t *const delete_;

        // pointers to implementation functions go here:
        using frobnicateFun_t = void (Impl* pimpl, Widget otherWidget);
        static frobnicateFun_t frobnicateImpl_v2;
        frobnicateFun_t *const frobnicate_;

        static constexpr VTable make() {
          return {2, &deleteImpl_v2, &frobnicateImpl_v2};
        }
      };
      inline constexpr auto vtable = VTable::make();
    }

    inline namespace v2 {
      struct VTable : v1::VTable {
        using frobozzleFun_t = void (Impl* pimpl, int);
        static frobozzleFun_t frobozzleImpl_v2;
        frobozzleFun_t *const frobozzle_; // ADDED IN V2!

        static constexpr VTable make() {
          return {v1::vtable, &frobozzleImpl_v2};
        }
      };
      inline constexpr auto vtable = VTable::make();
    }

    namespace v1 {
      // Preserve the old V1 impl layout
      struct Impl {
        static constexpr Impl make () {
          return {&v2::vtable};
        }
        // vtable ptr is first:
        // an alternate design puts the vtable ptr directly in Widget along side
        // the Impl ptr. That makes Widgets bigger, but saves indirections.
        VTable const*const vptr_;
      };
    }

    inline namespace v2 {
      // Extend the v1 layout with v2 bits
      struct Impl : v1::Impl {
        static constexpr Impl make(int data = 0) {
          return{v1::Impl::make(), data};
        }

        static constexpr Impl copy(const Impl& otherImpl) {
          // Check version number before accessing data that might not be there!
          return make(otherImpl.vptr_->version_ == 1 ? 0 : otherImpl.data_);
        }

        // Data members follow:
        int data_; // ADDED IN V2!
      };

      void VTable::frobozzleImpl_v2(Impl* pimpl, int) {
        assert(pimpl->vptr_->version_ == 2);
        // Do frobozzle action
        std::printf("In new frobozzle implementation for v2 Widgets.\n");
      }
    }

    namespace v1 {
      constexpr void Deleter::operator()(Impl* pimpl) const {
        pimpl->vptr_->delete_(pimpl);
      }

      void VTable::deleteImpl_v2(Impl* pimpl) {
        assert(pimpl->vptr_->version_ == 2);
        std::printf("New fangled delete of version %d\n", (int)pimpl->vptr_->version_);
        delete static_cast<v2::Impl*>(pimpl);
      }

      void VTable::frobnicateImpl_v2(Impl* pimpl, Widget otherWidget) {
        assert(pimpl->vptr_->version_ == 2);
        // do the frobnicate action for v2 of the type
        std::printf("frobnicating a v2 Widget: data = %d\n", static_cast<v2::Impl*>(pimpl)->data_);
      }
    }
  }

  Widget::Widget()
    : pimpl_(new auto(detail::Impl::make()), {})
  {}

  Widget::Widget(detail::Impl* pimpl)
    : pimpl_(pimpl, {})
  {
    std::printf("making version %d\n", (int)pimpl->vptr_->version_);
  }

  Widget::Widget(const Widget& otherWidget)
    : pimpl_(new auto(detail::Impl::copy(*otherWidget.pimpl_)), {})
  {}

  Widget& Widget::operator=(const Widget& otherWidget)
  {
    // Copy/swap idiom is strongly exception-safe
    Widget newWidget(otherWidget);
    swap(newWidget);
    return *this;
  }

  Widget::~Widget() = default;

  Widget::Widget(int data)
    : pimpl_(new auto(detail::Impl::make(data)), {})
  {}

  // Implementation of a non-virtual:
  void Widget::frob() {
    // Select the proper implementation for the Widget version
    if (pimpl_->vptr_->version_ == 1) {
      // fall-back implementation for the older Widget type
      std::printf("frob a v1 Widget\n");
    }
    else {
      // do the frob action for v2 of the type
      std::printf("frob a v2 Widget\n");
    }
  }

  // Dispatching to a virtual that has been there since v1
  void Widget::frobnicate(Widget otherWidget) {
    pimpl_->vptr_->frobnicate_(pimpl_.get(), otherWidget);
  }

  // Dispatching to a virtual that was added in v2
  void Widget::frobozzle(int value) {
    if (pimpl_->vptr_->version_ == 1) {
      // use fallback frobozzle action
      std::printf("In fallback frobozzle implementation for v1 Widgets.\n");
    }
    else {
      static_cast<detail::v2::VTable const*>(pimpl_->vptr_)->frobozzle_(pimpl_.get(), value);
    }
  }
}
