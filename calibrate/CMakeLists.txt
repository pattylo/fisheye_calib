cmake_minimum_required(VERSION 3.16)
project(fisheye_calibrate)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++17)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages



find_package(catkin REQUIRED COMPONENTS
  nodelet
  roscpp
  std_msgs
  roslaunch
  geometry_msgs
  mavros_msgs
  cv_bridge
  image_transport
  sensor_msgs
  message_generation
  genmsg
  visualization_msgs
  tf
)

# The Usuals
find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Sophus REQUIRED)


roslaunch_add_file_check(launch)

add_message_files(
  FILES
)

generate_messages(DEPENDENCIES std_msgs sensor_msgs)

catkin_package(
  INCLUDE_DIRS include

  CATKIN_DEPENDS 
  geometry_msgs 
  mavros_msgs 
  roscpp
  std_msgs
  message_generation
)

set(Sophus_LIBRARIES libSophus.so)


###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${Sophus_INCLUDE_DIRS}
)


############################################################################

add_executable(collect
  src/collect.cpp
)

target_link_libraries(collect
  ${catkin_LIBRARIES}
)

############################################################################

add_executable(calibrate
  src/calibrate.cpp
)

target_link_libraries(calibrate
  ${catkin_LIBRARIES}
)
