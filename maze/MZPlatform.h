// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef PLATFORM_H
#define PLATFORM_H
#pragma once

#ifdef WIN32

#	define MZ_WINDOWS

#	pragma warning(disable:4996)

#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN 
#	include <Windows.h>

#	ifdef _DEBUG

#		define MZ_DEBUG

#		pragma warning(disable:4005)
#		ifdef __cplusplus
#			include <cstdlib>
#		else
#			include <stdlib.h>
#		endif
#		define _CRTDBG_MAP_ALLOC
#		include <crtdbg.h>
#	endif
#else

#	define MZ_LINUX

#	include <pthread.h>

#endif

#endif
