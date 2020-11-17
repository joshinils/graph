cmake_minimum_required(VERSION 3.10)
project("graphViz")

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -fpermissive -Wall")
endif(UNIX)

add_executable("${PROJECT_NAME}"
    "main.cpp"
)

add_library(Graph
    "Aether.h"
    "Graph.h"
    "Graph.cpp"
    "Knoten.h"
    "Knoten.cpp"
    "Kante.h"
    "Kante.cpp"
    "Aether.cpp"
    "Index.h"
    "Index.cpp"
    "pge.cpp"
    "olcPixelGameEngine/olcPixelGameEngine.h"
    "plane/Plane.h"
    "plane/Plane.cpp"
)
add_library(Graph::Graph ALIAS Graph)
target_compile_features(Graph PUBLIC cxx_std_17)
target_include_directories(Graph
	PUBLIC
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

include_directories(olcPixelGameEngine)
include_directories(plane)

#target_link_libraries("${PROJECT_NAME}" PUBLIC Graph)

# set the startup project in visual studio, such that it is not ALL_BUILD and annoying
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")
    set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo")
endif(MSVC)

#jnl delete this if not needed, i don't need it for msvc
if(UNIX)
  find_package (PkgConfig REQUIRED)
endif(UNIX)

#OpenGL_GL_PREFERENCE
cmake_policy(SET CMP0072 "NEW")

find_package (OpenGL REQUIRED)
#find_package (GLUT REQUIRED)


if(UNIX)
    include_directories(${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIRS})
endif(UNIX)

file(GLOB SOURCE_FILES
    *.cpp
    *.h
)

if(UNIX)
    target_link_libraries (Graph
    ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GL m dl Xinerama Xrandr Xi Xcursor X11 Xxf86vm pthread png)
    target_link_libraries ("${PROJECT_NAME}" Graph)
endif(UNIX)
