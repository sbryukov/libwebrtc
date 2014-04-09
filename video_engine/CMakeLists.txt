
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

MESSAGE (STATUS "configuring video_engine !!!")

file(GLOB video_engine_src
    "*.h"
    "*.cc"
)


include_directories( ../ ./include)


add_library (vie ${video_engine_src})


target_link_libraries (vie) 