# code for opengl red book old version code 
# Code samples for the OpenGL v1.1 Programming Guide (Redbook)
# https://www.opengl.org/archives/resources/code/samples/redbook/

PROJECT(OGL-Redbook-old)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#include
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/include)
#lib
if(CMAKE_CL_64)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x64)
else()
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../sdk/lib/x86)
endif()
MESSAGE("${CMAKE_SOURCE_DIR}/../../sdk/lib")
LINK_LIBRARIES(opengl32.lib glu32.lib freeglut.lib)
#output 
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# copy binrary dll 


 
 SET(CMD_LOG "cmds.log")
 add_custom_command(OUTPUT ${CMD_LOG} COMMAND echo "Generation cmd.log file..." 
	COMMENT "this is a test")

add_custom_target(cmd ALL DEPENDS ${TEST_FILE})
# project 
# µÚÒ»ÕÂ
add_executable(ch01-hello src/hello.c)

# ch11
add_executable(ch11-tess src/tess.c)
add_executable(ch11-tesswind src/tesswind.c) 

#ch12
add_executable(ch12-bezcurve src/bezcurve.c)
add_executable(ch12-bezsurf src/bezsurf.c)
add_executable(ch12-bezmesh src/bezmesh.c)
add_executable(ch12-texturesurf src/texturesurf.c)
add_executable(ch12-surfac src/surface.c)
add_executable(ch12-trim src/trim.c)

