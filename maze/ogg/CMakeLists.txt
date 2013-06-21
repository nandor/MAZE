# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set(SOURCES bitwise.c
			framing.c)
			
set(HEADERS ogg.h
			os_types.h)
			
add_library(ogg STATIC ${SOURCES} ${HEADERS})
