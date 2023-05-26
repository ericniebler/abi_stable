#include "Widget.hpp"

#include <cstdint>
#include <cstdio>
#include <cassert>

namespace library {

  /*
  / The Widget type from v1
  /

  // A forward binary-compatible Widget class
  class Widget {
    struct VTable;
    struct Impl;
    struct Deleter {
      void operator()(Impl*) const;
    };
    // I'm taking a shortcut here, but it's not safe to assume that all
    // stdlibs have the same unique_ptr layout. We would need our own
    // unique_ptr type for use in ABI-stable interfaces.
    std::unique_ptr<Impl, Deleter> pimpl_;
    static const std::uint8_t s_currentVersion;

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
  };
  */

  namespace detail {
    namespace v1 {
      struct VTable {
        // A class version number is first, and is incremented with every
        // ABI-breaking change to the type.
        std::uint8_t const version_;

        // pointers to implementation functions go here:
        using deleteFun_t = void (Impl* pimpl);
        static deleteFun_t deleteImpl_v1;
        deleteFun_t *const delete_;

        // pointers to implementation functions go here:
        using frobnicateFun_t = void (Impl* pimpl, Widget otherWidget);
        static frobnicateFun_t frobnicateImpl_v1;
        frobnicateFun_t *const frobnicate_;

        static constexpr VTable make() {
          return {1, &deleteImpl_v1, &frobnicateImpl_v1};
        }
      };
      inline constexpr auto vtable = VTable::make();

      struct Impl {
        static constexpr Impl make() { return Impl{&vtable}; }

        // vtable ptr is first:
        // an alternate design puts the vtable ptr directly in Widget along side
        // the Impl ptr. That makes Widgets bigger, but saves indirections.
        VTable const* const vptr_;
      };

      void VTable::frobnicateImpl_v1(Impl* pimpl, Widget otherWidget) {
        assert(pimpl->vptr_->version_ == 1);
        std::printf("Ye olde frobnicate implementation\n");
      }

      void VTable::deleteImpl_v1(Impl* pimpl) {
        assert(pimpl->vptr_->version_ == 1);
        std::printf("Ye olde delete implementation\n");
        delete pimpl;
      }
    }
  }

  Widget makeV1Widget() {
    return Widget(
      reinterpret_cast<detail::v2::Impl*>(new auto(detail::v1::Impl::make())));
  }
}