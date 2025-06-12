cmake_minimum_required(VERSION 3.14)
project(MandelbrotViewer CXX)

set(CMAKE_CXX_STANDARD 17)
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot PRIVATE TBB::tbb ${OpenCV_LIBS})
