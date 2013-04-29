# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set (SOURCES main.cc)

set (HEADERS )

set (LIBS opengl32)

add_executable(survey WIN32 ${SOURCES} ${HEADERS})
target_link_libraries(survey ${LIBS})
