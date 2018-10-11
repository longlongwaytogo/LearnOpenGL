PROJECT(OGL-Redbook8)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#include
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
#lib
if(CMAKE_CL_64)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x64)
else()
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x86)
endif()
MESSAGE("${CMAKE_SOURCE_DIR}/../../sdk/lib")
LINK_LIBRARIES(opengl32.lib glu32.lib freeglut.lib comm)
#output 
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#set(CMAKE_DEBUG_POSTFIX "_d")
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")  


# copy binrary dll 


 
# SET(CMD_LOG "cmds.log")
# add_custom_command(OUTPUT ${CMD_LOG} COMMAND echo "Generation cmd.log file..." 
#	COMMENT "this is a test")

#add_custom_target(cmd ALL DEPENDS ${TEST_FILE})
# project 

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)


add_library(comm src/comm/gl3w.c src/comm/LoadShaders.cpp src/comm/targa.cpp src/comm/vbm.cpp)
# µÚÒ»ÕÂ
add_executable(triangles src/1/triangles.cpp)
SET_PROPERTY(TARGET triangles PROPERTY FOLDER "ch01")

# ch03 
add_executable(ch03-1-BufferSubData src/3/ch03-1-BufferSubData.cpp)
SET_PROPERTY(TARGET ch03-1-BufferSubData PROPERTY FOLDER "ch03")

add_executable(ch03-2-IndexDraw src/3/ch03-2-IndexDraw.cpp)
SET_PROPERTY(TARGET ch03-2-IndexDraw PROPERTY FOLDER "ch03")

add_executable(ch03-3-Instance-1 src/3/ch03-3-Instance-1.cpp src/3/vbm.h src/3/vbm.cpp)
SET_PROPERTY(TARGET ch03-3-Instance-1 PROPERTY FOLDER "ch03")

add_executable(primitiverestart src/3/ch03_primitive_restart.cpp)
SET_PROPERTY(TARGET primitiverestart PROPERTY FOLDER "ch03")

