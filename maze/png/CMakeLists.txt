# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set(SOURCES png.c
			pngerror.c
			pngget.c
			pngmem.c
			pngpread.c
			pngread.c
			pngrio.c
			pngrtran.c
			pngrutil.c
			pngset.c
			pngtest.c
			pngtrans.c
			pngwio.c
			pngwrite.c
			pngwtran.c
			pngwutil.c)
			
set(HEADERS png.h
			pngconf.h
			pngdebug.h
			pnginfo.h
			pngpriv.h
			pngstruct.h)
			
add_library(png STATIC ${SOURCES} ${HEADERS})
target_link_libraries(png zlib)