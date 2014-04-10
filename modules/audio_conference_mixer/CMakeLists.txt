
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


include_directories(../ ./source ./interface)

set (ACONFMIX_SRC 
	source/audio_frame_manipulator.cc
	source/time_scheduler.cc
	source/level_indicator.cc
	source/audio_conference_mixer_impl.cc
	)



add_library (aconfmix ${ACONFMIX_SRC})
target_link_libraries (aconfmix)