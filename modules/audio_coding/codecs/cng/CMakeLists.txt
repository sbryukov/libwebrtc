#CNG.LIB builder

cmake_minimum_required (VERSION 2.6)

#add_definitions(-DWIN32 -D_CONSOLE -D__USE_MPI__ -DMPICH_SKIP_MPICXX)

include_directories(AFTER 
../../../../
../include
)

link_directories(
#
)

#sources of the speakerprocessing

set (LIB_SOURCES
webrtc_cng.c
cng_helpfuns.c
cng_helpfuns.h
)

add_library (cng STATIC ${LIB_SOURCES})
target_link_libraries(cng)