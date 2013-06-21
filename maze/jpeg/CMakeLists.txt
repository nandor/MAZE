# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set(SOURCES jaricom.c
			jcapimin.c
			jcapistd.c
			jcarith.c
			jccoefct.c
			jccolor.c
			jcdctmgr.c
			jchuff.c
			jcinit.c
			jcmainct.c
			jcmarker.c
			jcmaster.c
			jcomapi.c
			jconfig.h
			jcparam.c
			jcprepct.c
			jcsample.c
			jctrans.c
			jdapimin.c
			jdapistd.c
			jdarith.c
			jdatadst.c
			jdatasrc.c
			jdcoefct.c
			jdcolor.c
			jdct.h
			jddctmgr.c
			jdhuff.c
			jdinput.c
			jdmainct.c
			jdmarker.c
			jdmaster.c
			jdmerge.c
			jdpostct.c
			jdsample.c
			jdtrans.c
			jerror.c
			jerror.h
			jfdctflt.c
			jfdctfst.c
			jfdctint.c
			jidctflt.c
			jidctfst.c
			jidctint.c
			jinclude.h
			jmemmgr.c
			jmemnobs.c
			jquant1.c
			jquant2.c
			jutils.c)
			
set(HEADERS jconfig.h
			jdct.h
			jmemsys.h
			jmorecfg.h
			jpegint.h
			jpeglib.h
			jversion.h)
			
add_library(jpeg STATIC ${SOURCES} ${HEADERS})
