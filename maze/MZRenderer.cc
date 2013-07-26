// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------	
const static float STATIC_LIGHT[] =
{
	/// Point light - sphere volume
	-0.16f,  0.16f, -0.50f,	 0.16f, -0.16f, -0.50f, -0.16f, -0.16f, -0.50f, 
	-0.16f,  0.16f, -0.50f,  0.16f,  0.16f, -0.50f,	 0.16f, -0.16f, -0.50f, 
	-0.16f,  0.16f,  0.50f, -0.16f, -0.16f,  0.50f,	 0.16f, -0.16f,  0.50f, 
	-0.16f,  0.16f,  0.50f,  0.16f, -0.16f,  0.50f,	 0.16f,  0.16f,  0.50f, 
	-0.50f,  0.16f, -0.16f, -0.50f, -0.16f, -0.16f,	-0.50f, -0.16f,  0.16f, 
	-0.50f,  0.16f, -0.16f, -0.50f, -0.16f,  0.16f,	-0.50f,  0.16f,  0.16f, 
	 0.50f,  0.16f, -0.16f,  0.50f, -0.16f,  0.16f,	 0.50f, -0.16f, -0.16f,	
	 0.50f,  0.16f, -0.16f,  0.50f,  0.16f,  0.16f,	 0.50f, -0.16f,  0.16f, 
	-0.16f, -0.50f, -0.16f,  0.16f, -0.50f,  0.16f,	-0.16f, -0.50f,  0.16f, 
	-0.16f, -0.50f, -0.16f,  0.16f, -0.50f, -0.16f,	 0.16f, -0.50f,  0.16f, 
	-0.16f,  0.50f, -0.16f, -0.16f,  0.50f,  0.16f,	 0.16f,  0.50f,  0.16f, 
	-0.16f,  0.50f, -0.16f,	 0.16f,  0.50f,  0.16f,	 0.16f,  0.50f, -0.16f, 
	-0.16f, -0.16f, -0.50f,  0.16f, -0.50f, -0.16f,	-0.16f, -0.50f, -0.16f, 
	-0.16f, -0.16f, -0.50f,  0.16f, -0.16f, -0.50f,	 0.16f, -0.50f, -0.16f, 
	-0.16f,  0.16f, -0.50f, -0.16f,  0.50f, -0.16f,  0.16f,  0.50f, -0.16f, 
	-0.16f,  0.16f, -0.50f,  0.16f,  0.50f, -0.16f,	 0.16f,  0.16f, -0.50f,  
	-0.16f,  0.16f,  0.50f,  0.16f,  0.50f,  0.16f, -0.16f,  0.50f,  0.16f, 
	-0.16f,  0.16f,  0.50f,  0.16f,  0.16f,  0.50f,  0.16f,  0.50f,  0.16f, 
	-0.16f, -0.16f,  0.50f, -0.16f, -0.50f,  0.16f,	 0.16f, -0.50f,  0.16f,
	-0.16f, -0.16f,  0.50f,  0.16f, -0.50f,  0.16f,	 0.16f, -0.16f,  0.50f, 
	 0.50f, -0.16f, -0.16f,	 0.16f, -0.50f,  0.16f,	 0.16f, -0.50f, -0.16f,
	 0.50f, -0.16f, -0.16f,	 0.50f, -0.16f,  0.16f,	 0.16f, -0.50f,  0.16f,
	-0.50f, -0.16f, -0.16f,	-0.16f, -0.50f, -0.16f, -0.16f, -0.50f,  0.16f,
	-0.50f, -0.16f, -0.16f,	-0.16f, -0.50f,  0.16f, -0.50f, -0.16f,  0.16f,
	 0.50f,  0.16f, -0.16f,	 0.16f,  0.50f, -0.16f,	 0.16f,  0.50f,  0.16f,
	 0.50f,  0.16f, -0.16f,	 0.16f,  0.50f,  0.16f,  0.50f,  0.16f,  0.16f,
	-0.50f,  0.16f, -0.16f,	-0.16f,  0.50f,  0.16f,	-0.16f,  0.50f, -0.16f,
	-0.50f,  0.16f, -0.16f,	-0.50f,  0.16f,  0.16f,	-0.16f,  0.50f,  0.16f,
	-0.50f,  0.16f, -0.16f,	-0.16f, -0.16f, -0.50f,	-0.50f, -0.16f, -0.16f,
	-0.50f,  0.16f, -0.16f,	-0.16f,  0.16f, -0.50f,	-0.16f, -0.16f, -0.50f,
	-0.50f,  0.16f,  0.16f,	-0.50f, -0.16f,  0.16f,	-0.16f, -0.16f,  0.50f,
	-0.50f,  0.16f,  0.16f,	-0.16f, -0.16f,  0.50f,	-0.16f,  0.16f,  0.50f,
	 0.50f,  0.16f, -0.16f,	 0.50f, -0.16f, -0.16f,	 0.16f, -0.16f, -0.50f,
	 0.50f,  0.16f, -0.16f,	 0.16f, -0.16f, -0.50f,	 0.16f,  0.16f, -0.50f,
	 0.50f,  0.16f,  0.16f,	 0.16f, -0.16f,  0.50f,	 0.50f, -0.16f,  0.16f,
	 0.50f,  0.16f,  0.16f,	 0.16f,  0.16f,  0.50f,	 0.16f, -0.16f,  0.50f,
	-0.16f,  0.50f, -0.16f,	-0.16f,  0.16f, -0.50f,	-0.50f,  0.16f, -0.16f,
	 0.16f,  0.50f, -0.16f,	 0.50f,  0.16f, -0.16f,	 0.16f,  0.16f, -0.50f,
	-0.16f,  0.50f,  0.16f,	-0.50f,  0.16f,  0.16f,	-0.16f,  0.16f,  0.50f,
	 0.16f,  0.50f,  0.16f,	 0.16f,  0.16f,  0.50f,	 0.50f,  0.16f,  0.16f,
	-0.16f, -0.50f, -0.16f,	-0.50f, -0.16f, -0.16f,	-0.16f, -0.16f, -0.50f,
	 0.16f, -0.50f, -0.16f,	 0.16f, -0.16f, -0.50f,	 0.50f, -0.16f, -0.16f,
	-0.16f, -0.50f,  0.16f,	-0.16f, -0.16f,  0.50f,	-0.50f, -0.16f,  0.16f,
	 0.16f, -0.50f,  0.16f,	 0.50f, -0.16f,  0.16f,	 0.16f, -0.16f,  0.50f,
	
	/// Spot light - cone volume
	 0.00f,  0.00f,  0.00f,  1.00f,  0.50f,  0.16f,  1.00f,  0.50f, -0.16f,
	 0.00f,  0.00f,  0.00f,  1.00f, -0.50f, -0.16f,  1.00f, -0.50f,  0.16f, 
	 0.00f,  0.00f,  0.00f,  1.00f,  0.16f, -0.50f,  1.00f, -0.16f, -0.50f, 
	 0.00f,  0.00f,  0.00f,  1.00f, -0.16f,  0.50f,  1.00f,  0.16f,  0.50f,
	 0.00f,  0.00f,  0.00f,  1.00f,  0.50f, -0.16f,  1.00f,  0.16f, -0.50f,
	 0.00f,  0.00f,  0.00f,  1.00f, -0.16f, -0.50f,  1.00f, -0.50f, -0.16f, 
	 0.00f,  0.00f,  0.00f,  1.00f,  0.16f,  0.50f,  1.00f,  0.50f,  0.16f,
	 0.00f,  0.00f,  0.00f,  1.00f, -0.50f,  0.16f,  1.00f, -0.16f,  0.50f, 	
	 1.00f,  0.16f, -0.50f,  1.00f, -0.50f, -0.16f,  1.00f, -0.16f, -0.50f,
	 1.00f,  0.16f, -0.50f,  1.00f,  0.50f, -0.16f,  1.00f, -0.50f, -0.16f,
	 1.00f,  0.50f, -0.16f,  1.00f, -0.50f,  0.16f,  1.00f, -0.50f, -0.16f,
	 1.00f,  0.50f, -0.16f,  1.00f,  0.50f,  0.16f,  1.00f, -0.50f,  0.16f,
	 1.00f,  0.50f,  0.16f,  1.00f, -0.16f,  0.50f,  1.00f, -0.50f,  0.16f,
	 1.00f,  0.50f,  0.16f,  1.00f,  0.16f,  0.50f,  1.00f, -0.16f,  0.50f
};

// ------------------------------------------------------------------------------------------------	
const static float STATIC_SKYBOX[] =
{
	-1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,		 
	-1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,		 
	-1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,		 
	-1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,		 
	-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,	
	-1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f
};

// ------------------------------------------------------------------------------------------------	
const static float STATIC_QUAD[] =
{
	-1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	 1.0f,  1.0f
};

// ------------------------------------------------------------------------------------------------
static const glm::mat4 DEPTH_BIAS(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

// ------------------------------------------------------------------------------------------------
static const MGLenum BUFFERS[] = 
{
	MGL_COLOR_ATTACHMENT + 0, 
	MGL_COLOR_ATTACHMENT + 1, 
	MGL_COLOR_ATTACHMENT + 2,
	MGL_COLOR_ATTACHMENT + 3
};

// ------------------------------------------------------------------------------------------------
MGLuint CreateTarget(MGLenum format, size_t width, size_t height)
{
	MGLuint texture;

	mglGenTextures(1, &texture);
	mglBindTexture(MGL_TEXTURE_2D, texture);

	// Keep in video memory
	mglTexParameterf(MGL_TEXTURE_2D, MGL_TEXTURE_PRIORITY, 1.0f);

	// Fast filetering
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR);

	// Clamp to edges - prevent artefacts in shaders
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_WRAP_S, MGL_CLAMP_TO_EDGE);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_WRAP_T, MGL_CLAMP_TO_EDGE);

	// Fill to blank
	mglTexImage2D(MGL_TEXTURE_2D, 0, format, width, height, 0, MGL_RGBA, MGL_FLOAT, 0);

	return texture;
}

// ------------------------------------------------------------------------------------------------
Renderer::Renderer(Engine* engine)
	: mObjectProgram(NULL),
	  mSkyboxProgram(NULL),
	  mDirlightProgram(NULL),
	  mSpotlightProgram(NULL),
	  mPointlightProgram(NULL),
	  mVolumeProgram(NULL),
	  mShadowProgram(NULL),
	  mFogProgram(NULL),
	  mDOFProgram(NULL),
	  mWidgetProgram(NULL),
	  mGeomDiffuseTarget(0),
	  mGeomNormalTarget(0),
	  mGeomPositionTarget(0),
	  mColor0Target(0),
	  mColor1Target(0),
	  mShadowTarget(0),
	  mDepthTarget(0),
	  mLightVBO(0),
	  mSkyboxVBO(0),
	  mQuadVBO(0),
	  mWidgetVBO(0),
	  mWidgets(NULL),
	  mInstanceVBO(0),
	  mInstances(NULL),
	  mMapSize(1024),
	  mEngine(engine),
	  mContext(NULL),
	  mBuffers(new RenderBuffer[2]),
	  mFront(&mBuffers[0])
{	
	Engine::Setup& setup = mEngine->GetSetup();
	mWidth		   = setup.WindowWidth;
	mHeight		   = setup.WindowHeight;
	mEnableShadows = setup.EnableShadows;
	mEnableFog	   = setup.EnableFog;
	mEnableDOF	   = setup.EnableDOF;

	::QueryPerformanceFrequency(&mFreq);
}

// ------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
	Destroy();

	if (mBuffers) 
	{ 
		delete[] mBuffers; 
		mBuffers = NULL; 
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::Init()
{
	// Create the fake context
	HDC	dc = mEngine->GetDC();
	
	if (!(mContext = wglCreateContext(dc)) || !wglMakeCurrent(dc, mContext))
	{
		throw MGLException("[Renderer] Cannot create OpenGL 1.4 context");
	}
		
	// Initialize extensions
	if (!mglInit())
	{
		throw MGLException("[Renderer] Cannot initialize extensions");
	}

	if (GL.VSync)
	{
		mwglSwapInterval(1);
	}
	
	mglEnable(MGL_TEXTURE_CUBE_MAP_SEAMLESS);
	Log::Inst() << "[Renderer] GL vendor:     " << (const char*)mglGetString(MGL_VENDOR);
	Log::Inst() << "[Renderer] GL version:    " << (const char*)mglGetString(MGL_VERSION);
	Log::Inst() << "[Renderer] GLSL version:  " << (const char*)mglGetString(MGL_SHADING_LANGUAGE_VERSION);

	InitTargets();
	InitPrograms();
	InitBuffers();

	// Context will be used on the rendering thread
	wglMakeCurrent(NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
void Renderer::InitTargets()
{
	// Color targets
	mGeomDiffuseTarget	= CreateTarget(MGL_RGBA,	mWidth, mHeight);
	mGeomNormalTarget	= CreateTarget(MGL_RGBA,	mWidth, mHeight);
	mGeomPositionTarget = CreateTarget(MGL_RGBA32F,	mWidth, mHeight);
	mColor0Target		= CreateTarget(MGL_RGBA,	mWidth, mHeight);
	mColor1Target		= CreateTarget(MGL_RGBA,	mWidth, mHeight);
	
	// Shadow map
	if (mEnableShadows)
	{
		mglGenTextures(1, &mShadowTarget);
		mglBindTexture(MGL_TEXTURE_2D_ARRAY, mShadowTarget);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_WRAP_S, MGL_CLAMP_TO_EDGE);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_WRAP_T, MGL_CLAMP_TO_EDGE);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_DEPTH_TEXTURE_MODE, MGL_INTENSITY); 	
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_COMPARE_FUNC, MGL_LEQUAL);
		mglTexParameteri(MGL_TEXTURE_2D_ARRAY, MGL_TEXTURE_COMPARE_MODE, MGL_COMPARE_R_TO_TEXTURE);
		mglTexImage3D(MGL_TEXTURE_2D_ARRAY, 0, MGL_DEPTH_COMPONENT, mMapSize, mMapSize, 4, 0, MGL_DEPTH_COMPONENT, MGL_UNSIGNED_BYTE, 0);
		mglBindTexture(MGL_TEXTURE_2D_ARRAY, 0);
	}
	
	// Depth renderbuffer
	mglGenRenderbuffers(1, &mDepthTarget);
	mglBindRenderbuffer(MGL_RENDERBUFFER, mDepthTarget);
	mglRenderbufferStorage(MGL_RENDERBUFFER, MGL_DEPTH_COMPONENT24, mWidth, mHeight);
	mglBindRenderbuffer(MGL_RENDERBUFFER, 0);
		
	// Geometry buffer FBO
	mglGenFramebuffers(1, &mGeometryFBO);
	mglBindFramebuffer(MGL_FRAMEBUFFER, mGeometryFBO);
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 0, MGL_TEXTURE_2D, mGeomDiffuseTarget, 0);
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 1, MGL_TEXTURE_2D, mGeomNormalTarget, 0);
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 2, MGL_TEXTURE_2D, mGeomPositionTarget, 0);
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 3, MGL_TEXTURE_2D, mColor0Target, 0);
	mglFramebufferRenderbuffer(MGL_FRAMEBUFFER, MGL_DEPTH_ATTACHMENT, MGL_RENDERBUFFER, mDepthTarget);

	// Shadow FBO
	mglGenFramebuffers(1, &mShadowFBO);
	mglBindFramebuffer(MGL_FRAMEBUFFER, mShadowFBO);
	mglReadBuffer(MGL_NONE);
	mglDrawBuffer(MGL_NONE);

	// PostFX FBO
	mglGenFramebuffers(1, &mPostFXFBO);
	mglBindFramebuffer(MGL_FRAMEBUFFER, mPostFXFBO);
	mglReadBuffer(MGL_COLOR_ATTACHMENT + 0);
	mglDrawBuffer(MGL_NONE);
	
	mglBindFramebuffer(MGL_FRAMEBUFFER, 0);
}

// ------------------------------------------------------------------------------------------------
void Renderer::InitPrograms()
{
	const std::string dir = mEngine->GetSetup().ResourceDir + "/shader/";

	mObjectProgram = new Program(this, "object");
	mObjectProgram->Compile(Program::VERTEX, dir + "object.vs.glsl");
	mObjectProgram->Compile(Program::FRAGMENT, dir + "object.fs.glsl");
	mObjectProgram->Link();
		
	mSkyboxProgram = new Program(this, "skybox");
	mSkyboxProgram->Compile(Program::VERTEX, dir + "skybox.vs.glsl");
	mSkyboxProgram->Compile(Program::FRAGMENT, dir + "skybox.fs.glsl");
	mSkyboxProgram->Link();
	
	mDirlightProgram = new Program(this, "dirlight");
	mDirlightProgram->Compile(Program::VERTEX, dir + "dirlight.vs.glsl");
	mDirlightProgram->Compile(Program::FRAGMENT, dir + (mEnableShadows ? "dirlight_shadow.fs.glsl" : "dirlight.fs.glsl"));
	mDirlightProgram->Link();

	mSpotlightProgram = new Program(this, "spotlight");
	mSpotlightProgram->Compile(Program::VERTEX, dir + "spotlight.vs.glsl");
	mSpotlightProgram->Compile(Program::FRAGMENT, dir + "spotlight.fs.glsl");
	mSpotlightProgram->Link();

	mPointlightProgram = new Program(this, "pointlight");
	mPointlightProgram->Compile(Program::VERTEX, dir + "pointlight.vs.glsl");
	mPointlightProgram->Compile(Program::FRAGMENT, dir + "pointlight.fs.glsl");
	mPointlightProgram->Link();

	mVolumeProgram = new Program(this, "volume");
	mVolumeProgram->Compile(Program::VERTEX, dir + "volume.vs.glsl");
	mVolumeProgram->Compile(Program::FRAGMENT, dir + "volume.fs.glsl");
	mVolumeProgram->Link();
	
	mWidgetProgram = new Program(this, "widget");
	mWidgetProgram->Compile(Program::VERTEX, dir + "widget.vs.glsl");
	mWidgetProgram->Compile(Program::FRAGMENT, dir + "widget.fs.glsl");
	mWidgetProgram->Link();

	if (mEnableShadows)
	{
		mShadowProgram = new Program(this, "shadow");
		mShadowProgram->Compile(Program::VERTEX, dir + "shadow.vs.glsl");
		mShadowProgram->Compile(Program::FRAGMENT, dir + "shadow.fs.glsl");
		mShadowProgram->Link();
	}

	if (mEnableFog)
	{
		mFogProgram = new Program(this, "fog");
		mFogProgram->Compile(Program::VERTEX, dir + "fog.vs.glsl");
		mFogProgram->Compile(Program::FRAGMENT, dir + "fog.fs.glsl");
		mFogProgram->Link();
	}

	if (mEnableDOF)
	{
		mDOFProgram = new Program(this, "dof");
		mDOFProgram->Compile(Program::VERTEX, dir + "dof.vs.glsl");
		mDOFProgram->Compile(Program::FRAGMENT, dir + "dof.fs.glsl");
		mDOFProgram->Link();
	}
}

// ------------------------------------------------------------------------------------------------	
void Renderer::InitBuffers()
{
	// Light volumes
	mglGenBuffers(1, &mLightVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mLightVBO);
	mglBufferData(MGL_ARRAY_BUFFER, sizeof(STATIC_LIGHT), STATIC_LIGHT, MGL_STATIC_DRAW);

	// Skybox
	mglGenBuffers(1, &mSkyboxVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mSkyboxVBO);
	mglBufferData(MGL_ARRAY_BUFFER, sizeof(STATIC_SKYBOX), STATIC_SKYBOX, MGL_STATIC_DRAW);

	// PostFX quad
	mglGenBuffers(1, &mQuadVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mQuadVBO);
	mglBufferData(MGL_ARRAY_BUFFER, sizeof(STATIC_QUAD), STATIC_QUAD, MGL_STATIC_DRAW);

	// Instance model matrices
	mInstances = new glm::mat4[INSTANCE_BATCH];
	mglGenBuffers(1, &mInstanceVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mInstanceVBO);
	mglBufferData(MGL_ARRAY_BUFFER, INSTANCE_BATCH * sizeof(glm::mat4), mInstances, MGL_DYNAMIC_DRAW);

	// Widgets
	mWidgets = new Vertex[WIDGET_BATCH * 6];
	mglGenBuffers(1, &mWidgetVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mWidgetVBO);
	mglBufferData(MGL_ARRAY_BUFFER, WIDGET_BATCH * sizeof(Vertex) * 6, mWidgets, MGL_DYNAMIC_DRAW);
}

// ------------------------------------------------------------------------------------------------
void Renderer::Destroy()
{
	if (mContext)
	{
		HGLRC mOldContext;
		
		if (mContext != (mOldContext = wglGetCurrentContext()))
		{
			wglMakeCurrent(mEngine->GetDC(), mContext);
		}
		
		DestroyBuffers();
		DestroyTargets();
		DestroyPrograms();

		mglDeleteFramebuffers(1, &mGeometryFBO);
		mglDeleteFramebuffers(1, &mPostFXFBO);
		mglDeleteFramebuffers(1, &mShadowFBO);

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mContext);

		if (mContext != mOldContext) 
		{
			wglMakeCurrent(mEngine->GetDC(), mOldContext);
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Renderer::DestroyTargets()
{	
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, 0);
	mglBindTexture(MGL_TEXTURE_2D, 0);
	mglBindTexture(MGL_TEXTURE_2D_ARRAY, 0);

	if (mGeometryFBO) { mglDeleteFramebuffers(1, &mGeometryFBO); mGeometryFBO = 0; }
	if (mPostFXFBO)   { mglDeleteFramebuffers(1, &mPostFXFBO);	 mPostFXFBO = 0;   }
	if (mShadowFBO)   { mglDeleteFramebuffers(1, &mShadowFBO);	 mShadowFBO = 0;   }

	if (mGeomDiffuseTarget)	 { mglDeleteTextures(1, &mGeomDiffuseTarget);  mGeomDiffuseTarget = 0;  }
	if (mGeomNormalTarget)	 { mglDeleteTextures(1, &mGeomNormalTarget);   mGeomNormalTarget = 0;	}
	if (mGeomPositionTarget) { mglDeleteTextures(1, &mGeomPositionTarget); mGeomPositionTarget = 0; }
	if (mColor0Target)		 { mglDeleteTextures(1, &mColor0Target);	   mColor0Target = 0;		}
	if (mColor1Target)		 { mglDeleteTextures(1, &mColor1Target);       mColor1Target = 0;		}
	if (mShadowTarget)		 { mglDeleteTextures(1, &mShadowTarget);	   mShadowTarget = 0;		}
	if (mDepthTarget)		 { mglDeleteTextures(1, &mDepthTarget);		   mDepthTarget = 0;		}
}

// ------------------------------------------------------------------------------------------------
void Renderer::DestroyPrograms()
{
	if (mObjectProgram)		{ delete mObjectProgram;	 mObjectProgram		= NULL; }
	if (mSkyboxProgram)		{ delete mSkyboxProgram;	 mSkyboxProgram		= NULL; }
	if (mDirlightProgram)	{ delete mDirlightProgram;	 mDirlightProgram	= NULL; }
	if (mSpotlightProgram)	{ delete mSpotlightProgram;	 mSpotlightProgram	= NULL; }
	if (mPointlightProgram) { delete mPointlightProgram; mPointlightProgram = NULL; }
	if (mVolumeProgram)		{ delete mVolumeProgram;	 mVolumeProgram		= NULL; }
	if (mShadowProgram)		{ delete mShadowProgram;	 mShadowProgram		= NULL; }
	if (mFogProgram)		{ delete mFogProgram;		 mFogProgram		= NULL; }
	if (mDOFProgram)		{ delete mDOFProgram;		 mDOFProgram		= NULL; }
	if (mWidgetProgram)		{ delete mWidgetProgram;	 mWidgetProgram		= NULL; }
}

// ------------------------------------------------------------------------------------------------
void Renderer::DestroyBuffers()
{
	mglBindBuffer(MGL_ARRAY_BUFFER, 0);

	if (mLightVBO)	  { mglDeleteBuffers(1, &mLightVBO);		mLightVBO = 0;	  }
	if (mSkyboxVBO)	  { mglDeleteBuffers(1, &mSkyboxVBO);		mSkyboxVBO = 0;	  }
	if (mQuadVBO)	  { mglDeleteBuffers(1, &mQuadVBO);			mQuadVBO = 0;	  }
	if (mInstanceVBO) { mglDeleteBuffers(1, &mInstanceVBO);		mInstanceVBO = 0; }
	if (mInstances)	  { delete[] mInstances;					mInstances = NULL;}
	if (mWidgetVBO)	  { mglDeleteBuffers(1, &mWidgetVBO);		mWidgetVBO = 0;	  }
	if (mWidgets)	  { delete[] mWidgets;						mWidgets = NULL;  }
}

// ------------------------------------------------------------------------------------------------
RenderBuffer* Renderer::GetBuffer()
{
	return (mFront == &mBuffers[0]) ? &mBuffers[1] : &mBuffers[0];
}

// ------------------------------------------------------------------------------------------------
void Renderer::SwapBuffers()
{
	Mutex::ScopedLock Lock(mBufferMutex);

	mFront->Clear();
	mFront = (mFront == &mBuffers[0]) ? &mBuffers[1] : &mBuffers[0];
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderGeometry()
{		
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
    mglFramebufferTexture2D(MGL_DRAW_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 3, MGL_TEXTURE_2D, mColor0Target, 0);

	mglDrawBuffers(4, &BUFFERS[0]);
	mglClear(MGL_COLOR_BUFFER_BIT);
		
	mglDrawBuffers(2, &BUFFERS[2]);	
	RenderSkybox();
	
	mglDrawBuffers(4, &BUFFERS[0]);
	RenderObjects();
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderSkybox()
{
	mSkyboxProgram->Use();
	mSkyboxProgram->Uniform("uMVP", mFront->Camera.ProjMatrix * mFront->Camera.SkyMatrix);

	mglActiveTexture(MGL_TEXTURE + 0);
	mglBindTexture(MGL_TEXTURE_CUBE_MAP, mFront->SkyTexture ? mFront->SkyTexture->mTexture : 0);
	mSkyboxProgram->Uniform("uTexture", 0);
	mSkyboxProgram->Uniform("uFarPlane", mFront->Camera.FarPlane);
	mSkyboxProgram->Uniform("uPosition", mFront->Camera.Position);

	mglDisable(MGL_DEPTH_TEST);
	mglEnable(MGL_CULL_FACE);		
	mglEnableClientState(MGL_VERTEX_ARRAY);
	
	mglCullFace(MGL_BACK);

	mglBindBuffer(MGL_ARRAY_BUFFER, mSkyboxVBO);
	mglVertexPointer(3, MGL_FLOAT, 0, 0);
	mglDrawArrays(MGL_TRIANGLES, 0, 36);	

	mglDisableClientState(MGL_VERTEX_ARRAY);
	mglDisable(MGL_CULL_FACE);			
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderObjects()
{
	mglEnable(MGL_DEPTH_TEST);
	mglEnable(MGL_CULL_FACE);
	mglCullFace(MGL_BACK);		
	mglDepthMask(MGL_TRUE);
	mglDepthFunc(MGL_LEQUAL);
	mglClear(MGL_DEPTH_BUFFER_BIT);
		
	mObjectProgram->Use();
	mObjectProgram->Uniform("uProj", mFront->Camera.ProjMatrix);
	mObjectProgram->Uniform("uView", mFront->Camera.ViewMatrix);
	mObjectProgram->Uniform("uPosition", mFront->Camera.Position);

	MGLuint index0 = mObjectProgram->Attribute("aModel0");
	MGLuint index1 = mObjectProgram->Attribute("aModel1");
	MGLuint index2 = mObjectProgram->Attribute("aModel2");
	MGLuint index3 = mObjectProgram->Attribute("aModel3");
				
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglEnableClientState(MGL_NORMAL_ARRAY);
	mglEnableClientState(MGL_TEXTURE_COORD_ARRAY);

	mglEnableVertexAttribArray(index0);
	mglEnableVertexAttribArray(index1);
	mglEnableVertexAttribArray(index2);
	mglEnableVertexAttribArray(index3);

	mglVertexAttribDivisor(index0, 1);				
	mglVertexAttribDivisor(index1, 1);				
	mglVertexAttribDivisor(index2, 1);								
	mglVertexAttribDivisor(index3, 1);
	
	size_t count = mFront->Objects.size();
	
	while (count)
	{
		Model *model = mFront->Objects[0].model;	

		size_t instanceCount = 0;
		for (size_t i = 0; i < count && instanceCount < INSTANCE_BATCH;)
		{
			if (mFront->Objects[i].model->GetID() != model->GetID())
			{
				++i;
				continue;
			}

			mInstances[instanceCount++] = mFront->Objects[i].ModelMatrix;
			std::swap(mFront->Objects[i], mFront->Objects[--count]);
		}	
		
		if (model && model->GetState() == Resource::LOADED && model->mVBO)
		{			
			mObjectProgram->Uniform("uTexture", mFront->Objects[0].TextureMatrix);	
			mObjectProgram->Uniform("uDiffuseMap", MGL_TEXTURE_2D, 0, model->mDiffuseMap->mTexture);
			mObjectProgram->Uniform("uNormalMap", MGL_TEXTURE_2D, 1, model->mBumpMap->mTexture);
				
			mglBindBuffer(MGL_ARRAY_BUFFER, model->mVBO);
			mglVertexPointer(3, MGL_FLOAT, 32, (void*)0);
			mglNormalPointer(MGL_FLOAT, 32, (void*)12);
			mglTexCoordPointer(2, MGL_FLOAT, 32, (void*)24);

			mglBindBuffer(MGL_ARRAY_BUFFER, mInstanceVBO);				
			mglBufferData(MGL_ARRAY_BUFFER, INSTANCE_BATCH * sizeof(glm::mat4), mInstances, MGL_DYNAMIC_DRAW);
			mglVertexAttribPointer(index0, 4, MGL_FLOAT, MGL_FALSE, 64, (void*)0);
			mglVertexAttribPointer(index1, 4, MGL_FLOAT, MGL_FALSE, 64, (void*)16);
			mglVertexAttribPointer(index2, 4, MGL_FLOAT, MGL_FALSE, 64, (void*)32);
			mglVertexAttribPointer(index3, 4, MGL_FLOAT, MGL_FALSE, 64, (void*)48);

			mglDrawArraysInstanced(MGL_TRIANGLES, 0, model->mVertexCount, instanceCount);
		}
	}
	
	mglDisableVertexAttribArray(index3);
	mglDisableVertexAttribArray(index2);
	mglDisableVertexAttribArray(index1);
	mglDisableVertexAttribArray(index0);

	mglVertexAttribDivisor(index0, 0);				
	mglVertexAttribDivisor(index1, 0);				
	mglVertexAttribDivisor(index2, 0);								
	mglVertexAttribDivisor(index3, 0);

	mglDisableClientState(MGL_TEXTURE_COORD_ARRAY);
	mglDisableClientState(MGL_NORMAL_ARRAY);
	mglDisableClientState(MGL_VERTEX_ARRAY);
	
	mglDisable(MGL_DEPTH_TEST);
	mglDisable(MGL_CULL_FACE);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderPointlights()
{	
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
	mglDrawBuffer(MGL_COLOR_ATTACHMENT + 3);

	mPointlightProgram->Use();
	mPointlightProgram->Uniform("uWidth", mWidth);
	mPointlightProgram->Uniform("uHeight", mHeight);
	mPointlightProgram->Uniform("uGDiffuse",	 MGL_TEXTURE_2D, 0, mGeomDiffuseTarget );
	mPointlightProgram->Uniform("uGNormal",	 MGL_TEXTURE_2D, 1, mGeomNormalTarget  );
	mPointlightProgram->Uniform("uGPosition", MGL_TEXTURE_2D, 2, mGeomPositionTarget);

	mglBindBuffer(MGL_ARRAY_BUFFER, mLightVBO);
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglVertexPointer(3, MGL_FLOAT, 12, (void*)0);

	mglEnable(MGL_DEPTH_TEST);
	mglEnable(MGL_CULL_FACE);
	mglEnable(MGL_BLEND);

	mglDepthFunc(MGL_GEQUAL);
	mglDepthMask(MGL_FALSE);
	mglCullFace(MGL_FRONT);
	mglBlendFunc(MGL_ONE, MGL_ONE);

	for (size_t i = 0; i < mFront->Lights.size(); ++i)
	{
		LightRenderData* light = &mFront->Lights[i];
				
		if (light->Type == Light::POINT)
		{
			mPointlightProgram->Uniform("uEyePosition", mFront->Camera.Position);		
			mPointlightProgram->Uniform("uMVP", mFront->Camera.ProjMatrix * mFront->Camera.ViewMatrix * light->ModelMatrix);
			mPointlightProgram->Uniform("lPosition", light->Position);
			mPointlightProgram->Uniform("lDirection", light->Direction);
			mPointlightProgram->Uniform("lDiffuse", light->Diffuse);
			mPointlightProgram->Uniform("lSpecular", light->Specular);

			mglDrawArrays(MGL_TRIANGLES, 0, 132);
		}
	}

	mglDisable(MGL_BLEND);
	mglDisable(MGL_CULL_FACE);
	mglDisable(MGL_DEPTH_TEST);
	mglDisableClientState(MGL_VERTEX_ARRAY);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderSpotlights()
{	
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
	mglDrawBuffer(MGL_COLOR_ATTACHMENT + 3);

	mSpotlightProgram->Use();
	mSpotlightProgram->Uniform("uWidth", mWidth);
	mSpotlightProgram->Uniform("uHeight", mHeight);
	mSpotlightProgram->Uniform("uGDiffuse",	 MGL_TEXTURE_2D, 0, mGeomDiffuseTarget );
	mSpotlightProgram->Uniform("uGNormal",	 MGL_TEXTURE_2D, 1, mGeomNormalTarget  );
	mSpotlightProgram->Uniform("uGPosition",  MGL_TEXTURE_2D, 2, mGeomPositionTarget);

	mglBindBuffer(MGL_ARRAY_BUFFER, mLightVBO);
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglVertexPointer(3, MGL_FLOAT, 12, (void*)0);

	mglEnable(MGL_CULL_FACE);
	mglEnable(MGL_BLEND);

	mglDepthFunc(MGL_GEQUAL);
	mglDepthMask(MGL_FALSE);
	mglCullFace(MGL_FRONT);
	mglBlendFunc(MGL_ONE, MGL_ONE);

	for (size_t i = 0; i < mFront->Lights.size(); ++i)
	{
		LightRenderData* light = &mFront->Lights[i];
				
		if (light->Type == Light::SPOT)
		{
			mSpotlightProgram->Uniform("uEyePosition", mFront->Camera.Position);		
			mSpotlightProgram->Uniform("uMVP", mFront->Camera.ProjMatrix * mFront->Camera.ViewMatrix * light->ModelMatrix);
			mSpotlightProgram->Uniform("lPosition", light->Position);
			mSpotlightProgram->Uniform("lDirection", light->Direction);
			mSpotlightProgram->Uniform("lDiffuse", light->Diffuse);
			mSpotlightProgram->Uniform("lSpecular", light->Specular);

			mglDrawArrays(MGL_TRIANGLES, 132, 42);
		}
	}

	mglDisable(MGL_BLEND);
	mglDisable(MGL_CULL_FACE);
	mglDisable(MGL_DEPTH_TEST);
	mglDisableClientState(MGL_VERTEX_ARRAY);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderDirlights()
{
	glm::vec4 shadowZ;

	MGLuint index0 = mShadowProgram->Attribute("aModel0");
	MGLuint index1 = mShadowProgram->Attribute("aModel1");
	MGLuint index2 = mShadowProgram->Attribute("aModel2");
	MGLuint index3 = mShadowProgram->Attribute("aModel3");
					
	mglEnable(MGL_DEPTH_TEST);
	mglEnableClientState(MGL_VERTEX_ARRAY);

	for (size_t j = 0; j < mFront->Lights.size(); ++j)
	{
		LightRenderData* light = &mFront->Lights[j];
				
		if (light->Type == Light::DIRECTIONAL)
		{
			mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mShadowFBO);	
			mglViewport(0, 0, mMapSize, mMapSize);
			
			if (mEnableShadows)
			{
				if (light->CastsShadows)
				{
					mglEnable(MGL_CULL_FACE);
					mglEnable(MGL_POLYGON_OFFSET_FILL);

					mglDepthMask(MGL_TRUE);
					mglDepthFunc(MGL_LEQUAL);
					mglCullFace(MGL_BACK);	
					mglPolygonOffset(1.0f, 2.0f);

					mglVertexAttribDivisor(index0, 1);				
					mglVertexAttribDivisor(index1, 1);				
					mglVertexAttribDivisor(index2, 1);								
					mglVertexAttribDivisor(index3, 1);
					mglEnableVertexAttribArray(index0);
					mglEnableVertexAttribArray(index1);
					mglEnableVertexAttribArray(index2);
					mglEnableVertexAttribArray(index3);

					mShadowProgram->Use();
					for (int i = 0; i < light->ShadowLevels; ++i)
					{
						mglFramebufferTextureLayer(MGL_DRAW_FRAMEBUFFER, MGL_DEPTH_ATTACHMENT, mShadowTarget, 0, i);
						mglClear(MGL_DEPTH_BUFFER_BIT);
				
						shadowZ[i] = light->Shadow[i].NearZ;
						mShadowProgram->Uniform("uVP", light->Shadow[i].MVP);

						size_t count = light->Shadow[i].Count, beg = light->Shadow[i].Index;	
						while (count)
						{
							Model *model = mFront->ShadowCasters[beg].model;	

							size_t instanceCount = 0;
							for (size_t j = 0; j < count && instanceCount < INSTANCE_BATCH;)
							{
								if (mFront->ShadowCasters[beg + j].model->GetID() != model->GetID())
								{
									++j;
									continue;
								}

								mInstances[instanceCount++] = mFront->ShadowCasters[beg + j].ModelMatrix;
								std::swap(mFront->ShadowCasters[beg + j], mFront->ShadowCasters[beg + (--count)]);
							}	
		
							if (model && model->GetState() == Resource::LOADED && model->mVBO)
							{			
								mglBindBuffer(MGL_ARRAY_BUFFER, model->mVBO);
								mglVertexPointer(3, MGL_FLOAT, sizeof(Model::Vertex), (void*)0);

								mglBindBuffer(MGL_ARRAY_BUFFER, mInstanceVBO);				
								mglBufferData(MGL_ARRAY_BUFFER, INSTANCE_BATCH * sizeof(glm::mat4), mInstances, MGL_DYNAMIC_DRAW);
								mglVertexAttribPointer(index0, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)0);
								mglVertexAttribPointer(index1, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)16);
								mglVertexAttribPointer(index2, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)32);
								mglVertexAttribPointer(index3, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)48);

								mglDrawArraysInstanced(MGL_TRIANGLES, 0, model->mVertexCount, instanceCount);
							}
						}
					}

					mglDisableVertexAttribArray(index3);
					mglDisableVertexAttribArray(index2);
					mglDisableVertexAttribArray(index1);
					mglDisableVertexAttribArray(index0);
					mglVertexAttribDivisor(index0, 0);				
					mglVertexAttribDivisor(index1, 0);				
					mglVertexAttribDivisor(index2, 0);								
					mglVertexAttribDivisor(index3, 0);

					mglDisable(MGL_POLYGON_OFFSET_FILL);
					mglDisable(MGL_CULL_FACE);			
				}
				else
				{				
					mglDepthMask(MGL_TRUE);
					for (size_t i = 0; i < 4; ++i)
					{
						mglFramebufferTextureLayer(MGL_DRAW_FRAMEBUFFER, MGL_DEPTH_ATTACHMENT, mShadowTarget, 0, i);
						mglClear(MGL_DEPTH_BUFFER_BIT);
					}
				}
			}

			mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
			mglDrawBuffer(MGL_COLOR_ATTACHMENT + 3);
			mglViewport(0, 0, mWidth, mHeight);
	
			mDirlightProgram->Use();
			mDirlightProgram->Uniform("uGDiffuse",	MGL_TEXTURE_2D, 0, mGeomDiffuseTarget);
			mDirlightProgram->Uniform("uGNormal",	MGL_TEXTURE_2D, 1, mGeomNormalTarget);
			mDirlightProgram->Uniform("uGPosition",	MGL_TEXTURE_2D, 2, mGeomPositionTarget);
			mDirlightProgram->Uniform("uEyePosition", mFront->Camera.Position);
			mDirlightProgram->Uniform("lDiffuse", light->Diffuse);
			mDirlightProgram->Uniform("lSpecular", light->Specular);
			mDirlightProgram->Uniform("lAmbient", light->Ambient);
			mDirlightProgram->Uniform("lDirection", light->Direction);
						
			if (mEnableShadows)
			{
				mDirlightProgram->Uniform("uLevels", light->ShadowLevels);
				mDirlightProgram->Uniform("uShadow",    MGL_TEXTURE_2D_ARRAY, 4, mShadowTarget);
				mDirlightProgram->Uniform("uShadowMVP0", DEPTH_BIAS * light->Shadow[0].MVP);
				mDirlightProgram->Uniform("uShadowMVP1", DEPTH_BIAS * light->Shadow[1].MVP);
				mDirlightProgram->Uniform("uShadowMVP2", DEPTH_BIAS * light->Shadow[2].MVP);
				mDirlightProgram->Uniform("uShadowMVP3", DEPTH_BIAS * light->Shadow[3].MVP);	
				mDirlightProgram->Uniform("uShadowZ", shadowZ);
			}

			mglDisable(MGL_CULL_FACE);
			mglEnable(MGL_BLEND);

			mglDepthFunc(MGL_GREATER);
			mglDepthMask(MGL_FALSE);
			mglBlendFunc(MGL_ONE, MGL_ONE);

			mglBindBuffer(MGL_ARRAY_BUFFER, mQuadVBO);
			mglVertexPointer(2, MGL_FLOAT, 2 * sizeof(float), 0);
			mglDrawArrays(MGL_TRIANGLES, 0, 6);		

			mglDisable(MGL_BLEND);
		}
	}

	mglDisable(MGL_DEPTH_TEST);
	mglDisableClientState(MGL_VERTEX_ARRAY);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderPostFX()
{
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mPostFXFBO);
	mglDrawBuffer(MGL_COLOR_ATTACHMENT + 0);
		
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglBindBuffer(MGL_ARRAY_BUFFER, mQuadVBO);
	mglVertexPointer(2, MGL_FLOAT, 2 * sizeof(float), 0);

	mFXCount = 0;
	if (mEnableFog) { RenderFog(); }
	if (mEnableDOF) { RenderDOF(); }

	mglDisableClientState(MGL_VERTEX_ARRAY);

	// Copy the color target to the fbo
	if (mFXCount > 0)
	{
		mglBindFramebuffer(MGL_READ_FRAMEBUFFER, mPostFXFBO);
		mglReadBuffer(MGL_COLOR_ATTACHMENT + 0);
	}
	else
	{
		mglBindFramebuffer(MGL_READ_FRAMEBUFFER, mGeometryFBO);
		mglReadBuffer(MGL_COLOR_ATTACHMENT + 3);
	}

	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, 0);	
	mglBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, MGL_COLOR_BUFFER_BIT, MGL_NEAREST);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderFog()
{
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 0, MGL_TEXTURE_2D, mColor1Target, 0);
	mglClear(MGL_COLOR_BUFFER_BIT);

	mFogProgram->Use(); 
	mFogProgram->Uniform("uViewDir", mFront->Camera.Direction);
	mFogProgram->Uniform("uPosition", mFront->Camera.Position);
	mFogProgram->Uniform("uFarPlane", mFront->Camera.FarPlane);
	mFogProgram->Uniform("uFogColor", mFront->Fog.Color);
	mFogProgram->Uniform("uFogDensity", mFront->Fog.Density);
	mFogProgram->Uniform("uFogHeight", mFront->Fog.Height);
	mFogProgram->Uniform("uTime", mTime);
	mFogProgram->Uniform("uGPosition", MGL_TEXTURE_2D, 1, mGeomPositionTarget);
	mFogProgram->Uniform("uGColor", MGL_TEXTURE_2D, 2, mColor0Target);

	mglDrawArrays(MGL_TRIANGLES, 0, 6);	
	std::swap(mColor0Target, mColor1Target);
	mFXCount++;
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderDOF()
{
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 0, MGL_TEXTURE_2D, mColor1Target, 0);
	mglClear(MGL_COLOR_BUFFER_BIT);
	
	mDOFProgram->Use();
	mDOFProgram->Uniform("uWidth", mWidth);
	mDOFProgram->Uniform("uHeight",	mHeight);
	mDOFProgram->Uniform("uFocus",	mFront->Camera.Focus);
	mDOFProgram->Uniform("uPosition", MGL_TEXTURE_2D, 1, mGeomPositionTarget);
	mDOFProgram->Uniform("uColor", MGL_TEXTURE_2D, 2, mColor0Target);
		
	mglDrawArrays(MGL_TRIANGLES, 0, 6);		
	std::swap(mColor0Target, mColor1Target);
	mFXCount++;
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderWidgets()
{
	size_t count;

	mWidgetProgram->Use();
	mWidgetProgram->Uniform("uProj", glm::ortho(0.0f, (float)mWidth, (float)mHeight, 0.0f, -1.0f, 1000.0f));
	mWidgetProgram->Uniform("uColor", glm::vec3(1.0f));

	mglEnable(MGL_DEPTH_TEST);
	mglEnable(MGL_BLEND);

	mglDepthMask(MGL_TRUE);
	mglDepthFunc(MGL_LEQUAL);
	mglClear(MGL_DEPTH_BUFFER_BIT);

	mglBlendFunc(MGL_SRC_ALPHA, MGL_ONE_MINUS_SRC_ALPHA);
		
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglEnableClientState(MGL_TEXTURE_COORD_ARRAY);
	
	// Render 2D widgets (simple textures)
	count = mFront->Widgets.size();
	while (count)
	{
		Texture *front = mFront->Widgets[0].texture;
		
		size_t j = 0;
		for (size_t i = 0; i < WIDGET_BATCH && i < count;)
		{
			WidgetRenderData *widget;
			
			widget = &mFront->Widgets[i];
			if (widget->texture != front)
			{
				++i;
				continue;
			}

			mWidgets[j * 6 + 0].x = widget->Position.x;
			mWidgets[j * 6 + 0].y = widget->Position.y;
			mWidgets[j * 6 + 0].z = widget->Z;
			mWidgets[j * 6 + 0].s = 0.0f;
			mWidgets[j * 6 + 0].t = 0.0f;
			mWidgets[j * 6 + 1].x = widget->Position.x;
			mWidgets[j * 6 + 1].y = widget->Position.y + widget->Size.y;
			mWidgets[j * 6 + 1].z = widget->Z;
			mWidgets[j * 6 + 1].s = 0.0f;
			mWidgets[j * 6 + 1].t = 1.0f;
			mWidgets[j * 6 + 2].x = widget->Position.x + widget->Size.x;
			mWidgets[j * 6 + 2].y = widget->Position.y + widget->Size.y;
			mWidgets[j * 6 + 2].z = widget->Z;
			mWidgets[j * 6 + 2].s = 1.0f;
			mWidgets[j * 6 + 2].t = 1.0f;
			
			mWidgets[j * 6 + 3].x = widget->Position.x;
			mWidgets[j * 6 + 3].y = widget->Position.y;
			mWidgets[j * 6 + 3].z = widget->Z;
			mWidgets[j * 6 + 3].s = 0.0f;
			mWidgets[j * 6 + 3].t = 0.0f;
			mWidgets[j * 6 + 4].x = widget->Position.x + widget->Size.x;
			mWidgets[j * 6 + 4].y = widget->Position.y + widget->Size.y;
			mWidgets[j * 6 + 4].z = widget->Z;
			mWidgets[j * 6 + 4].s = 1.0f;
			mWidgets[j * 6 + 4].t = 1.0f;
			mWidgets[j * 6 + 5].x = widget->Position.x + widget->Size.x;
			mWidgets[j * 6 + 5].y = widget->Position.y;
			mWidgets[j * 6 + 5].z = widget->Z;
			mWidgets[j * 6 + 5].s = 1.0f;
			mWidgets[j * 6 + 5].t = 0.0f;
		
			++j;
			std::swap(mFront->Widgets[i], mFront->Widgets[--count]);
		}

		if (j > 0 && front != NULL && front->IsReady())
		{
			mWidgetProgram->Uniform("uTexture", MGL_TEXTURE_2D, 0, front->mTexture);

			mglBindBuffer(MGL_ARRAY_BUFFER, mWidgetVBO);
			mglBufferData(MGL_ARRAY_BUFFER, WIDGET_BATCH * 6 * sizeof(Vertex), mWidgets, MGL_DYNAMIC_DRAW);
			mglVertexPointer(3, MGL_FLOAT, sizeof(Vertex), (void*)0);
			mglTexCoordPointer(2, MGL_FLOAT, sizeof(Vertex), (void*)12);

			mglDrawArrays(MGL_TRIANGLES, 0, j * 6);
		}
	}

	// Render 2D text
	count = mFront->Text.size();
	while (count)
	{
		TextRenderData *front = &mFront->Text[0];

		size_t j = 0;
		for (size_t i = 0; i < WIDGET_BATCH && i < count;)
		{
			TextRenderData *text;
			
			text = &mFront->Text[i];
			if (text->font != mFront->Text[0].font || j + text->Text.size() > WIDGET_BATCH)
			{
				++i;
				continue;
			}

			float offset = 0.0f;
			for (size_t k = 0; k < text->Text.size(); ++k)
			{
				Font::Glyph& glyph = text->font->GetGlyph(text->Text[k]);
								
				mWidgets[j * 6 + 0].x = text->Position.x + glyph.Offset.x + offset;
				mWidgets[j * 6 + 0].y = text->Position.y + glyph.Offset.y;
				mWidgets[j * 6 + 0].z = text->Z;
				mWidgets[j * 6 + 0].s = glyph.MapOffset.x;
				mWidgets[j * 6 + 0].t = glyph.MapOffset.y;

				mWidgets[j * 6 + 1].x = text->Position.x + glyph.Offset.x + offset;
				mWidgets[j * 6 + 1].y = text->Position.y + glyph.Offset.y + glyph.Size.y;
				mWidgets[j * 6 + 1].z = text->Z;
				mWidgets[j * 6 + 1].s = glyph.MapOffset.x;
				mWidgets[j * 6 + 1].t = glyph.MapOffset.y + glyph.MapSize.y;

				mWidgets[j * 6 + 2].x = text->Position.x + glyph.Offset.x + glyph.Size.x + offset;
				mWidgets[j * 6 + 2].y = text->Position.y + glyph.Offset.y + glyph.Size.y;
				mWidgets[j * 6 + 2].z = text->Z;
				mWidgets[j * 6 + 2].s = glyph.MapOffset.x + glyph.MapSize.x;
				mWidgets[j * 6 + 2].t = glyph.MapOffset.y + glyph.MapSize.y;
							
				mWidgets[j * 6 + 3].x = text->Position.x + glyph.Offset.x + offset;
				mWidgets[j * 6 + 3].y = text->Position.y + glyph.Offset.y;
				mWidgets[j * 6 + 3].z = text->Z;
				mWidgets[j * 6 + 3].s = glyph.MapOffset.x;
				mWidgets[j * 6 + 3].t = glyph.MapOffset.y;

				mWidgets[j * 6 + 4].x = text->Position.x + glyph.Offset.x + glyph.Size.x + offset;
				mWidgets[j * 6 + 4].y = text->Position.y + glyph.Offset.y + glyph.Size.y;
				mWidgets[j * 6 + 4].z = text->Z;
				mWidgets[j * 6 + 4].s = glyph.MapOffset.x + glyph.MapSize.x;
				mWidgets[j * 6 + 4].t = glyph.MapOffset.y + glyph.MapSize.y;

				mWidgets[j * 6 + 5].x = text->Position.x + glyph.Offset.x + glyph.Size.x + offset;
				mWidgets[j * 6 + 5].y = text->Position.y + glyph.Offset.y;
				mWidgets[j * 6 + 5].z = text->Z;
				mWidgets[j * 6 + 5].s = glyph.MapOffset.x + glyph.MapSize.x;
				mWidgets[j * 6 + 5].t = glyph.MapOffset.y;

				offset += glyph.Advance;
				++j;
			}

			std::swap(mFront->Text[i], mFront->Text[--count]);
		}

		if (j > 0 && front->font != NULL && front->font->IsReady())
		{
			mWidgetProgram->Uniform("uTexture", MGL_TEXTURE_2D, 0, front->font->mTexture);

			mglBindBuffer(MGL_ARRAY_BUFFER, mWidgetVBO);
			mglBufferData(MGL_ARRAY_BUFFER, WIDGET_BATCH * 6 * sizeof(Vertex), mWidgets, MGL_DYNAMIC_DRAW);
			mglVertexPointer(3, MGL_FLOAT, sizeof(Vertex), (void*)0);
			mglTexCoordPointer(2, MGL_FLOAT, sizeof(Vertex), (void*)12);

			mglDrawArrays(MGL_TRIANGLES, 0, j * 6);
		}
	}
	
	mglDisableClientState(MGL_TEXTURE_COORD_ARRAY);
	mglDisableClientState(MGL_VERTEX_ARRAY);

	mglDisable(MGL_BLEND);
	mglDisable(MGL_DEPTH_TEST);
}

// ------------------------------------------------------------------------------------------------
int Renderer::Worker()
{
	LARGE_INTEGER time;

	try 
	{
		wglMakeCurrent(mEngine->GetDC(), mContext);
		mglViewport(0, 0, mWidth, mHeight);
		
		Log::Inst() << "[Renderer] Thread started";

		while (IsRunning()) 
		{	
			{
				Mutex::ScopedLock lock(mBufferMutex);

				::QueryPerformanceCounter(&time);
				mTime = time.QuadPart * 1000.0f / mFreq.QuadPart;
                
				RenderGeometry();
				RenderPointlights();
				RenderSpotlights();
				RenderDirlights();
				RenderPostFX();
				RenderWidgets();
			}

			::SwapBuffers(mEngine->GetDC());
			mglFinish();
		}

		Log::Inst() << "[Renderer] Thread stopped";

		wglMakeCurrent(NULL, NULL);
		return 0;
	} 
	catch (std::exception& e) 
	{
		wglMakeCurrent(NULL, NULL);
		Log::Inst() << "[Renderer] Unhandled exception: " << e.what();
		mEngine->Quit();		
		return -1;
	}
}

