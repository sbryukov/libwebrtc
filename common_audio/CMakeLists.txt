
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


include_directories(../ ./include vad vad/include)

set (VAD_SRC 
	vad_core_unittest.cc
	vad_filterbank_unittest.cc
	vad_sp_unittest.cc
	vad_gmm_unittest.cc
	vad_unittest.cc
)

set (SIGNA_PROCESSIG_SRC 
	vad_core_unittest.cc
	vad_filterbank_unittest.cc
	vad_sp_unittest.cc
	vad_gmm_unittest.cc
	vad_unittest.cc
)



add_library (autil audio_util.cc)


#add_library (coa ${COA_SRC})
#target_link_libraries (coa) 

target_link_libraries (autil)