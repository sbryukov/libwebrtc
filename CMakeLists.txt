
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of project
PROJECT(LIBWEBRTC)

# Compile with gcc c89 standard
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_SYSTEM_NAME Linux)
    MESSAGE(STATUS "GCC detected - Adding compiler flags")
    SET(CMAKE_C_FLAGS "-pthread -ggdb -Wextra -Wall -pedantic -std=c89")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(WIN32)
    SET(CMAKE_C_FLAGS "")
ENDIF(WIN32)


SET(CMAKE_MODULE_PATH "${LIBWEBRTC_BINARY_DIR}/modules")
SET(CMAKE_SYSTEM_WRAPPERS_PATH "${LIBWEBRTC_BINARY_DIR}/system_wrappers")
SET(CMAKE_COMMON_AUDIO_PATH "${LIBWEBRTC_BINARY_DIR}/common_audio")
SET(CMAKE_COMMON_VIDEO_PATH "${LIBWEBRTC_BINARY_DIR}/common_video")
SET(CMAKE_VIDEO_ENGINE_PATH "${LIBWEBRTC_BINARY_DIR}/video_engine")
SET(CMAKE_VOE_ENGINE_PATH "${LIBWEBRTC_BINARY_DIR}/voice_engine")
SET(CMAKE_AUDIO_CONF_MIX_PATH "${CMAKE_MODULE_PATH}/audio_conference_mixer")


MESSAGE (STATUS "root path: ${LIBWEBRTC_BINARY_DIR} !!!")

add_subdirectory("${CMAKE_VIDEO_ENGINE_PATH}")
add_subdirectory("${CMAKE_COMMON_AUDIO_PATH}")
add_subdirectory("${CMAKE_AUDIO_CONF_MIX_PATH}")
add_subdirectory("${CMAKE_SYSTEM_WRAPPERS_PATH}")

#add_subdirectory("${CMAKE_VOE_ENGINE_PATH}")

link_directories("${CMAKE_VIDEO_ENGINE_PATH} 
				  ${CMAKE_COMMON_AUDIO_PATH}  
				  ${CMAKE_VOE_ENGINE_PATH} 
				  ${CMAKE_AUDIO_CONF_MIX_PATH} ")


add_library (webrtc_all)
target_link_libraries (webrtc_all vie system_wrappers) 
