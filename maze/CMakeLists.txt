# This file is part of the MAZE project.
# Licensing information can be found in the LICENSE file
# (C) 2012 The MAZE project. All rights reserved.

cmake_minimum_required(VERSION 2.8)

set (SOURCES MZCamera.cc
			 MZCallback.cc
             MZConfig.cc
             MZEngine.cc
             MZEntity.cc
			 MZException.cc
			 MZBoundingBox.cc
			 MZFont.cc
			 MZFrustum.cc
			 MZGL.cc
			 MZLight.cc
			 MZLog.cc
             MZLua.cc
			 MZLuaMath.cc
			 MZLuaScene.cc
			 MZMain.cc
			 MZMath.cc
			 MZModel.cc
			 MZNetwork.cc
			 MZObject.cc
			 MZPlayer.cc
			 MZProgram.cc
			 MZPlatform.cc
			 MZRay.cc
			 MZRenderer.cc
			 MZRenderBuffer.cc
			 MZResource.cc
			 MZRsmngr.cc
			 MZScene.cc
			 MZSphere.cc
             MZSound.cc
			 MZSoundSource.cc
			 MZSSE.cc
			 MZTexture.cc
			 MZThread.cc
			 MZWorld.cc)
			 
set (HEADERS MZAtomic.h
			 MZCamera.h
			 MZCallback.h
             MZConfig.h
			 MZEngine.h
			 MZEntity.h
			 MZException.h
			 MZBoundingBox.h
			 MZFont.h
			 MZFrustum.h
			 MZGL.h
			 MZLight.h
			 MZLog.h
             MZLua.h
			 MZMath.h
			 MZModel.h
			 MZNetwork.h
			 MZObject.h
			 MZPlayer.h
			 MZPlatform.h
			 MZProgram.h
			 MZRay.h
			 MZRenderer.h
			 MZRenderBuffer.h
			 MZResource.h
			 MZRsmngr.h
			 MZScene.h
			 MZSingleton.h
			 MZSphere.h
			 MZString.h
             MZSound.h
			 MZSoundSource.h
			 MZSSE.h
			 MZTexture.h
			 MZThread.h
			 MZWorld.h)

set (LIBS jpeg
		  ogg 
		  vorbis
		  zlib
		  png
		
		  Rpcrt4
		  wsock32
		  opengl32
		
		  openal32 			
		  lua52 
		  freetype)

add_executable(maze WIN32 ${SOURCES} ${HEADERS})
add_precompiled_header(maze MZPlatform.h)
target_link_libraries(maze ${LIBS})

