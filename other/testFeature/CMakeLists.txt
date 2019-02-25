PROJECT(OGL-TestFeature)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#include
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../RedBook/redbook8/include)
#lib
if(CMAKE_CL_64)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x64)
else()
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x86)
endif()
MESSAGE("${CMAKE_SOURCE_DIR}/../../sdk/lib")
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../RedBook/redbook8/lib/Release)
LINK_LIBRARIES(opengl32.lib glu32.lib glew32s.lib freeglut_static_vs2010.lib comm.lib)
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


# # µÚÒ»ÕÂ
# add_executable(triangles src/1/triangles.cpp)
# SET_PROPERTY(TARGET triangles PROPERTY FOLDER "ch01")
 

add_executable(antiLine antiLine/antiLine.cpp)
SET_PROPERTY(TARGET antiLine PROPERTY FOLDER "testFeature")

add_executable(MSAA-shader antiLine/MSAA.cpp)
SET_PROPERTY(TARGET MSAA-shader PROPERTY FOLDER "testFeature")


MESSAGE( status "coutput:${SHADER_FILE}")
