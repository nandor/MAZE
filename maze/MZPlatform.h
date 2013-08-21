// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef PLATFORM_H
#define PLATFORM_H

#undef __STRICT_ANSI__

// Standard libraries
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <map>
#include <new>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <typeinfo>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <forward_list>
#include <unordered_map>

// Platform libraries
#if defined(_WIN32)

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <WinSock2.h>
#	include <Rpc.h>
#	include <gl/GL.h>

#	ifdef _MSC_VER
#		pragma warning(disable: 4005)
#		pragma warning(disable: 4996)
#	endif

#	ifdef _DEBUG
#		define _CRTDBG_MAP_ALLOC
#		include <crtdbg.h>
#	endif

#endif

// SSE, SSE2, SSE3
#include <xmmintrin.h>

// 3rd party libraries
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <jpeg/jpeglib.h>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <al/al.h>
#include <al/alc.h>
#include <png.h>
#include <lua.hpp>
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include <freetype/freetype.h>

// MAZE headers
#include "MZGL.h"
#include "MZAtomic.h"
#include "MZSingleton.h"
#include "MZLog.h"
#include "MZThread.h"
#include "MZConfig.h"
#include "MZString.h"
#include "MZException.h"
#include "MZCallback.h"
#include "MZMath.h"
#include "MZSSE.h"
#include "MZRay.h"
#include "MZSphere.h"
#include "MZBoundingBox.h"
#include "MZFrustum.h"
#include "MZResource.h"
#include "MZFont.h"
#include "MZSound.h"
#include "MZTexture.h"
#include "MZModel.h"
#include "MZRsmngr.h"
#include "MZSoundSource.h"
#include "MZLua.h"
#include "MZFrustum.h"
#include "MZEntity.h"
#include "MZObject.h"
#include "MZLight.h"
#include "MZCamera.h"
#include "MZPlayer.h"
#include "MZCharacter.h"
#include "MZScene.h"
#include "MZNetwork.h"
#include "MZProgram.h"
#include "MZRenderBuffer.h"
#include "MZRenderer.h"
#include "MZWorld.h"
#include "MZEngine.h"

#endif
