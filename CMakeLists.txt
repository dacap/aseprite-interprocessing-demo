cmake_minimum_required (VERSION 3.10)

project(livelink CXX C)
set(CMAKE_CXX_STANDARD 11)

set(IXWEBSOCKET_INSTALL OFF CACHE BOOL "Install IXWebSocket")
add_subdirectory(IXWebSocket)
target_include_directories(ixwebsocket PUBLIC)

add_executable(livelink main.cpp tigr/tigr.c)
target_link_libraries(livelink ixwebsocket)

if(WIN32)
    target_link_libraries(livelink opengl32 gdi32)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(livelink GLU GL X11)
endif()

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL NAMES GL)
    target_link_libraries(livelink ${COCOA_LIBRARY} ${OPENGL_LIBRARY})
endif()