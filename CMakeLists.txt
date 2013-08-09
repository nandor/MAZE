# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)
project(MAZE)

# Find dependencies
find_package(OpenGL)

link_directories("${PROJECT_SOURCE_DIR}/libs")
include_directories("${PROJECT_SOURCE_DIR}/maze")
include_directories("${PROJECT_SOURCE_DIR}/maze/png")
include_directories("${PROJECT_SOURCE_DIR}/maze/zlib")

# Compiler & platform specifics
if (WIN32)
	if (MSVC)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS")
	elseif(MINGW)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99 -march=prescott")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -march=prescott")
	endif(MSVC)
endif (WIN32)

include ("tools/PrecompiledHeader.cmake")

# Recurse into project dirs
add_subdirectory(maze/ogg)
add_subdirectory(maze/png)
add_subdirectory(maze/jpeg)
add_subdirectory(maze/zlib)
add_subdirectory(maze/vorbis)
add_subdirectory(maze)