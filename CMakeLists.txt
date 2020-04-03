cmake_minimum_required(VERSION 3.13)
project(opencv_hello_world)

set(CMAKE_CXX_STANDARD 14)
find_package( OpenCV REQUIRED )

add_subdirectory(libs/bpc_prp_opnecv_lib)

include_directories(libs/bpc_prp_opnecv_lib/include)

add_executable(opencv_hello_world src/main.cpp)
target_link_libraries( opencv_hello_world ${OpenCV_LIBS} bpc_prp_opencv_lib)