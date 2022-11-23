cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# This is the name of the project
project(CompleteRemoteRobot2022 LANGUAGES CXX)

set( CMAKE_VERBOSE_MAKEFILE on )

#Obtain the openCV package 
set(OpenCV_DIR /rpi_sysroot/usr/usr/lib/arm-linux-gnueabihf/cmake/opencv4/)
find_package( OpenCV REQUIRED)
#find_package( OpenCV REQUIRED PATH /rpi_sysroot/usr/usr/lib/arm-linux-gnueabihf/cmake/opencv4/OpenCVConfig.cmake )
#find_library( libmmal REQUIRED)

# This defines system level include directories, where headers for system libraries are defined.
include_directories(SYSTEM /rpi_sysroot/usr/local/include /rpi_sysroot/usr/include /rpi_sysroot/opt/vc/include)

# This provides additional compile options.
add_definitions(-Wall -g -O0 -pg -L/rpi_sysroot/usr/local/lib -lwiringPi -lpthread -std=c++11)

# This identifies the source code files that are relevant to the project.
file(GLOB SOURCES "*.cpp")



# Find the doxygen tool
find_package(Doxygen)

include(UseDoxygenDoc)

add_doxygen_doc(
  BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/../docs/
  DOXY_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in
  TARGET_NAME
    docs
  COMMENT
    "HTML documentation"
  )

link_directories( /rpi_sysroot/opt/vc/lib /rpi_sysroot/usr/lib 
/rpi_sysroot/usr/lib/arm-linux-gnueabihf ) # for specific path

# This defines that an executable is to be created.
add_executable(CompleteRemoteRobot2022 ${SOURCES})

# This defines the libraries which are needed by this project for it to link properly.
target_link_libraries(CompleteRemoteRobot2022   ${OpenCV_LIBS} )
target_link_libraries(CompleteRemoteRobot2022    wiringPi)
target_link_libraries(CompleteRemoteRobot2022   pthread )
target_link_libraries(CompleteRemoteRobot2022   rt )
target_link_libraries(CompleteRemoteRobot2022   mmal_core )
target_link_libraries(CompleteRemoteRobot2022   mmal_util )
target_link_libraries(CompleteRemoteRobot2022   mmal_vc_client )
target_link_libraries(CompleteRemoteRobot2022   vcos  )
target_link_libraries(CompleteRemoteRobot2022   bcm_host )
target_link_libraries(CompleteRemoteRobot2022   vchiq_arm )
target_link_libraries(CompleteRemoteRobot2022   vcsm )
target_link_libraries(CompleteRemoteRobot2022 /rpi_sysroot/usr/lib/arm-linux-gnueabihf/blas/libblas.so.3 )
target_link_libraries(CompleteRemoteRobot2022 /rpi_sysroot/usr/lib/arm-linux-gnueabihf/lapack/liblapack.so.3)



