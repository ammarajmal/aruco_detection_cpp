cmake_minimum_required(VERSION 3.16.3)
project(aruco_det)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(aruco_executable aruco.cpp)
target_link_libraries(aruco_executable ${OpenCV_LIBRARIES})






project(detect_marker_oem)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(marker detect_markers_my.cpp)
target_link_libraries(marker ${OpenCV_LIBRARIES})
