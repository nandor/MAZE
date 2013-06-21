# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set(SOURCES adler32.c
			compress.c
			crc32.c
			deflate.c
			gzclose.c
			gzlib.c
			gzread.c
			gzwrite.c
			infback.c
			inffast.c
			inflate.c
			inftrees.c
			trees.c
			uncompr.c
			zutil.c)
			
set(HEADERS crc32.h
			deflate.h
			gzguts.h
			inffast.h
			inffixed.h
			inflate.h
			inftrees.h
			trees.h
			zconf.h
			zlib.h
			zutil.h)
			
add_library(zlib STATIC ${SOURCES} ${HEADERS})
