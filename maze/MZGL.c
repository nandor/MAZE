// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
#include "MZGL.h"
#include <stdio.h>
#include <gl/GL.h>

#ifdef WIN32
#	define GET_ADDR(func) m ## func = (m ## func ## Proc)wglGetProcAddress(#func)
#else
#	error "GLX is not supported yet"
#endif

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

// Shaders --------------------------------------------------------------------
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
mglGetUniformLocationProc					mglGetUniformLocation;
mglUniform1iProc							mglUniform1i;
mglUniform1fProc							mglUniform1f;
mglUniform1fvProc							mglUniform1fv;
mglUniform3fvProc							mglUniform3fv;
mglUniform4fvProc							mglUniform4fv;
mglUniformMatrix3fvProc						mglUniformMatrix2fv;
mglUniformMatrix3fvProc						mglUniformMatrix3fv;
mglUniformMatrix4fvProc						mglUniformMatrix4fv;

// Textures ---- --------------------------------------------------------------
mglGenTexturesProc							mglGenTextures;
mglBindTextureProc							mglBindTexture;
mglTexParameterfProc						mglTexParameterf;
mglTexParameteriProc						mglTexParameteri;
mglTexImage2DProc							mglTexImage2D;
mglTexImage3DProc							mglTexImage3D;
mglDeleteTexturesProc						mglDeleteTextures;
mglActiveTextureProc						mglActiveTexture;
mglGenerateMipmapProc						mglGenerateMipmap;
mglTexImage2DMultisampleProc				mglTexImage2DMultisample;

// Vertex Arrays --------------------------------------------------------------
mglVertexPointerProc						mglVertexPointer;
mglNormalPointerProc						mglNormalPointer;
mglEnableClientStateProc					mglEnableClientState;
mglDisableClientStateProc					mglDisableClientState;
mglDrawArraysProc							mglDrawArrays;
mglTexCoordPointerProc						mglTexCoordPointer;
mglGenBuffersProc							mglGenBuffers;
mglDeleteBuffersProc						mglDeleteBuffers;
mglBindBufferProc							mglBindBuffer;
mglBufferSubDataProc						mglBufferSubData;
mglBufferDataProc							mglBufferData;
mglMapBufferProc							mglMapBuffer;
mglUnmapBufferProc							mglUnmapBuffer;

// Framebuffer object ---------------------------------------------------------
mglIsRenderbufferProc						mglIsRenderbuffer;
mglIsFramebufferProc						mglIsFramebuffer;
mglCheckFramebufferStatusProc				mglCheckFramebufferStatus;
mglFramebufferTextureLayerProc				mglFramebufferTextureLayer;
mglGenFramebuffersProc						mglGenFramebuffers;
mglDeleteFramebuffersProc					mglDeleteFramebuffers;
mglBindFramebufferProc						mglBindFramebuffer;
mglGetFramebufferAttachmentParameterivProc	mglGetFramebufferAttachmentParameteriv;
mglFramebufferTexture2DProc					mglFramebufferTexture2D;
mglFramebufferRenderbufferProc				mglFramebufferRenderbuffer;
mglGenRenderbuffersProc						mglGenRenderbuffers;
mglDeleteRenderbuffersProc					mglDeleteRenderbuffers;
mglBindRenderbufferProc						mglBindRenderbuffer;
mglRenderbufferStorageProc					mglRenderbufferStorage;
mglGetRenderbufferParameterivProc			mglGetRenderbufferParameteriv;
mglDrawBuffersProc							mglDrawBuffers;
mglDrawBufferProc							mglDrawBuffer;
mglReadBufferProc							mglReadBuffer;
mglBlitFramebufferProc						mglBlitFramebuffer;

// ------------------------------------------------------------------------------------------------
GLState GL;

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
int mglInit()
{
	const char* ver;

	// Retrieve version number
	ver = (const char*)glGetString(GL_VERSION);
	if (sscanf(ver, "%d.%d", &GL.MajorVersion, &GL.MinorVersion) != 2)
	{
		GL.MajorVersion = 1; 
		GL.MinorVersion = 0;
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

	// OpenGL 1.4 is required
	if (GL.MajorVersion < 1 || (GL.MajorVersion == 1 && GL.MinorVersion < 4))
		return 0;

	GET_ADDR(glGenerateMipmap);
	
	// Vertex & fragment shaders are needed
	if ((!mglIsSupported("GL_ARB_vertex_shader") || 
		 !mglIsSupported("GL_ARB_fragment_shader")) && 
		(!mglIsSupported("GL_EXT_vertex_shader") ||
		 !mglIsSupported("GL_EXT_fragment_shader")))
		return 0;

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
	GET_ADDR(glGetUniformLocation);

	GET_ADDR(glUniform1i);
	GET_ADDR(glUniform1f);
	GET_ADDR(glUniform1fv);
	GET_ADDR(glUniform3fv);
	GET_ADDR(glUniform4fv);
	GET_ADDR(glUniformMatrix2fv);
	GET_ADDR(glUniformMatrix3fv);
	GET_ADDR(glUniformMatrix4fv);
	
	// Multitexturing was included in 2.0
	GET_ADDR(glActiveTexture);
	GET_ADDR(glTexImage3D);

	// VBOs are supported by lots of cards
	if (!mglIsSupported("GL_ARB_vertex_buffer_object"))
		return 0;

	GET_ADDR(glGenBuffers);
	GET_ADDR(glDeleteBuffers);
	GET_ADDR(glBindBuffer);
	GET_ADDR(glBufferSubData);
	GET_ADDR(glBufferData);
	GET_ADDR(glMapBuffer);
	GET_ADDR(glUnmapBuffer);
	GET_ADDR(glDrawArrays);
	
	// FBOs are supported by nearly all the cards which support OpenGL 2.0
	if (!mglIsSupported("GL_ARB_framebuffer_object") && 
		!mglIsSupported("GL_EXT_framebuffer_object"))
		return 0;
	
	GET_ADDR(glGenFramebuffers);
	GET_ADDR(glDeleteFramebuffers);
	GET_ADDR(glBindFramebuffer);
	GET_ADDR(glIsFramebuffer);
	GET_ADDR(glFramebufferTexture2D);
	GET_ADDR(glFramebufferRenderbuffer);
	GET_ADDR(glCheckFramebufferStatus);
	GET_ADDR(glGetFramebufferAttachmentParameteriv);
	GET_ADDR(glFramebufferTextureLayer);

	GET_ADDR(glGenRenderbuffers);
	GET_ADDR(glDeleteRenderbuffers);
	GET_ADDR(glIsRenderbuffer);
	GET_ADDR(glBindRenderbuffer);
	GET_ADDR(glRenderbufferStorage);
	GET_ADDR(glGetRenderbufferParameteriv);
	GET_ADDR(glBlitFramebuffer);

	if (!mglIsSupported("GL_ARB_draw_buffers"))
		return 0;

	GET_ADDR(glDrawBuffers);

	// Anisotropic filtering is a plus	
	GL.Anisotropy = 0.0f;
	if (mglIsSupported("GL_EXT_texture_filter_anisotropic"))
		mglGetFloatv(MGL_MAX_TEXTURE_MAX_ANISOTROPY, &GL.Anisotropy);
	
	// Multisampling
	GL.Multisample = 0;
	if (mglIsSupported("GL_ARB_texture_multisample"))
		GL.Multisample = 1;
	
	GET_ADDR(glTexImage2DMultisample);

	// Stencil buffer
	mglGetIntegerv(MGL_STENCIL_BITS, &GL.StencilBits);

	return 1;
}
