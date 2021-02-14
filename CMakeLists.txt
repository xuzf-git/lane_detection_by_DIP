cmake_minimum_required(VERSION 3.17)
project(lane_detection)

set(CMAKE_CXX_STANDARD 14)

set(OpenCV_DIR D:\\Envs\\opencv\\build\\x64\\mingw\\install)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(lane_detection main.cpp include/Img.hpp include/Kernel.h source/Kernel.cpp include/Edge.h source/Edge.cpp source/Hough.cpp include/Hough.h include/SaveResult.h source/SaveResult.cpp)

target_link_libraries(lane_detection ${OpenCV_LIBS})