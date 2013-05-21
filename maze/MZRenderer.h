// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RENDERER_H
#define RENDERER_H
#pragma once

#include <hash_map>
#include "MZPlatform.h"
#include "MZThread.h"
#include "MZLight.h"
#include "MZRenderBuffer.h"

namespace MAZE
{
	class Program;
	class Engine;
	class ResourceManager;
			
	/**
		The renderer managers the rendering thread and the renderbuffers
	*/
	class Renderer : public Thread
	{	
	private:

		/**
			Vertex used when rendering widgets
		*/
		struct Vertex
		{
			// Position
			float x, y;

			// Layer
			float z;

			// Texture
			float s, t;
		};

	public:

		/**
			Creates a new renderer with an associated OpenGL context
			@param engine Pointer to the engine
		*/
		Renderer(Engine* engine);

		/**
			Destroys the renderer
		*/
		~Renderer();

		/**
			Initializes the renderer
		*/
		void Init();

		/**
			Destroys the renderer
		*/
		void Destroy();

		/**
			Return the back buffer
		*/
		RenderBuffer* GetBuffer();
		
		/**
			Swap the rendering buffers
		*/
		void SwapBuffers();
				
		/**
			Return the context
		*/
		HGLRC GetContext()
		{
			return mContext;
		}

	private:
						
		void RenderGeometry();
		void RenderLights();
		void RenderPostFX();

	private:

		void RenderSkybox();
		void RenderObjects();
		void RenderPointlights();
		void RenderSpotlights();
		void RenderDirlights();
		void RenderFog();
		void RenderDOF();
		void RenderWidgets();

		void InitTargets();
		void InitPrograms();
		void InitBuffers();

		void DestroyTargets();
		void DestroyPrograms();
		void DestroyBuffers();

	private:

		/**
			Worker thread
		*/
		int Worker();
		
	private:

		// Number of instances in a batch
		static const size_t INSTANCE_BATCH = 1024;
		static const size_t WIDGET_BATCH = 4096;

		// List of shader programs
		Program *mObjectProgram;
		Program *mSkyboxProgram;
		Program *mDirlightProgram;
		Program *mSpotlightProgram;
		Program *mPointlightProgram;
		Program *mVolumeProgram;
		Program *mShadowProgram;
		Program *mFogProgram;
		Program *mDOFProgram;
		Program *mBlurProgram;
		Program *mWidgetProgram;

		// List of render targets
		MGLuint mGeomDiffuseTarget;
		MGLuint mGeomNormalTarget;
		MGLuint mGeomPositionTarget;
		MGLuint mColor0Target;
		MGLuint mColor1Target;
		MGLuint mShadowTarget;
		MGLuint mDepthTarget;
		MGLuint mRandomTexture;
				
		// List of FBOs
		MGLuint mGeometryFBO;
		MGLuint mShadowFBO;
		MGLuint mPostFXFBO;
		
		// Static VBOs
		MGLuint mLightVBO;
		MGLuint mSkyboxVBO;
		MGLuint mQuadVBO;
		MGLuint mWidgetVBO;
		Vertex *mWidgets;
		MGLuint mInstanceVBO;
		glm::mat4 *mInstances;

		// Configuration
		int mWidth;
		int mHeight;
		bool mEnableShadows;
		bool mEnableDOF;
		bool mEnableFog;
		MGLuint mMapSize;
						
		// Buffers & system
		Engine *mEngine;
		RenderBuffer mBuffers[2];
		RenderBuffer *mFront;
		Mutex mBufferMutex;
		HGLRC mContext;
		LARGE_INTEGER mFreq;
		size_t mFXCount;
		float mTime;
	};	
};

#endif
