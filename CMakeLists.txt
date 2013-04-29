# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)
project(MAZE)

# Finde dependencies
find_package(OpenGL)

# Recurse into project dirs
add_subdirectory(maze)
add_subdirectory(tools/survey)
