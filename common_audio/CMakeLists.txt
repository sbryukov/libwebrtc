
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


include_directories(../ ./include )

set (AUDIO_UTIL_SRC 
	
)



add_library (autil audio_util.cc)


#add_library (coa ${COA_SRC})
#target_link_libraries (coa) 

target_link_libraries (autil)