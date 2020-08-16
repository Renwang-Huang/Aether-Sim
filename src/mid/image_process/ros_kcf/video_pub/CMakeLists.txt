cmake_minimum_required(VERSION 2.8.3)
project(video_pub)

#需要opencv3.4.4 所以需要重新编译opencv以及cv_bridge
set(OpenCV_DIR "/home/jcuav/tools/opencv/build")
set(cv_bridge_DIR /home/jcuav/tools/cv_bridge_ws/devel/share/cv_bridge/cmake)   
find_package(OpenCV REQUIRED)

find_package(catkin REQUIRED COMPONENTS
 
  geometry_msgs
  roscpp
  rospy
  std_msgs
   cv_bridge
  image_transport
  rospy
  sensor_msgs
)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${video_pub_SOURCE_DIR}/include/video_pub
)



add_executable(
	video_pub 
	src/video_pub.cpp 
	)

## Specify libraries to link a library or executable target against
target_link_libraries(video_pub
 ${catkin_LIBRARIES}
 ${OpenCV_LIBRARIES}
)
