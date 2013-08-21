// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RENDERER_H
#define RENDERER_H
#pragma once

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
			float x, y, z;

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

		/**
			Return a reference to the engine
		*/
		Engine* GetEngine()
		{
			return mEngine;
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
		static const size_t WIDGET_BATCH = 1024;

		// List of shader programs
		Program *mObjectProgram;
		Program *mSkinnedProgram;
		Program *mSkyboxProgram;
		Program *mDLightProgram;
		Program *mSLightProgram;
		Program *mPLightProgram;
		Program *mDShadowProgram;
		Program *mSShadowProgram;
		Program *mPShadowProgram;
		Program *mDProjProgram;
		Program *mSProjProgram;
		Program *mPProjProgram;
		Program *mFogProgram;
		Program *mDOFProgram;
		Program *mWidgetProgram;

		// List of render targets
		MGLuint mGeomDiffuseTarget;
		MGLuint mGeomNormalTarget;
		MGLuint mGeomPositionTarget;
		MGLuint mColor0Target;
		MGLuint mColor1Target;
		MGLuint mShadowTarget;
		MGLuint mDepthTarget;
				
		// List of FBOs
		MGLuint mGeometryFBO;
		MGLuint mShadowFBO;
		MGLuint mPostFXFBO;
		size_t mFXCount;
		
		// Static VBOs
		MGLuint mLightVBO;
		MGLuint mSkyboxVBO;
		MGLuint mQuadVBO;
		MGLuint mWidgetVBO;
		MGLuint mInstanceVBO;
		glm::mat4 *mInstances;
		Vertex *mWidgets;

		// Configuration
		int mWidth;
		int mHeight;
		bool mEnableShadows;
		bool mEnableDOF;
		bool mEnableFog;
		MGLuint mMapSize;
						
		// System
		Engine *mEngine;
		HGLRC mContext;

		// Renderbuffer
		RenderBuffer *mBuffers;
		RenderBuffer *mFront;
		Mutex mBufferMutex;

		// Timer
		LARGE_INTEGER mFreq;
		float mTime;
	};	
};

#endif
