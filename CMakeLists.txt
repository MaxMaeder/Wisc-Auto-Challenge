cmake_minimum_required(VERSION 3.25)
project( CodingChallenge )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( CodingChallenge main.cpp )
target_link_libraries( CodingChallenge ${OpenCV_LIBS} )