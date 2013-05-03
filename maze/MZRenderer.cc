// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZPlatform.h"
#include "MZException.h"
#include "MZLog.h"
#include "MZGL.h"
#include "MZProgram.h"
#include "MZEngine.h"
#include "MZRsmngr.h"
#include "MZRenderer.h"
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
const static float STATIC_MESH[] =
{
	0.0f, 0.0f, 0.0f
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
	  mInstanceProgram(NULL),
	  mSkyboxProgram(NULL),
	  mDirlightProgram(NULL),
	  mSpotlightProgram(NULL),
	  mPointlightProgram(NULL),
	  mVolumeProgram(NULL),
	  mDOFProgram(NULL),
	  mBlurProgram(NULL),
	  mGeomDiffuseTarget(NULL),
	  mGeomNormalTarget(NULL),
	  mGeomPositionTarget(NULL),
	  mColor0Target(NULL),
	  mColor1Target(NULL),
	  mShadowTarget(NULL),
	  mDepthTarget(NULL),
	  mRandomTexture(NULL),
	  mLightVBO(NULL),
	  mSkyboxVBO(NULL),
	  mQuadVBO(NULL),
	  mInstanceVBO(NULL),
	  mInstances(NULL),
	  mMapSize(1024),
	  mEngine(engine),
	  mContext(NULL)
{	
	mWidth = mEngine->GetConfig().WindowWidth;
	mHeight = mEngine->GetConfig().WindowHeight;

	mBuffers[0].Ready = false;
	mBuffers[1].Ready = false;
	mFront = &mBuffers[0];
}

// ------------------------------------------------------------------------------------------------
Renderer::~Renderer()
{
	Destroy();
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
	mglInit();
	mglEnable(MGL_TEXTURE_CUBE_MAP_SEAMLESS);

	Log::Inst() << "[Renderer] GL vendor:     " << (const char*)mglGetString(MGL_VENDOR);
	Log::Inst() << "[Renderer] GL version:    " << (const char*)mglGetString(MGL_VERSION);

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
	mColor0Target		= CreateTarget(MGL_RGBA16F, mWidth, mHeight);
	mColor1Target		= CreateTarget(MGL_RGBA16F, mWidth, mHeight);

	// Random texture
	std::vector<float> data;
	data.resize(64 * 64 * 3);
	for (int rand = 1234, i = 0; i < (int)data.size() / 3; i++)
	{
		float x = ((float)rand / 32749.0f);
		rand = rand * (32719 + 3) % 32749;

		float y = ((float)rand / 32749.0f);
		rand = rand * (32719 + 3) % 32749;

		float z = ((float)rand / 32749.0f);
		rand = rand * (32719 + 3) % 32749;

		float length = sqrt(x * x + y * y + z );
		data[i * 3 + 0] = x / length;
		data[i * 3 + 1] = y / length;
		data[i * 3 + 2] = z / length;
	}

	mglGenTextures(1, &mRandomTexture);
	mglBindTexture(MGL_TEXTURE_2D, mRandomTexture);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR);
	mglTexImage2D(MGL_TEXTURE_2D, 0, MGL_RGB, 64, 64, 0, MGL_RGB, MGL_FLOAT, &data[0]);
	mglBindTexture(MGL_TEXTURE_2D, 0);

	// Shadow map
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
	const std::string dir = mEngine->GetConfig().ResourceDir + "/shader/";

	mObjectProgram = new Program("object");
	mObjectProgram->Compile(Program::VERTEX, dir + "object.vert");
	mObjectProgram->Compile(Program::FRAGMENT, dir + "object.frag");
	mObjectProgram->Link();
	
	mInstanceProgram = new Program("instance");
	mInstanceProgram->Compile(Program::VERTEX, dir + "instance.vert");
	mInstanceProgram->Compile(Program::FRAGMENT, dir + "object.frag");
	mInstanceProgram->Link();
	
	mSkyboxProgram = new Program("skybox");
	mSkyboxProgram->Compile(Program::VERTEX, dir + "skybox.vert");
	mSkyboxProgram->Compile(Program::FRAGMENT, dir + "skybox.frag");
	mSkyboxProgram->Link();

	mDirlightProgram = new Program("dirlight");
	mDirlightProgram->Compile(Program::VERTEX, dir + "dirlight.vert");
	mDirlightProgram->Compile(Program::FRAGMENT, dir + "dirlight.frag");
	mDirlightProgram->Link();

	mSpotlightProgram = new Program("spotlight");
	mSpotlightProgram->Compile(Program::VERTEX, dir + "spotlight.vert");
	mSpotlightProgram->Compile(Program::FRAGMENT, dir + "spotlight.frag");
	mSpotlightProgram->Link();

	mPointlightProgram = new Program("pointlight");
	mPointlightProgram->Compile(Program::VERTEX, dir + "pointlight.vert");
	mPointlightProgram->Compile(Program::FRAGMENT, dir + "pointlight.frag");
	mPointlightProgram->Link();

	mVolumeProgram = new Program("volume");
	mVolumeProgram->Compile(Program::VERTEX, dir + "volume.vert");
	mVolumeProgram->Compile(Program::FRAGMENT, dir + "volume.frag");
	mVolumeProgram->Link();

	mDOFProgram = new Program("dof");
	mDOFProgram->Compile(Program::VERTEX, dir + "dof.vert");
	mDOFProgram->Compile(Program::FRAGMENT, dir + "dof.frag");
	mDOFProgram->Link();

	mBlurProgram = new Program("blur");
	mBlurProgram->Compile(Program::VERTEX, dir + "blur.vert");
	mBlurProgram->Compile(Program::FRAGMENT, dir + "blur.frag");
	mBlurProgram->Link();
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

	mInstances = new glm::mat4[INSTANCE_BATCH];

	mglGenBuffers(1, &mInstanceVBO);
	mglBindBuffer(MGL_ARRAY_BUFFER, mInstanceVBO);
	mglBufferData(MGL_ARRAY_BUFFER, INSTANCE_BATCH * sizeof(glm::mat4), mInstances, MGL_DYNAMIC_DRAW);

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
	if (mRandomTexture)		 { mglDeleteTextures(1, &mRandomTexture);	   mRandomTexture = 0;		}
}

// ------------------------------------------------------------------------------------------------
void Renderer::DestroyPrograms()
{
	if (mObjectProgram)		{ delete mObjectProgram;	 mObjectProgram		= NULL; }
	if (mInstanceProgram)	{ delete mInstanceProgram;	 mInstanceProgram	= NULL; }
	if (mSkyboxProgram)		{ delete mSkyboxProgram;	 mSkyboxProgram		= NULL; }
	if (mDirlightProgram)	{ delete mDirlightProgram;	 mDirlightProgram	= NULL; }
	if (mSpotlightProgram)	{ delete mSpotlightProgram;	 mSpotlightProgram	= NULL; }
	if (mPointlightProgram) { delete mPointlightProgram; mPointlightProgram = NULL; }
	if (mVolumeProgram)		{ delete mVolumeProgram;	 mVolumeProgram		= NULL; }
	if (mDOFProgram)		{ delete mDOFProgram;		 mDOFProgram		= NULL; }
	if (mBlurProgram)		{ delete mBlurProgram;		 mBlurProgram		= NULL; }
}

// ------------------------------------------------------------------------------------------------
void Renderer::DestroyBuffers()
{
	mglBindBuffer(MGL_ARRAY_BUFFER, 0);

	if (mLightVBO != NULL)		{ mglDeleteBuffers(1, &mLightVBO);		mLightVBO = NULL;	 }
	if (mSkyboxVBO != NULL)		{ mglDeleteBuffers(1, &mSkyboxVBO);		mLightVBO = NULL;	 }
	if (mQuadVBO != NULL)		{ mglDeleteBuffers(1, &mQuadVBO);		mLightVBO = NULL;	 }
	if (mInstanceVBO != NULL)	{ mglDeleteBuffers(1, &mInstanceVBO);	mInstanceVBO = NULL; }
	if (mInstances != NULL)		{ delete[] mInstances;					mInstances = NULL;	 }
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

	mFront->Ready = false;
	mFront->Clear();

	mFront = (mFront == &mBuffers[0]) ? &mBuffers[1] : &mBuffers[0];
	mFront->Ready = true;
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderGeometry()
{		
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
    mglFramebufferTexture2D(MGL_DRAW_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 3, MGL_TEXTURE_2D, mColor0Target, 0);

	mglDrawBuffers(4, &BUFFERS[0]);
	mglClear(MGL_COLOR_BUFFER_BIT);
		
	mglDrawBuffers(1, &BUFFERS[3]);	
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
	mglBindTexture(MGL_TEXTURE_CUBE_MAP, mFront->SkyTexture->mTexture);
	mSkyboxProgram->Uniform("uTexture", 0);

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
	glm::mat4 modelProj = mFront->Camera.ProjMatrix * mFront->Camera.ViewMatrix;

	mglEnable(MGL_DEPTH_TEST);
	mglEnable(MGL_CULL_FACE);
	mglCullFace(MGL_BACK);		
	mglDepthMask(MGL_TRUE);
	mglDepthFunc(MGL_LEQUAL);
	mglClear(MGL_DEPTH_BUFFER_BIT);
	
	mObjectProgram->Use();
	mObjectProgram->Uniform("uNear", mFront->Camera.NearPlane);
	mObjectProgram->Uniform("uFar", mFront->Camera.FarPlane);
	mObjectProgram->Uniform("uProj", mFront->Camera.ProjMatrix);
	mObjectProgram->Uniform("uView", mFront->Camera.ViewMatrix);
	mObjectProgram->Uniform("uPosition", mFront->Camera.Position);

	mInstanceProgram->Use();	
	mInstanceProgram->Uniform("uNear", mFront->Camera.NearPlane);
	mInstanceProgram->Uniform("uFar", mFront->Camera.FarPlane);
	mInstanceProgram->Uniform("uProj", mFront->Camera.ProjMatrix);
	mInstanceProgram->Uniform("uView", mFront->Camera.ViewMatrix);
	mInstanceProgram->Uniform("uPosition", mFront->Camera.Position);
	
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglEnableClientState(MGL_NORMAL_ARRAY);
	mglEnableClientState(MGL_TEXTURE_COORD_ARRAY);
		
	for (size_t i = 0; i < mFront->Objects.size(); ++i)
	{
		mFront->Objects[i].Rendered = false;
	}

	for (size_t i = 0; i < mFront->Objects.size(); ++i)
	{
		ObjectRenderData *base = &mFront->Objects[i];			
		Model *model = base->Model;	

		if (base->Rendered)
		{
			continue;
		}
			
		if (model && model->GetState() == Resource::LOADED && model->mMeshVBO)
		{
			// Check if there are enough objects for instancing
			size_t instanceCount = 0;
			for (size_t j = i; j < mFront->Objects.size() && instanceCount < INSTANCE_BATCH; ++j)
			{
				ObjectRenderData *second = &mFront->Objects[j];

				if (second->Rendered == false && second->Model == model)
				{
					mInstances[instanceCount++] = second->ModelMatrix;
					second->Rendered = true;
				}
			}

			if (instanceCount == 1)
			{	
				mObjectProgram->Use();
				mObjectProgram->Uniform("uModel", base->ModelMatrix);
				mObjectProgram->Uniform("uNormal", base->NormalMatrix);
				mObjectProgram->Uniform("uTexture", base->TextureMatrix);
				mObjectProgram->Uniform("uDiffuseMap", MGL_TEXTURE_2D, 0, model->mDiffuseMap->mTexture);
				mObjectProgram->Uniform("uNormalMap", MGL_TEXTURE_2D, 1, model->mBumpMap->mTexture);

				mglBindBuffer(MGL_ARRAY_BUFFER, base->Model->mMeshVBO);

				mglVertexPointer(3, MGL_FLOAT, sizeof(Model::Vertex), (void*)0);
				mglNormalPointer(MGL_FLOAT, sizeof(Model::Vertex), (void*)12);
				mglTexCoordPointer(2, MGL_FLOAT, sizeof(Model::Vertex), (void*)24);
			
				mglDrawArrays(MGL_TRIANGLES, 0, model->mVertexCount);
			}
			else
			{
				MGLuint index0 = mInstanceProgram->Attribute("aModel0");
				MGLuint index1 = mInstanceProgram->Attribute("aModel1");
				MGLuint index2 = mInstanceProgram->Attribute("aModel2");
				MGLuint index3 = mInstanceProgram->Attribute("aModel3");

				mInstanceProgram->Use();
				mInstanceProgram->Uniform("uNormal", base->NormalMatrix);
				mInstanceProgram->Uniform("uTexture", base->TextureMatrix);					
				mInstanceProgram->Uniform("uDiffuseMap", MGL_TEXTURE_2D, 0, model->mDiffuseMap->mTexture);
				mInstanceProgram->Uniform("uNormalMap", MGL_TEXTURE_2D, 1, model->mBumpMap->mTexture);
				
				mglBindBuffer(MGL_ARRAY_BUFFER, base->Model->mMeshVBO);
				mglVertexPointer(3, MGL_FLOAT, sizeof(Model::Vertex), (void*)0);
				mglNormalPointer(MGL_FLOAT, sizeof(Model::Vertex), (void*)12);
				mglTexCoordPointer(2, MGL_FLOAT, sizeof(Model::Vertex), (void*)24);
				
				mglBindBuffer(MGL_ARRAY_BUFFER, mInstanceVBO);				
				mglBufferData(MGL_ARRAY_BUFFER, INSTANCE_BATCH * sizeof(glm::mat4), mInstances, MGL_DYNAMIC_DRAW);

				mglEnableVertexAttribArray(index0);
				mglVertexAttribPointer(index0, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)0);
				mglVertexAttribDivisor(index0, 1);
				
				mglEnableVertexAttribArray(index1);
				mglVertexAttribPointer(index1, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)16);
				mglVertexAttribDivisor(index1, 1);
				
				mglEnableVertexAttribArray(index2);
				mglVertexAttribPointer(index2, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)32);
				mglVertexAttribDivisor(index2, 1);
								
				mglEnableVertexAttribArray(index3);
				mglVertexAttribPointer(index3, 4, MGL_FLOAT, MGL_FALSE, sizeof(glm::mat4), (void*)48);
				mglVertexAttribDivisor(index3, 1);

				mglDrawArraysInstanced(MGL_TRIANGLES, 0, model->mVertexCount, instanceCount);
				
				mglDisableVertexAttribArray(index0);
				mglDisableVertexAttribArray(index1);
				mglDisableVertexAttribArray(index2);
				mglDisableVertexAttribArray(index3);
			}
		}
	}

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
	glm::mat4 shadowMVP[4];
	glm::vec4 shadowZ;
	
	mglEnable(MGL_DEPTH_TEST);
	mglEnableClientState(MGL_VERTEX_ARRAY);

	for (size_t i = 0; i < mFront->Lights.size(); ++i)
	{
		LightRenderData* light = &mFront->Lights[i];
				
		if (light->Type == Light::DIRECTIONAL)
		{
			mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mShadowFBO);	
			mglViewport(0, 0, mMapSize, mMapSize);
			
			if (light->CastsShadows)
			{
				mglEnable(MGL_CULL_FACE);
				mglEnable(MGL_POLYGON_OFFSET_FILL);

				mglDepthMask(MGL_TRUE);
				mglDepthFunc(MGL_LEQUAL);
				mglCullFace(MGL_BACK);	
				mglPolygonOffset(1.0f, 4.0f);

				mVolumeProgram->Use();
				for (size_t i = 0; i < 4; ++i)
				{
					mglFramebufferTextureLayer(MGL_DRAW_FRAMEBUFFER, MGL_DEPTH_ATTACHMENT, mShadowTarget, 0, i);
					mglClear(MGL_DEPTH_BUFFER_BIT);
				
					shadowMVP[i] = DEPTH_BIAS * light->Shadow[i].MVP;
					shadowZ[i] = light->Shadow[i].NearZ;
				
					for (size_t j = 0; j < light->Shadow[i].Count; ++j)
					{
						ObjectRenderData* obj = &mFront->ShadowCasters[light->Shadow[i].Index + j];		
						if (obj->Model && obj->Model->GetState() == Resource::LOADED && obj->Model->mMeshVBO)
						{
							mVolumeProgram->Uniform("uMVP", light->Shadow[i].MVP * obj->ModelMatrix);
							mglBindBuffer(MGL_ARRAY_BUFFER, obj->Model->mMeshVBO);
							mglVertexPointer(3, MGL_FLOAT, sizeof(Model::Vertex), 0);
							mglDrawArrays(MGL_TRIANGLES, 0, obj->Model->mVertexCount);
						}
					}
				}

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

			mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mGeometryFBO);
			mglDrawBuffer(MGL_COLOR_ATTACHMENT + 3);
			mglViewport(0, 0, mWidth, mHeight);
	
			mDirlightProgram->Use();
			mDirlightProgram->Uniform("uGDiffuse",	MGL_TEXTURE_2D, 0, mGeomDiffuseTarget);
			mDirlightProgram->Uniform("uGNormal",	MGL_TEXTURE_2D, 1, mGeomNormalTarget);
			mDirlightProgram->Uniform("uGPosition",	MGL_TEXTURE_2D, 2, mGeomPositionTarget);
			mDirlightProgram->Uniform("uShadow",    MGL_TEXTURE_2D_ARRAY, 4, mShadowTarget);
			mDirlightProgram->Uniform("uShadowMVP0", shadowMVP[0]);
			mDirlightProgram->Uniform("uShadowMVP1", shadowMVP[1]);
			mDirlightProgram->Uniform("uShadowMVP2", shadowMVP[2]);
			mDirlightProgram->Uniform("uShadowMVP3", shadowMVP[3]);	
			mDirlightProgram->Uniform("uShadowZ", shadowZ);
			mDirlightProgram->Uniform("uEyePosition", mFront->Camera.Position);
			mDirlightProgram->Uniform("lDiffuse", light->Diffuse);
			mDirlightProgram->Uniform("lSpecular", light->Specular);
			mDirlightProgram->Uniform("lAmbient", light->Ambient);
			mDirlightProgram->Uniform("lDirection", light->Direction);
			
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
	RenderDOF();

	// Copy the color target to the fbo
	mglBindFramebuffer(MGL_READ_FRAMEBUFFER, mPostFXFBO);
	mglReadBuffer(MGL_COLOR_ATTACHMENT + 0);
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, 0);	
	mglBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, MGL_COLOR_BUFFER_BIT, MGL_NEAREST);
}

// ------------------------------------------------------------------------------------------------
void Renderer::RenderDOF()
{
	mglBindFramebuffer(MGL_DRAW_FRAMEBUFFER, mPostFXFBO);
    mglFramebufferTexture2D(MGL_FRAMEBUFFER, MGL_COLOR_ATTACHMENT + 0, MGL_TEXTURE_2D, mColor1Target, 0);
	mglDrawBuffer(MGL_COLOR_ATTACHMENT + 0);
	mglClear(MGL_COLOR_BUFFER_BIT);

	mDOFProgram->Use();
	mDOFProgram->Uniform("uWidth", mWidth);
	mDOFProgram->Uniform("uHeight",	mHeight);
	mDOFProgram->Uniform("uFocus",	mFront->Camera.Focus);
	mDOFProgram->Uniform("uPosition", MGL_TEXTURE_2D, 1, mGeomPositionTarget);
	mDOFProgram->Uniform("uColor", MGL_TEXTURE_2D, 2, mColor0Target);
		
	mglEnableClientState(MGL_VERTEX_ARRAY);
	mglBindBuffer(MGL_ARRAY_BUFFER, mQuadVBO);
	mglVertexPointer(2, MGL_FLOAT, 2 * sizeof(float), 0);
	mglDrawArrays(MGL_TRIANGLES, 0, 6);		
	mglDisableClientState(MGL_VERTEX_ARRAY);

	std::swap(mColor0Target, mColor1Target);
}

// ------------------------------------------------------------------------------------------------
int Renderer::Worker()
{
	try 
	{
		wglMakeCurrent(mEngine->GetDC(), mContext);
		mglViewport(0, 0, mWidth, mHeight);
		
		while (IsRunning()) 
		{	
			if (!mFront->Ready)
			{
				continue;
			}

			{
				Mutex::ScopedLock lock(mBufferMutex);

				RenderGeometry();
				RenderPointlights();
				RenderSpotlights();
				RenderDirlights();
				RenderPostFX();
			}

			::SwapBuffers(mEngine->GetDC());
		}

		wglMakeCurrent(NULL, NULL);
		return 0;
	} 
	catch (std::exception& e) 
	{
		wglMakeCurrent(NULL, NULL);
		Log::Inst() << "Renderer exception: " << e.what();
		mEngine->Quit();		
		return -1;
	}
}

