cmake_minimum_required (VERSION 2.6)

#Project name
project (WEB_RTC)

#List of IPP libraries

#Common variables

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/lib)

#General include dir
include_directories(AFTER include)

#Dependency libraries
add_subdirectory(modules/audio_coding/codecs/cng "${CMAKE_CURRENT_BINARY_DIR}/modules/audio_coding/codecs/cng")

#Tools

# place project files in folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(TARGET cng PROPERTY FOLDER "libs")

#SET_PROPERTY(TARGET abnf PROPERTY FOLDER "tools")
