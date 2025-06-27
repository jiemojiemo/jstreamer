cmake_minimum_required(VERSION 3.10)
project(jstreamer VERSION 1.0.0 LANGUAGES C CXX)

add_library(jstreamer STATIC
        src/jst_base_element.cpp
)
target_include_directories(jstreamer PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_subdirectory(tests)