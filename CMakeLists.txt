cmake_minimum_required(VERSION 3.21.0)
project(widget LANGUAGES CXX)

add_library(widget src/WidgetV1.cpp src/WidgetV2.cpp)
target_compile_features(widget PRIVATE cxx_std_23)
target_compile_options(widget PRIVATE -fsanitize=address,undefined)
target_link_libraries(widget PRIVATE asan ubsan)

add_library(third_party ThirdParty/src/ThirdParty.cpp)
target_compile_features(third_party PRIVATE cxx_std_23)
target_compile_options(third_party PRIVATE -fsanitize=address,undefined)
target_include_directories(third_party PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(third_party PRIVATE asan ubsan widget)

add_executable(abi_demo main.cpp)
target_compile_features(abi_demo PRIVATE cxx_std_23)
target_compile_options(abi_demo PRIVATE -fsanitize=address,undefined)
target_include_directories(abi_demo PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(abi_demo PRIVATE asan ubsan widget third_party)
