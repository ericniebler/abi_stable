ABI-Stable Class Example
========================

This repo contains some code that demonstrates how to design and implement
a C++ class type that is ABI-stable. In other words, code that was compiled
and linked with V1 of a library can be linked with V2 _without recompilation_.
This only works if the layout of types is unchanged.

But that doesn't mean you cannot evolve the types. It is possible to hide
the changes behind layers of indirection. This code shows how.

Contents
--------

* [`Widget.hpp`](Widget.hpp): The header for an ABI-stable Widget type
    that changed from version 1 to version 2.
* [`src/WidgetV1.*pp`](src/WidgetV1.ipp): The implementation of V1 of
    the widget type.
* [`src/WidgetV2.*pp`](src/WidgetV2.ipp): The implementation of V2 of
    the widget type.
* [`ThirdParty/ThirdParty.hpp`](ThirdParty.hpp): The header
    for a third-party library that was compiled and linked against V1 of the
    widget library, but that will now be linked against V2.
* [`ThirdParty/src/ThirdParty.cpp`](ThirdParty.hpp): The
    implementation of a third-party library that was compiled against V1 of
    the widget library, but that will now be linked against V2.
