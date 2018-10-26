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
add_library(vermilion   src/vermilion/loadtexture.cpp src/vermilion/vdds.cpp)
# # 第一章
# add_executable(triangles src/1/triangles.cpp)
# SET_PROPERTY(TARGET triangles PROPERTY FOLDER "ch01")

# # ch03 
# add_executable(ch03-1-BufferSubData src/3/ch03-1-BufferSubData.cpp)
# SET_PROPERTY(TARGET ch03-1-BufferSubData PROPERTY FOLDER "ch03")

# add_executable(ch03-2-IndexDraw src/3/ch03-2-IndexDraw.cpp)
# SET_PROPERTY(TARGET ch03-2-IndexDraw PROPERTY FOLDER "ch03")

# add_executable(ch03-3-Instance-1 src/3/ch03-3-Instance-1.cpp src/3/vbm.h src/3/vbm.cpp)
# SET_PROPERTY(TARGET ch03-3-Instance-1 PROPERTY FOLDER "ch03")

# #  对上一个案例的修改，使用
# add_executable(ch03-4-Instance-2 src/3/ch03-4-Instance-2.cpp src/3/vbm.h src/3/vbm.cpp)
# SET_PROPERTY(TARGET ch03-4-Instance-2 PROPERTY FOLDER "ch03")

# add_executable(ch03-5-Primitiverestart src/3/ch03_primitive_restart.cpp)
# SET_PROPERTY(TARGET ch03-5-Primitiverestart PROPERTY FOLDER "ch03")


# #ch04 
# add_executable(ch04-1-DrawColor src/4/Triangle.cpp)
# SET_PROPERTY(TARGET ch04-1-DrawColor PROPERTY FOLDER "ch04")

# #ch05
# add_executable(ch05-TransformFeedback src/5/ch05-transformFeedback.cpp src/5/vbm.h src/5/vbm.cpp)
# SET_PROPERTY(TARGET ch05-TransformFeedback PROPERTY FOLDER "ch05")

# #ch06
# add_executable(ch06-1-Texture-Data src/6/TextureData.cpp)
# target_link_libraries(ch06-1-Texture-Data vermilion)
# SET_PROPERTY(TARGET ch06-1-Texture-Data PROPERTY FOLDER "ch06")

# add_executable(ch06-1-Texture-Data-2 src/6/TextureData2.cpp)
# target_link_libraries(ch06-1-Texture-Data-2 vermilion)
# SET_PROPERTY(TARGET ch06-1-Texture-Data-2 PROPERTY FOLDER "ch06")

# add_executable(ch06-2-Texture-Wrapping src/6/texture_wrapping.cpp)
# target_link_libraries(ch06-2-Texture-Wrapping vermilion)
# SET_PROPERTY(TARGET ch06-2-Texture-Wrapping PROPERTY FOLDER "ch06")

# add_executable(ch06-3-Load-Texture src/6/LoadTexture.cpp)
# target_link_libraries(ch06-3-Load-Texture vermilion)
# SET_PROPERTY(TARGET ch06-3-Load-Texture PROPERTY FOLDER "ch06")

# add_executable(ch06-4-Texture-Unit src/6/TextureUnit.cpp)
# target_link_libraries(ch06-4-Texture-Unit vermilion)
# SET_PROPERTY(TARGET ch06-4-Texture-Unit PROPERTY FOLDER "ch06")

# add_executable(ch06-5-CubeMap src/6/CubeMap.cpp)
# target_link_libraries(ch06-5-CubeMap vermilion)
# SET_PROPERTY(TARGET ch06-5-CubeMap PROPERTY FOLDER "ch06")

# add_executable(ch06-6-VolumeTexture src/6/VolumeTexture.cpp)
# target_link_libraries(ch06-6-VolumeTexture vermilion)
# SET_PROPERTY(TARGET ch06-6-VolumeTexture PROPERTY FOLDER "ch06")

# add_executable(ch06-7-MipmapFilter src/6/Mipmap_Filter.cpp)
# target_link_libraries(ch06-7-MipmapFilter vermilion)
# SET_PROPERTY(TARGET ch06-7-MipmapFilter PROPERTY FOLDER "ch06")

# add_executable(ch06-8-Mipmap src/6/Mipmap.cpp)
# target_link_libraries(ch06-8-Mipmap vermilion)
# SET_PROPERTY(TARGET ch06-8-Mipmap PROPERTY FOLDER "ch06")

# add_executable(ch06-9-PointSprite-1 src/6/PointSprite.cpp)
# target_link_libraries(ch06-9-PointSprite-1 vermilion)
# SET_PROPERTY(TARGET ch06-9-PointSprite-1  PROPERTY FOLDER "ch06")

# add_executable(ch06-10-PointSprite-2 src/6/PointSprite2.cpp)
# target_link_libraries(ch06-10-PointSprite-2 vermilion)
# SET_PROPERTY(TARGET ch06-10-PointSprite-2  PROPERTY FOLDER "ch06")

# # ch07 light
# add_executable(ch07-1-LightShader src/7/LightShader.cpp)
# target_link_libraries(ch07-1-LightShader vermilion)
# SET_PROPERTY(TARGET ch07-1-LightShader PROPERTY FOLDER "ch07")

# add_executable(ch07-2-LightShader-phong src/7/LightShader_phong.cpp)
# target_link_libraries(ch07-2-LightShader-phong vermilion)
# SET_PROPERTY(TARGET ch07-2-LightShader-phong PROPERTY FOLDER "ch07")

# add_executable(ch07-3-ShadowMap src/7/Shadowmap.cpp)
# target_link_libraries(ch07-3-ShadowMap vermilion)
# SET_PROPERTY(TARGET ch07-3-ShadowMap PROPERTY FOLDER "ch07")


# #ch09
# add_executable(teapot src/9/teapot.cpp)
# SET_PROPERTY(TARGET teapot PROPERTY FOLDER "ch09")

# add_executable(simple src/9/simple.cpp)
# SET_PROPERTY(TARGET simple PROPERTY FOLDER  "ch09")



# #ch10
# add_executable(ch10-1-ViewportArray src/10/ViewportArray.cpp)
# SET_PROPERTY(TARGET ch10-1-ViewportArray PROPERTY FOLDER "ch10")

# add_executable(ch10-2-Draw_xfb src/10/Draw_xfb.cpp)
# SET_PROPERTY(TARGET ch10-2-Draw_xfb PROPERTY FOLDER "ch10")


# add_executable(ch10-3-Fur_Render src/10/fur_render.cpp)
# SET_PROPERTY(TARGET ch10-3-Fur_Render PROPERTY FOLDER "ch10")

#ch11
add_executable(ch11-1-doubleWrite src/11/DoubleWrite.cpp src/11/vbm.cpp src/11/vbm.h)
#target_link_libraries(ch11-1-doubleWrite vermilion)
SET_PROPERTY(TARGET ch11-1-doubleWrite PROPERTY FOLDER "ch11")

add_executable(ch11-2-overdrawcount src/11/overdrawcount.cpp src/11/vbm.cpp src/11/vbm.h)
SET_PROPERTY(TARGET ch11-2-overdrawcount PROPERTY FOLDER "ch11")

SET(SHADER_FILE  bin/Media/Shaders/11/OIT_blit.fs.glsl
bin/Media/Shaders/11/OIT_blit.vs.glsl
bin/Media/Shaders/11/OIT.fs.glsl
bin/Media/Shaders/11/OIT.vs.glsl)

add_executable(ch11-3-OrderIndependentTransparency  src/11/OIT.cpp src/11/vbm.cpp src/11/vbm.h ${SHADER_FILE})
SET_PROPERTY(TARGET ch11-3-OrderIndependentTransparency PROPERTY FOLDER "ch11")

 
#ch12

SET(SHADER_FILE bin/Media/shaders/12/simpleComputeShader.cp.glsl  
bin/Media/shaders/12/simpleComputeShader.vs.glsl
bin/Media/shaders/12/simpleComputeShader.fs.glsl)

add_executable(ch12-1-simpleComputeShader  src/12/simpleComputeShader.cpp ${SHADER_FILE})
SET_PROPERTY(TARGET ch12-1-simpleComputeShader PROPERTY FOLDER "ch12")


SET(SHADER_FILE bin/Media/shaders/12/particlesimulator.vs.glsl
 bin/Media/shaders/12/particlesimulator.fs.glsl
 bin/Media/shaders/12/particlesimulator.cp.glsl)
add_executable(ch12-2-particlesimulator  src/12/particlesimulator.cpp   ${SHADER_FILE})
SET_PROPERTY(TARGET ch12-2-particlesimulator PROPERTY FOLDER "ch12")

MESSAGE( status "coutput:${SHADER_FILE}")
