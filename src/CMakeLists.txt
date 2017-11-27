cmake_minimum_required(VERSION 2.6)
project(clippingSSBDVideos)

set(CMAKE_VERBOSE_MAKEFILE OFF)

# print some settings
# the compiler used for C files
MESSAGE( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )

# some CMAKE information
MESSAGE (STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})

MESSAGE (STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

#OPENCV
include_directories(~/local/include)
link_directories(~/local/lib)

#boost
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_package(OpenCV REQUIRED HINTS ~/local)
MESSAGE( STATUS "OpenCV version: " ${OpenCV_VERSION} )
MESSAGE( STATUS "OpenCV libs: " ${OpenCV_LIBS} )

#C++ AND linker flags
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -pipe -D __STDC_CONSTANT_MACROS -D STD=std -Wall -O3 -DNDEBUG -ggdb ${CMAKE_CXX_FLAGS}")
#list(APPEND CMAKE_CXX_LINK_EXECUTABLE "-pipe -Wall -O3 -ggdb ${CMAKE_CXX_LINK_EXECUTABLE}")

# add the executable for myIDT
add_executable(${PROJECT_NAME} clipping.cpp)
target_link_libraries(${PROJECT_NAME} opencv_core opencv_highgui opencv_video opencv_videoio boost_filesystem boost_system )

add_executable(clip_segment clip_segment.cpp)
target_link_libraries(clip_segment opencv_core opencv_highgui opencv_video opencv_videoio boost_filesystem boost_system )
