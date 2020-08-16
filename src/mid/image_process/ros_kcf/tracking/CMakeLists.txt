cmake_minimum_required(VERSION 2.8.3)
project(tracking)
find_package(catkin REQUIRED)
set(OpenCV_DIR "/home/jcuav/tools/opencv/build")    
#需要opencv3.4.4 所以需要重新编译opencv以及cv_bridge
set(cv_bridge_DIR /home/jcuav/tools/cv_bridge_ws/devel/share/cv_bridge/cmake)
find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  std_msgs
   cv_bridge
  image_transport
  darknet_ros_msgs
  sensor_msgs
)

if(NOT WIN32)
  ADD_DEFINITIONS("-std=c++11")
endif(NOT WIN32)

add_library(kcf_tracker STATIC
 include/fhog.cpp
 include/kcftracker.cpp)

include_directories("/usr/include/eigen3")
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${tracking_SOURCE_DIR} include/
)
target_link_libraries(kcf_tracker ${OpenCV_LIBRARIES})

target_include_directories(kcf_tracker PRIVATE include)


generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
)


add_executable(tracking_test
  src/tracking_main.cpp
  src/tracking_test.cpp
 )

target_link_libraries(tracking_test
  kcf_tracker
  ${OpenCV_LIBS}
  ${catkin_LIBRARIES})

target_include_directories(tracking_test
  PUBLIC
  ${catkin_INCLUDE_DIRS})


catkin_package()

add_dependencies(tracking_test ${catkin_EXPORTED_TARGETS})

