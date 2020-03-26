cmake_minimum_required(VERSION 3.13)
project(bpc_prp_opencv_lib)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories(include)

add_library(bpc_prp_opencv_lib src/ImageProcessor.cpp)
target_link_libraries( bpc_prp_opencv_lib ${OpenCV_LIBS} )