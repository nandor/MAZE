# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set (SOURCES MZCamera.cc
             MZEngine.cc
             MZEntity.cc
			 MZException.cc
			 MZBound.cc
			 MZGL.c
			 MZLight.cc
			 MZLog.cc
			 MZMain.cc
			 MZMaterial.cc
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
			 MZEngine.h
			 MZEntity.h
			 MZException.h
			 MZBound.h
			 MZGL.h
			 MZLight.h
			 MZLog.h
			 MZMaterial.h
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

set (LIBS libjpeg opengl32)
set (LIBS_RELASE libpng15_static zlibstatic)
set (LIBS_DEBUG	libpng15_staticd zlibstaticd)

add_executable(maze WIN32 ${SOURCES} ${HEADERS})
target_link_libraries(maze ${LIBS})
target_link_libraries(maze debug ${LIBS_DEBUG})
target_link_libraries(maze optimized ${LIBS_RELASE})
