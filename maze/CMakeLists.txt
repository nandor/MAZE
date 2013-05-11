# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set (SOURCES MZCamera.cc
			 MZCallback.cc
             MZEngine.cc
			 MZException.cc
			 MZBound.cc
			 MZFont.cc
			 MZGL.c
             MZJSON.cc
			 MZLight.cc
			 MZLog.cc
             MZLua.cc
			 MZLuaMath.cc
			 MZLuaScene.cc
			 MZMain.cc
			 MZMath.cc
			 MZModel.cc
			 MZObject.cc
			 MZPlayer.cc
			 MZProgram.cc
			 MZRenderer.cc
			 MZRenderBuffer.cc
			 MZResource.cc
			 MZRsmngr.cc
			 MZScene.cc
			 MZTexture.cc
			 MZThread.cc
			 MZWorld.cc)
			 
set (HEADERS MZAtomic.h
			 MZCamera.h
			 MZCallback.h
			 MZEngine.h
			 MZEntity.h
			 MZException.h
			 MZBound.h
			 MZFont.h
			 MZGL.h
             MZJSON.h
			 MZLight.h
			 MZLog.h
             MZLua.h
			 MZLuaMath.h
			 MZLuaScene.h
			 MZMath.h
			 MZModel.h
			 MZObject.h
			 MZPlayer.h
			 MZPlatform.h
			 MZProgram.h
			 MZRenderer.h
			 MZRenderBuffer.h
			 MZResource.h
			 MZRsmngr.h
			 MZScene.h
			 MZSingleton.h
			 MZString.h
			 MZTexture.h
			 MZThread.h
			 MZWorld.h)

set (LIBS libjpeg opengl32 lua52)
set (LIBS_RELASE libpng15_static zlibstatic)
set (LIBS_DEBUG	libpng15_staticd zlibstaticd)

add_executable(maze WIN32 ${SOURCES} ${HEADERS})
target_link_libraries(maze ${LIBS})
target_link_libraries(maze debug ${LIBS_DEBUG})
target_link_libraries(maze optimized ${LIBS_RELASE})
