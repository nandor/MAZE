// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

#define GET_ADDR(func)\
	m ## func = (m ## func ## Proc)wglGetProcAddress(#func);\
	if (m ## func == NULL)\
	{\
		return 0;\
	}
	
#define GET_ADDR_ARB(func)\
	m ## func = (m ## func ## Proc)wglGetProcAddress(#func "ARB");\
	if (m ## func == NULL)\
	{\
		return 0;\
	}
	
#define GET_ADDR_EXT(func)\
	m ## func = (m ## func ## Proc)wglGetProcAddress(#func "EXT");\
	if (m ## func == NULL)\
	{\
		return 0;\
	}

// GL State -------------------------------------------------------------------
GLState GL;

// Core -----------------------------------------------------------------------
mglGetErrorProc								mglGetError;
mglClearProc								mglClear;
mglClearColorProc							mglClearColor;
mglGetIntegervProc							mglGetIntegerv;
mglGetFloatvProc							mglGetFloatv;
mglDepthMaskProc							mglDepthMask;
mglColorMaskProc							mglColorMask;
mglEnableProc								mglEnable;
mglDisableProc								mglDisable;
mglClearDepthProc							mglClearDepth;
mglDepthFuncProc							mglDepthFunc;
mglFrontFaceProc							mglFrontFace;
mglFlushProc								mglFlush;
mglFinishProc								mglFinish;
mglGetStringProc							mglGetString;
mglStencilFuncProc							mglStencilFunc;
mglStencilOpProc							mglStencilOp;
mglStencilMaskProc							mglStencilMask;
mglCullFaceProc								mglCullFace;
mglBlendFuncProc							mglBlendFunc;
mglClearStencilProc							mglClearStencil;
mglPolygonOffsetProc						mglPolygonOffset;
mglUseProgramProc							mglUseProgram;
mglCreateShaderProc							mglCreateShader;
mglCreateProgramProc						mglCreateProgram;
mglShaderSourceProc							mglShaderSource;
mglCompileShaderProc						mglCompileShader;
mglGetShaderivProc							mglGetShaderiv;
mglGetProgramivProc							mglGetProgramiv;
mglGetShaderInfoLogProc						mglGetShaderInfoLog;
mglGetProgramInfoLogProc					mglGetProgramInfoLog;
mglAttachShaderProc							mglAttachShader;
mglDetachShaderProc							mglDetachShader;
mglLinkProgramProc							mglLinkProgram;
mglValidateProgramProc						mglValidateProgram;
mglGetAttachedShadersProc					mglGetAttachedShaders;
mglDeleteShaderProc							mglDeleteShader;
mglDeleteProgramProc						mglDeleteProgram;
mglViewportProc								mglViewport;
mglGetActiveUniformProc						mglGetActiveUniform;
mglGetActiveAttribProc						mglGetActiveAttrib;
mglGetUniformLocationProc					mglGetUniformLocation;
mglGetAttribLocationProc					mglGetAttribLocation;
mglUniform1iProc							mglUniform1i;
mglUniform1fProc							mglUniform1f;
mglUniform1fvProc							mglUniform1fv;
mglUniform3fvProc							mglUniform3fv;
mglUniform4fvProc							mglUniform4fv;
mglUniformMatrix3fvProc						mglUniformMatrix2fv;
mglUniformMatrix3fvProc						mglUniformMatrix3fv;
mglUniformMatrix4fvProc						mglUniformMatrix4fv;
mglGenTexturesProc							mglGenTextures;
mglBindTextureProc							mglBindTexture;
mglTexParameterfProc						mglTexParameterf;
mglTexParameteriProc						mglTexParameteri;
mglTexSubImage2DProc						mglTexSubImage2D;
mglTexImage2DProc							mglTexImage2D;
mglTexImage3DProc							mglTexImage3D;
mglDeleteTexturesProc						mglDeleteTextures;
mglActiveTextureProc						mglActiveTexture;
mglGenerateMipmapProc						mglGenerateMipmap;
mglVertexPointerProc						mglVertexPointer;
mglNormalPointerProc						mglNormalPointer;
mglEnableClientStateProc					mglEnableClientState;
mglDisableClientStateProc					mglDisableClientState;
mglDrawArraysProc							mglDrawArrays;
mglDrawElementsProc							mglDrawElements;
mglTexCoordPointerProc						mglTexCoordPointer;
mglGenBuffersProc							mglGenBuffers;
mglDeleteBuffersProc						mglDeleteBuffers;
mglBindBufferProc							mglBindBuffer;
mglBufferSubDataProc						mglBufferSubData;
mglBufferDataProc							mglBufferData;
mglMapBufferProc							mglMapBuffer;
mglUnmapBufferProc							mglUnmapBuffer;
mglEnableVertexAttribArrayProc				mglEnableVertexAttribArray;
mglDisableVertexAttribArrayProc				mglDisableVertexAttribArray;
mglVertexAttribPointerProc					mglVertexAttribPointer;
mglVertexAttribDivisorProc					mglVertexAttribDivisor;
mglGenVertexArraysProc						mglGenVertexArrays;
mglDeleteVertexArraysProc					mglDeleteVertexArrays;
mglBindVertexArrayProc						mglBindVertexArray;
mglDrawArraysInstancedProc					mglDrawArraysInstanced;
mglDrawElementsInstancedProc				mglDrawElementsInstanced;
mglIsRenderbufferProc						mglIsRenderbuffer;
mglIsFramebufferProc						mglIsFramebuffer;
mglCheckFramebufferStatusProc				mglCheckFramebufferStatus;
mglFramebufferTextureLayerProc				mglFramebufferTextureLayer;
mglGenFramebuffersProc						mglGenFramebuffers;
mglDeleteFramebuffersProc					mglDeleteFramebuffers;
mglBindFramebufferProc						mglBindFramebuffer;
mglFramebufferTexture2DProc					mglFramebufferTexture2D;
mglFramebufferRenderbufferProc				mglFramebufferRenderbuffer;
mglGenRenderbuffersProc						mglGenRenderbuffers;
mglDeleteRenderbuffersProc					mglDeleteRenderbuffers;
mglBindRenderbufferProc						mglBindRenderbuffer;
mglRenderbufferStorageProc					mglRenderbufferStorage;
mglDrawBuffersProc							mglDrawBuffers;
mglDrawBufferProc							mglDrawBuffer;
mglReadBufferProc							mglReadBuffer;
mglBlitFramebufferProc						mglBlitFramebuffer;
mglGetProgramBinaryProc						mglGetProgramBinary;
mglProgramBinaryProc						mglProgramBinary;
mglProgramParameteriProc					mglProgramParameteri;
mwglSwapIntervalProc						mwglSwapInterval;
mwglGetSwapIntervalProc						mwglGetSwapInterval;

// ------------------------------------------------------------------------------------------------
int mglIsSupported(const char *ext)
{
	const char *start = GL.ExtList, *pos, *end;

	if (*ext == '\0' || strchr(ext, ' '))
		return 0;

	while (start)
	{
		if ((pos = strstr(start, ext)) == NULL)
			return 0;

		end = pos + strlen(ext);
		if ((pos == start || *(pos - 1) == ' ') && (*end == ' ' || *end == '\0'))
			return 1;

		start = end;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
float mglGetMaxAnisotropy()
{
	return GL.Anisotropy;
}

// ------------------------------------------------------------------------------------------------
int mglInit()
{
	// Retrieve version number
	glGetIntegerv(MGL_MAJOR_VERSION, &GL.MajorVersion);
	glGetIntegerv(MGL_MINOR_VERSION, &GL.MinorVersion);
	
	// OpenGL 3.3 is required
	if (GL.MajorVersion < 2 || (GL.MajorVersion == 2 && GL.MinorVersion < 1))
	{
		return 0;
	}
	
	// Retrieve extension list
	GL.ExtList = (const char*)glGetString(GL_EXTENSIONS);

	// Core functions, simply alias them
	mglGetError				= glGetError;
	mglGetString			= glGetString;
	mglGetIntegerv			= glGetIntegerv;
	mglGetFloatv			= glGetFloatv;
	mglEnable				= glEnable;
	mglDisable				= glDisable;
	mglClear				= glClear;
	mglClearColor			= glClearColor;
	mglViewport				= glViewport;
	mglDepthMask			= glDepthMask;	
	mglColorMask			= glColorMask;
	mglClearDepth			= glClearDepth;
	mglDepthFunc			= glDepthFunc;
	mglFrontFace			= glFrontFace;
	mglFlush				= glFlush;
	mglFinish				= glFinish;
	mglTexParameterf		= glTexParameterf;
	mglTexParameteri		= glTexParameteri;
	mglTexImage2D			= glTexImage2D;
	mglTexSubImage2D		= glTexSubImage2D;
	mglGenTextures			= glGenTextures;
	mglBindTexture			= glBindTexture;
	mglDeleteTextures		= glDeleteTextures;
	mglVertexPointer		= glVertexPointer;
	mglNormalPointer		= glNormalPointer;
	mglTexCoordPointer		= glTexCoordPointer;
	mglDrawBuffer			= glDrawBuffer;
	mglReadBuffer			= glReadBuffer;
	mglEnableClientState	= glEnableClientState;
	mglDisableClientState	= glDisableClientState;
	mglStencilFunc			= glStencilFunc;
	mglStencilOp			= glStencilOp;
	mglStencilMask			= glStencilMask;
	mglBlendFunc			= glBlendFunc;
	mglCullFace				= glCullFace;
	mglBlendFunc			= glBlendFunc;
	mglClearStencil			= glClearStencil;
	mglPolygonOffset		= glPolygonOffset;
	mglDrawArrays			= glDrawArrays;
	mglDrawElements			= glDrawElements;

	GET_ADDR(glGenerateMipmap);
	GET_ADDR(glUseProgram);
	GET_ADDR(glCreateProgram);
	GET_ADDR(glGetProgramiv);
	GET_ADDR(glGetProgramInfoLog);
	GET_ADDR(glLinkProgram);
	GET_ADDR(glValidateProgram);
	GET_ADDR(glGetAttachedShaders);
	GET_ADDR(glDeleteProgram);
	GET_ADDR(glCreateShader);
	GET_ADDR(glShaderSource);
	GET_ADDR(glCompileShader);
	GET_ADDR(glGetShaderInfoLog);
	GET_ADDR(glGetShaderiv);
	GET_ADDR(glAttachShader);
	GET_ADDR(glDetachShader);
	GET_ADDR(glDeleteShader);
	GET_ADDR(glGetActiveUniform);
	GET_ADDR(glGetActiveAttrib);
	GET_ADDR(glGetUniformLocation);
	GET_ADDR(glGetAttribLocation);
	GET_ADDR(glUniform1i);
	GET_ADDR(glUniform1f);
	GET_ADDR(glUniform1fv);
	GET_ADDR(glUniform3fv);
	GET_ADDR(glUniform4fv);
	GET_ADDR(glUniformMatrix2fv);
	GET_ADDR(glUniformMatrix3fv);
	GET_ADDR(glUniformMatrix4fv);
	GET_ADDR(glActiveTexture);
	GET_ADDR(glTexImage3D);
	GET_ADDR(glGenBuffers);
	GET_ADDR(glDeleteBuffers);
	GET_ADDR(glBindBuffer);
	GET_ADDR(glBufferSubData);
	GET_ADDR(glBufferData);
	GET_ADDR(glMapBuffer);
	GET_ADDR(glUnmapBuffer);
	GET_ADDR(glEnableVertexAttribArray);
	GET_ADDR(glDisableVertexAttribArray);
	GET_ADDR(glVertexAttribPointer);
	GET_ADDR(glVertexAttribDivisor);
	GET_ADDR(glGenVertexArrays);
	GET_ADDR(glDeleteVertexArrays);
	GET_ADDR(glBindVertexArray);

	mwglSwapInterval = (mwglSwapIntervalProc)wglGetProcAddress("wglSwapIntervalEXT");
	mwglGetSwapInterval = (mwglGetSwapIntervalProc)wglGetProcAddress("wglGetSwapIntervalEXT");

	GL.VSync = (!mwglSwapInterval || !mwglGetSwapInterval) ? 0 : 1;

	if (mglIsSupported("GL_ARB_draw_instanced"))
	{
		GET_ADDR_ARB(glDrawArraysInstanced);
		GET_ADDR_ARB(glDrawElementsInstanced);
		GL.Instancing = 1;
	}
	else if (mglIsSupported("GL_EXT_draw_instanced"))
	{
		GET_ADDR_EXT(glDrawArraysInstanced);
		GET_ADDR_EXT(glDrawElementsInstanced);
		GL.Instancing = 1;
	}
	else
	{
		GL.Instancing = 0;
		return 0;
	}

	if (mglIsSupported("GL_ARB_framebuffer_object"))
	{
		GET_ADDR(glGenFramebuffers);
		GET_ADDR(glDeleteFramebuffers);
		GET_ADDR(glBindFramebuffer);
		GET_ADDR(glIsFramebuffer);
		GET_ADDR(glFramebufferTexture2D);
		GET_ADDR(glFramebufferRenderbuffer);
		GET_ADDR(glCheckFramebufferStatus);
		GET_ADDR(glFramebufferTextureLayer);
		GET_ADDR(glGenRenderbuffers);
		GET_ADDR(glDeleteRenderbuffers);
		GET_ADDR(glIsRenderbuffer);
		GET_ADDR(glBindRenderbuffer);
		GET_ADDR(glRenderbufferStorage);
		GET_ADDR(glBlitFramebuffer);
		GET_ADDR_ARB(glDrawBuffers);
		GL.FBO = 1;
	}
	else if (mglIsSupported("GL_EXT_framebuffer_object"))
	{
		GET_ADDR_EXT(glGenFramebuffers);
		GET_ADDR_EXT(glDeleteFramebuffers);
		GET_ADDR_EXT(glBindFramebuffer);
		GET_ADDR_EXT(glIsFramebuffer);
		GET_ADDR_EXT(glFramebufferTexture2D);
		GET_ADDR_EXT(glFramebufferRenderbuffer);
		GET_ADDR_EXT(glCheckFramebufferStatus);
		GET_ADDR_EXT(glFramebufferTextureLayer);
		GET_ADDR_EXT(glGenRenderbuffers);
		GET_ADDR_EXT(glDeleteRenderbuffers);
		GET_ADDR_EXT(glIsRenderbuffer);
		GET_ADDR_EXT(glBindRenderbuffer);
		GET_ADDR_EXT(glRenderbufferStorage);
		GET_ADDR_EXT(glDrawBuffers);
		GET_ADDR_EXT(glBlitFramebuffer);
		GET_ADDR_EXT(glDrawBuffers);
		GL.FBO = 1;	
	}
	else
	{
		GL.FBO = 0;	
		return 0;
	}
		
	if (mglIsSupported("GL_ARB_get_program_binary"))
	{
		GET_ADDR(glGetProgramBinary);
		GET_ADDR(glProgramBinary);
		GET_ADDR(glProgramParameteri);
		GL.SupportCache = 1;
	}
	else
	{
		GL.SupportCache = 0;
	}

	// Anisotropic filtering is a plus	
	GL.Anisotropy = 0.0f;
	if (mglIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		mglGetFloatv(MGL_MAX_TEXTURE_MAX_ANISOTROPY, &GL.Anisotropy);
	}

	return 1;
}
