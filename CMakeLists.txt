cmake_minimum_required (VERSION 3.10)

project(livelink CXX)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(IXWebSocket)
target_include_directories(ixwebsocket PUBLIC)

add_executable(livelink main.cpp tigr/tigr.c)
target_link_libraries(livelink ixwebsocket)

if(WIN32)
    target_link_libraries(livelink opengl32 gdi32)
endif()

if(UNIX)
    ## untested ##
    target_link_libraries(livelink GLU GL X11)
endif()

if(APPLE)
    ## untested ##
    target_link_libraries(livelink OpenGL Cocoa)
endif()