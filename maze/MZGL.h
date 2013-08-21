// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MGL_H
#define MGL_H

#ifdef APIENTRY
#	define MZAPIENTRY APIENTRY
#else
#	define MZAPIENTRY __stdcall
#endif

// ------------------------------------------------------------------------------------------------ 
typedef char			MGLchar;
typedef unsigned int	MGLenum;
typedef unsigned int	MGLbitfield;
typedef unsigned int	MGLuint;
typedef int				MGLint;
typedef int				MGLsizei;
typedef unsigned char	MGLboolean;
typedef signed char		MGLbyte;
typedef short			MGLshort;
typedef unsigned char	MGLubyte;
typedef unsigned short	MGLushort;
typedef unsigned long	MGLulong;
typedef float			MGLfloat;
typedef float			MGLclampf;
typedef double			MGLdouble;
typedef double			MGLclampd;
typedef void			MGLvoid;
typedef ptrdiff_t		MGLintptr;
typedef ptrdiff_t 		MGLsizeiptr;
	
// ------------------------------------------------------------------------------------------------ 
typedef struct
{
	/// List of OpenGL extensions
	const char *ExtList;
	
	/// Major version
	MGLint MajorVersion;

	/// Minor version
	MGLint MinorVersion;
	
	/// Max anisotropy supported
	MGLfloat Anisotropy;

	/// Program binary cacheing
	MGLboolean SupportCache;

	/// Occlusion queries
	MGLboolean SupportQueries;

	/// Instancing
	MGLboolean Instancing;

	/// FBO
	MGLboolean FBO;

	/// Vertical sync
	MGLboolean VSync;

} GLState;

extern GLState GL;
	
// ------------------------------------------------------------------------------------------------ 
#define MWGL_CONTEXT_MAJOR_VERSION				0x2091
#define MWGL_CONTEXT_MINOR_VERSION				0x2092
#define MWGL_CONTEXT_LAYER_PLANE				0x2093
#define MWGL_CONTEXT_FLAGS						0x2094
#define MWGL_CONTEXT_PROFILE_MASK				0x9126	
#define MWGL_CONTEXT_DEBUG_BIT					0x0001
#define MWGL_CONTEXT_FORWARD_COMPATIBLE_BIT		0x0002
#define MWGL_CONTEXT_CORE_PROFILE_BIT			0x00000001
#define MWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT	0x00000002    

#define MGL_FALSE								0x0000
#define MGL_NO_ERROR							0x0000
#define MGL_ZERO								0x0000
#define MGL_NONE								0x0000
#define MGL_TRUE								0x0001
#define MGL_ONE									0x0001
#define MGL_TRIANGLES							0x0004
#define MGL_QUADS								0x0007
#define MGL_NEVER								0x0200
#define MGL_LESS								0x0201
#define MGL_EQUAL								0x0202
#define MGL_LEQUAL								0x0203
#define MGL_GREATER								0x0204
#define MGL_NOTEQUAL							0x0205
#define MGL_GEQUAL								0x0206
#define MGL_ALWAYS								0x0207
#define MGL_SRC_ALPHA							0x0302
#define MGL_ONE_MINUS_SRC_ALPHA					0x0303
#define MGL_FRONT								0x0404
#define MGL_BACK								0x0405
#define MGL_CW									0x0900
#define MGL_CCW									0x0901
#define MGL_CULL_FACE							0x0B44
#define MGL_DEPTH_TEST							0x0B71
#define MGL_DEPTH_FUNC							0x0B74
#define MGL_STENCIL_TEST						0x0B90
#define MGL_VIEWPORT							0x0BA2
#define MGL_BLEND_DST							0x0BE0
#define MGL_BLEND_SRC							0x0BE1
#define MGL_BLEND								0x0BE2
#define MGL_STENCIL_BITS						0x0D57
#define MGL_TEXTURE_1D							0x0DE0
#define MGL_TEXTURE_2D							0x0DE1
#define MGL_INVALID_ENUM						0x0500
#define MGL_INVALID_VALUE						0x0501
#define MGL_INVALID_OPERATION					0x0502
#define MGL_STACK_OVERFLOW						0x0503
#define MGL_STACK_UNDERFLOW						0x0504
#define MGL_OUT_OF_MEMORY						0x0505
#define MGL_INVALID_FRAMEBUFFER_OPERATION		0x0506
#define MGL_BYTE								0x1400
#define MGL_UNSIGNED_BYTE						0x1401
#define MGL_SHORT					            0x1402
#define MGL_UNSIGNED_SHORT					    0x1403
#define MGL_INT						            0x1404
#define MGL_UNSIGNED_INT				        0x1405
#define MGL_FLOAT						        0x1406
#define MGL_DOUBLE					            0x140A
#define MGL_DEPTH_COMPONENT						0x1902
#define MGL_RGB									0x1907
#define MGL_RGBA								0x1908
#define MGL_LUMINANCE							0x1909
#define MGL_LUMINANCE_ALPHA						0x190A
#define MGL_KEEP								0x1E00
#define MGL_REPLACE								0x1E01
#define MGL_INCR								0x1E02
#define MGL_DECR								0x1E03
#define MGL_VENDOR								0x1F00
#define MGL_RENDERER							0x1F01
#define MGL_VERSION								0x1F02
#define MGL_EXTENSIONS							0x1F03
#define MGL_NEAREST								0x2600
#define MGL_LINEAR								0x2601
#define MGL_NEAREST_MIPMAP_NEAREST				0x2700
#define MGL_LINEAR_MIPMAP_NEAREST				0x2701
#define MGL_NEAREST_MIPMAP_LINEAR				0x2702
#define MGL_LINEAR_MIPMAP_LINEAR				0x2703
#define MGL_TEXTURE_MAG_FILTER					0x2800
#define MGL_TEXTURE_MIN_FILTER					0x2801
#define MGL_TEXTURE_WRAP_S						0x2802
#define MGL_TEXTURE_WRAP_T						0x2803
#define MGL_CLAMP								0x2900
#define MGL_REPEAT								0x2901
#define MGL_FUNC_ADD							0x8006
#define MGL_POLYGON_OFFSET_FILL					0x8037
#define MGL_INTENSITY							0x8049
#define MGL_INTENSITY4							0x804A
#define MGL_INTENSITY8							0x804B
#define MGL_INTENSITY12							0x804C
#define MGL_INTENSITY16							0x804D
#define MGL_TEXTURE_PRIORITY					0x8066
#define MGL_TEXTURE_RESIDENT					0x8067
#define MGL_VERTEX_ARRAY						0x8074
#define MGL_NORMAL_ARRAY						0x8075
#define MGL_COLOR_ARRAY							0x8076
#define MGL_INDEX_ARRAY							0x8077
#define MGL_TEXTURE_COORD_ARRAY					0x8078
#define MGL_BGR									0x80E0
#define MGL_TEXTURE_3D							0x806F
#define MGL_CLAMP_TO_EDGE						0x812F
#define MGL_GENERATE_MIPMAP						0x8191
#define MGL_DEPTH_COMPONENT16					0x81A5
#define MGL_DEPTH_COMPONENT24					0x81A6
#define MGL_DEPTH_COMPONENT32					0x81A7
#define MGL_DEPTH_STENCIL_ATTACHMENT			0x821A
#define MGL_MAJOR_VERSION						0x821B
#define MGL_MINOR_VERSION						0x821C
#define MGL_R8									0x8229
#define MGL_PROGRAM_BINARY_RETRIEVABLE_HINT		0x8257
#define MGL_ACTIVE_PROGRAM						0x8259
#define MGL_TEXTURE								0x84C0
#define MGL_DEPTH_STENCIL						0x84F9
#define MGL_TEXTURE_MAX_ANISOTROPY			    0x84FE
#define MGL_MAX_TEXTURE_MAX_ANISOTROPY			0x84FF
#define MGL_TEXTURE_CUBE_MAP					0x8513
#define MGL_TEXTURE_CUBE_MAP_POSITIVE_X			0x8515
#define MGL_TEXTURE_CUBE_MAP_NEGATIVE_X			0x8516
#define MGL_TEXTURE_CUBE_MAP_POSITIVE_Y			0x8517
#define MGL_TEXTURE_CUBE_MAP_NEGATIVE_Y			0x8518
#define MGL_TEXTURE_CUBE_MAP_POSITIVE_Z			0x8519
#define MGL_TEXTURE_CUBE_MAP_NEGATIVE_Z			0x851A
#define MGL_PROGRAM_BINARY_LENGTH				0x8741
#define MGL_NUM_PROGRAM_BINARY_FORMATS			0x87FE
#define MGL_PROGRAM_BINARY_FORMATS				0x87FF
#define MGL_RGBA32F								0x8814
#define MGL_RGB32F								0x8815
#define MGL_RGBA16F								0x881A
#define MGL_RGB16F								0x881B
#define MGL_TEXTURE_CUBE_MAP_SEAMLESS			0x884F
#define MGL_TEXTURE_DEPTH_SIZE					0x884A
#define MGL_DEPTH_TEXTURE_MODE					0x884B
#define MGL_TEXTURE_COMPARE_MODE				0x884C
#define MGL_TEXTURE_COMPARE_FUNC				0x884D
#define MGL_COMPARE_R_TO_TEXTURE				0x884E
#define MGL_QUERY_COUNTER_BITS					0x8864
#define MGL_CURRENT_QUERY						0x8865
#define MGL_QUERY_RESULT						0x8866
#define MGL_QUERY_RESULT_AVAILABLE				0x8867
#define MGL_READ_ONLY							0x88B8
#define MGL_WRITE_ONLY							0x88B9
#define MGL_READ_WRITE							0x88BA
#define MGL_BUFFER_ACCESS						0x88BB
#define MGL_BUFFER_MAPPED						0x88BC
#define MGL_BUFFER_MAP_POINTER					0x88BD
#define MGL_STREAM_DRAW							0x88E0
#define MGL_STREAM_READ							0x88E1
#define MGL_STREAM_COPY							0x88E2
#define MGL_STATIC_DRAW							0x88E4
#define MGL_STATIC_READ							0x88E5
#define MGL_STATIC_COPY							0x88E6
#define MGL_DYNAMIC_DRAW						0x88E8
#define MGL_DYNAMIC_READ						0x88E9
#define MGL_DYNAMIC_COPY						0x88EA
#define MGL_ARRAY_BUFFER						0x8892
#define MGL_ELEMENT_ARRAY_BUFFER				0x8893
#define MGL_SAMPLES_PASSED						0x8914
#define MGL_FRAGMENT_SHADER						0x8B30
#define MGL_VERTEX_SHADER						0x8B31
#define MGL_COMPILE_STATUS						0x8B81
#define MGL_LINK_STATUS							0x8B82
#define MGL_FLOAT_VEC2							0x8B50
#define MGL_FLOAT_VEC3							0x8B51
#define MGL_FLOAT_VEC4							0x8B52
#define MGL_INT_VEC2							0x8B53
#define MGL_INT_VEC3							0x8B54
#define MGL_INT_VEC4							0x8B55
#define MGL_BOOL								0x8B56
#define MGL_BOOL_VEC2							0x8B57
#define MGL_BOOL_VEC3							0x8B58
#define MGL_BOOL_VEC4							0x8B59
#define MGL_FLOAT_MAT2							0x8B5A
#define MGL_FLOAT_MAT3							0x8B5B
#define MGL_FLOAT_MAT4							0x8B5C
#define MGL_SAMPLER_1D							0x8B5D
#define MGL_SAMPLER_2D							0x8B5E
#define MGL_SAMPLER_3D							0x8B5F
#define MGL_SAMPLER_CUBE						0x8B60
#define MGL_SAMPLER_1D_SHADOW					0x8B61
#define MGL_SAMPLER_2D_SHADOW					0x8B62
#define MGL_VALIDATE_STATUS						0x8B83
#define MGL_INFO_LOG_LENGTH						0x8B84
#define MGL_ATTACHED_SHADERS					0x8B85
#define MGL_ACTIVE_UNIFORMS						0x8B86
#define MGL_ACTIVE_UNIFORM_MAX_LENGTH			0x8B87
#define MGL_ACTIVE_ATTRIBUTES					0x8B89
#define MGL_ACTIVE_ATTRIBUTE_MAX_LENGTH			0x8B8A
#define MGL_SHADING_LANGUAGE_VERSION       		0x8B8C
#define MGL_TEXTURE_2D_ARRAY					0x8C1A
#define MGL_ANY_SAMPLES_PASSED					0x8C2F
#define MGL_READ_FRAMEBUFFER					0x8CA8
#define MGL_DRAW_FRAMEBUFFER					0x8CA9
#define MGL_FRAMEBUFFER_COMPLETE				0x8CD5
#define MGL_COLOR_ATTACHMENT					0x8CE0
#define MGL_DEPTH_ATTACHMENT					0x8D00
#define MGL_STENCIL_ATTACHMENT					0x8D20
#define MGL_FRAMEBUFFER							0x8D40
#define MGL_RENDERBUFFER						0x8D41
#define NGL_TEXTURE_MAX_ANISOTROPY_EXT			0x84FE
#define MGL_TEXTURE_2D_MULTISAMPLE				0x9100
#define MGL_DEPTH_BUFFER_BIT					0x00000100
#define MGL_STENCIL_BUFFER_BIT					0x00000400
#define MGL_COLOR_BUFFER_BIT					0x00004000

// ------------------------------------------------------------------------------------------------  
typedef MGLenum (MZAPIENTRY *mglGetErrorProc) (void);
typedef const MGLubyte* (MZAPIENTRY *mglGetStringProc) (MGLenum name);
typedef void (MZAPIENTRY *mglClearProc) (MGLbitfield);
typedef void (MZAPIENTRY *mglClearColorProc) (MGLfloat, MGLfloat, MGLfloat, MGLfloat);
typedef void (MZAPIENTRY *mglGetIntegervProc) (MGLenum, MGLint*);
typedef void (MZAPIENTRY *mglGetFloatvProc) (MGLenum, MGLfloat*);
typedef void (MZAPIENTRY *mglViewportProc) (MGLint, MGLint, MGLsizei, MGLsizei);
typedef void (MZAPIENTRY *mglDepthMaskProc) (MGLboolean);
typedef void (MZAPIENTRY *mglColorMaskProc) (MGLboolean, MGLboolean, MGLboolean, MGLboolean);
typedef void (MZAPIENTRY *mglStencilMaskProc) (MGLuint);
typedef void (MZAPIENTRY *mglEnableProc) (MGLenum);
typedef void (MZAPIENTRY *mglDisableProc) (MGLenum);
typedef void (MZAPIENTRY *mglFrontFaceProc) (MGLenum);
typedef void (MZAPIENTRY *mglCullFaceProc) (MGLenum);
typedef void (MZAPIENTRY *mglFinishProc) (void);
typedef void (MZAPIENTRY *mglFlushProc) (void);
typedef void (MZAPIENTRY *mglBlendFuncProc) (MGLenum, MGLenum);
typedef void (MZAPIENTRY *mglStencilFuncProc) (MGLenum, MGLint, MGLuint);
typedef void (MZAPIENTRY *mglStencilOpProc) (MGLenum, MGLenum, MGLenum);
typedef void (MZAPIENTRY *mglClearStencilProc) (MGLint);
typedef void (MZAPIENTRY *mglPolygonOffsetProc) (MGLfloat, MGLfloat);
typedef MGLuint (MZAPIENTRY *mglCreateProgramProc) ();
typedef MGLuint (MZAPIENTRY *mglCreateShaderProc) (MGLenum);
typedef MGLint (MZAPIENTRY *mglGetUniformLocationProc) (MGLuint, const MGLchar*);
typedef MGLint (MZAPIENTRY *mglGetAttribLocationProc) (MGLuint, const MGLchar*);
typedef void (MZAPIENTRY *mglUseProgramProc) (MGLuint);;
typedef void (MZAPIENTRY *mglShaderSourceProc) (MGLuint, MGLsizei, const MGLchar**, const MGLint*);
typedef void (MZAPIENTRY *mglCompileShaderProc) (MGLuint);
typedef void (MZAPIENTRY *mglGetShaderivProc) (MGLuint, MGLenum, MGLint*);
typedef void (MZAPIENTRY *mglGetProgramivProc) (MGLuint, MGLenum, MGLint*);
typedef void (MZAPIENTRY *mglGetShaderInfoLogProc) (MGLuint, MGLsizei, MGLsizei*, MGLchar*);
typedef void (MZAPIENTRY *mglGetProgramInfoLogProc) (MGLuint, MGLsizei, MGLsizei*, MGLchar*);
typedef void (MZAPIENTRY *mglAttachShaderProc) (MGLuint, MGLuint);
typedef void (MZAPIENTRY *mglDetachShaderProc) (MGLuint, MGLuint);
typedef void (MZAPIENTRY *mglLinkProgramProc) (MGLuint);
typedef void (MZAPIENTRY *mglValidateProgramProc) (MGLuint);
typedef void (MZAPIENTRY *mglGetAttachedShadersProc) (MGLuint, MGLsizei, MGLsizei*, MGLuint*);
typedef void (MZAPIENTRY *mglDeleteShaderProc) (MGLuint);
typedef void (MZAPIENTRY *mglDeleteProgramProc) (MGLuint);
typedef void (MZAPIENTRY *mglUniformMatrix2fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (MZAPIENTRY *mglUniformMatrix3fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (MZAPIENTRY *mglUniformMatrix4fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (MZAPIENTRY *mglUniform1iProc) (MGLint, MGLint);
typedef void (MZAPIENTRY *mglUniform1fProc) (MGLint, MGLfloat);
typedef void (MZAPIENTRY *mglUniform1fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (MZAPIENTRY *mglUniform3fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (MZAPIENTRY *mglUniform4fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (MZAPIENTRY *mglGetActiveUniformProc) (MGLuint, MGLuint, MGLsizei, MGLsizei*, MGLint*, MGLenum*, MGLchar*);
typedef void (MZAPIENTRY *mglGetActiveAttribProc) (MGLuint, MGLuint, MGLsizei, MGLsizei*, MGLint*, MGLenum*, MGLchar*);
typedef void (MZAPIENTRY *mglGenTexturesProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglBindTextureProc) (MGLenum, MGLuint);
typedef void (MZAPIENTRY *mglTexParameterfProc) (MGLenum, MGLenum, MGLfloat);
typedef void (MZAPIENTRY *mglTexParameteriProc) (MGLenum, MGLenum, MGLint);
typedef void (MZAPIENTRY *mglTexSubImage2DProc) (MGLenum, MGLint, MGLint, MGLint, MGLsizei, MGLsizei, MGLenum, MGLenum, const MGLvoid*);
typedef void (MZAPIENTRY *mglTexImage2DProc) (MGLenum, MGLint, MGLint, MGLsizei, MGLsizei, MGLint, MGLenum, MGLenum, const MGLvoid*);
typedef void (MZAPIENTRY *mglTexImage3DProc) (MGLenum, MGLint, MGLint, MGLsizei, MGLsizei, MGLsizei, MGLint, MGLenum, MGLenum, const MGLvoid*);
typedef void (MZAPIENTRY *mglGenerateMipmapProc) (MGLenum);
typedef void (MZAPIENTRY *mglActiveTextureProc) (MGLenum);
typedef void (MZAPIENTRY *mglClearDepthProc) (MGLdouble);
typedef void (MZAPIENTRY *mglDepthFuncProc) (MGLenum);
typedef void (MZAPIENTRY *mglVertexPointerProc) (MGLint, MGLenum, MGLsizei, const MGLvoid *);
typedef void (MZAPIENTRY *mglNormalPointerProc) (MGLenum, MGLsizei, const MGLvoid *);
typedef void (MZAPIENTRY *mglTexCoordPointerProc) (MGLint, MGLenum, MGLsizei, const MGLvoid *);
typedef void (MZAPIENTRY *mglEnableClientStateProc) (MGLenum);
typedef void (MZAPIENTRY *mglDisableClientStateProc) (MGLenum);
typedef void (MZAPIENTRY *mglDrawArraysProc) (MGLenum, MGLint, MGLsizei);
typedef void (MZAPIENTRY *mglDrawElementsProc) (MGLenum, MGLsizei, MGLenum, const MGLvoid *);
typedef void (MZAPIENTRY *mglDeleteTexturesProc) (MGLsizei, const MGLuint*);
typedef void (MZAPIENTRY *mglGenBuffersProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglDeleteBuffersProc) (MGLsizei, const MGLuint*);
typedef void (MZAPIENTRY *mglBindBufferProc) (MGLenum, MGLuint);
typedef void (MZAPIENTRY *mglBufferSubDataProc) (MGLenum, MGLintptr, MGLsizeiptr size, const MGLvoid *);
typedef void (MZAPIENTRY *mglBufferDataProc) (MGLenum, MGLsizeiptr, const MGLvoid *, MGLenum);
typedef void* (MZAPIENTRY *mglMapBufferProc) (MGLenum, MGLenum);
typedef MGLboolean (MZAPIENTRY *mglUnmapBufferProc) (MGLenum);
typedef void (MZAPIENTRY *mglEnableVertexAttribArrayProc) (MGLuint);
typedef void (MZAPIENTRY *mglDisableVertexAttribArrayProc) (MGLuint);
typedef void (MZAPIENTRY *mglVertexAttribPointerProc) (MGLuint, MGLint, MGLenum, MGLboolean, MGLsizei, const MGLvoid *);
typedef void (MZAPIENTRY *mglVertexAttribDivisorProc) (MGLuint, MGLuint);
typedef void (MZAPIENTRY *mglGenVertexArraysProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglDeleteVertexArraysProc) (MGLsizei, const MGLuint*);
typedef void (MZAPIENTRY *mglBindVertexArrayProc) (MGLuint);
typedef void (MZAPIENTRY *mglDrawArraysInstancedProc) (MGLenum, MGLint, MGLsizei, MGLsizei);
typedef void (MZAPIENTRY *mglDrawElementsInstancedProc) (MGLenum, MGLsizei, MGLenum, const void *, MGLsizei);
typedef void (MZAPIENTRY *mglClientActiveTextureProc) (MGLenum);
typedef MGLboolean (MZAPIENTRY *mglIsRenderbufferProc) (MGLuint);
typedef MGLboolean (MZAPIENTRY *mglIsFramebufferProc) (MGLuint);
typedef MGLenum (MZAPIENTRY *mglCheckFramebufferStatusProc) (MGLenum);
typedef void (MZAPIENTRY *mglFramebufferTextureLayerProc) (MGLenum, MGLenum, MGLuint, MGLint, MGLint);
typedef void (MZAPIENTRY *mglGenFramebuffersProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglDeleteFramebuffersProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglBindFramebufferProc) (MGLenum, MGLuint);
typedef void (MZAPIENTRY *mglFramebufferTexture2DProc) (MGLenum, MGLenum, MGLenum, MGLuint, MGLint);
typedef void (MZAPIENTRY *mglFramebufferRenderbufferProc) (MGLenum, MGLenum, MGLenum, MGLuint);
typedef void (MZAPIENTRY *mglGenRenderbuffersProc) (MGLsizei, MGLuint*);
typedef void (MZAPIENTRY *mglDeleteRenderbuffersProc) (MGLsizei, const MGLuint *);
typedef void (MZAPIENTRY *mglBindRenderbufferProc) (MGLenum, MGLuint);
typedef void (MZAPIENTRY *mglRenderbufferStorageProc) (MGLenum, MGLenum, MGLsizei, MGLsizei);
typedef void (MZAPIENTRY *mglDrawBuffersProc) (MGLsizei, const MGLenum*);
typedef void (MZAPIENTRY *mglDrawBufferProc) (MGLenum);
typedef void (MZAPIENTRY *mglReadBufferProc) (MGLenum);
typedef void (MZAPIENTRY *mglBlitFramebufferProc) (MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLbitfield, MGLenum);
typedef void (MZAPIENTRY *mglGetProgramBinaryProc) (MGLuint, MGLsizei, MGLsizei*, MGLenum*, void*);
typedef void (MZAPIENTRY *mglProgramBinaryProc) (MGLuint, MGLenum, const void*, MGLsizei);
typedef void (MZAPIENTRY *mglProgramParameteriProc) (MGLuint, MGLenum, MGLint);
typedef BOOL (MZAPIENTRY *mwglSwapIntervalProc) (int interval);
typedef int  (MZAPIENTRY *mwglGetSwapIntervalProc) (void);

// ------------------------------------------------------------------------------------------------ 
extern mglGetErrorProc							mglGetError;
extern mglClearProc								mglClear;
extern mglClearColorProc						mglClearColor;
extern mglGetIntegervProc						mglGetIntegerv;
extern mglGetFloatvProc							mglGetFloatv;
extern mglViewportProc							mglViewport;
extern mglDepthMaskProc							mglDepthMask;
extern mglColorMaskProc							mglColorMask;
extern mglEnableProc							mglEnable;
extern mglDisableProc							mglDisable;
extern mglFrontFaceProc							mglFrontFace;
extern mglFlushProc								mglFlush;
extern mglFinishProc							mglFinish;
extern mglGetStringProc							mglGetString;
extern mglStencilFuncProc						mglStencilFunc;
extern mglStencilOpProc							mglStencilOp;
extern mglStencilMaskProc						mglStencilMask;
extern mglBlendFuncProc							mglBlendFunc;
extern mglCullFaceProc							mglCullFace;
extern mglClearStencilProc						mglClearStencil;
extern mglPolygonOffsetProc						mglPolygonOffset;
extern mglUseProgramProc						mglUseProgram;
extern mglCreateShaderProc						mglCreateShader;
extern mglCreateProgramProc						mglCreateProgram;
extern mglShaderSourceProc						mglShaderSource;
extern mglCompileShaderProc						mglCompileShader;
extern mglGetShaderivProc						mglGetShaderiv;
extern mglGetProgramivProc						mglGetProgramiv;
extern mglGetShaderInfoLogProc					mglGetShaderInfoLog;
extern mglGetProgramInfoLogProc					mglGetProgramInfoLog;
extern mglAttachShaderProc						mglAttachShader;
extern mglDetachShaderProc						mglDetachShader;
extern mglLinkProgramProc						mglLinkProgram;
extern mglValidateProgramProc					mglValidateProgram;
extern mglGetAttachedShadersProc				mglGetAttachedShaders;
extern mglDeleteShaderProc						mglDeleteShader;
extern mglDeleteProgramProc						mglDeleteProgram;
extern mglGetUniformLocationProc				mglGetUniformLocation;
extern mglGetAttribLocationProc					mglGetAttribLocation;
extern mglGetActiveUniformProc					mglGetActiveUniform;
extern mglUniform1iProc							mglUniform1i;
extern mglUniform1fProc							mglUniform1f;
extern mglUniform1fvProc						mglUniform1fv;
extern mglUniform3fvProc						mglUniform3fv;
extern mglUniform4fvProc						mglUniform4fv;
extern mglUniformMatrix3fvProc					mglUniformMatrix2fv;
extern mglUniformMatrix3fvProc					mglUniformMatrix3fv;
extern mglUniformMatrix4fvProc					mglUniformMatrix4fv;
extern mglGetActiveAttribProc					mglGetActiveAttrib;
extern mglGenTexturesProc						mglGenTextures;
extern mglBindTextureProc						mglBindTexture;
extern mglTexParameterfProc						mglTexParameterf; 
extern mglTexParameteriProc						mglTexParameteri;
extern mglTexSubImage2DProc						mglTexSubImage2D;
extern mglTexImage2DProc						mglTexImage2D;
extern mglTexImage3DProc						mglTexImage3D;
extern mglActiveTextureProc						mglActiveTexture;
extern mglClearDepthProc						mglClearDepth;
extern mglDepthFuncProc							mglDepthFunc;
extern mglGenerateMipmapProc					mglGenerateMipmap;
extern mglVertexPointerProc						mglVertexPointer;
extern mglNormalPointerProc						mglNormalPointer;
extern mglEnableClientStateProc					mglEnableClientState;
extern mglDisableClientStateProc				mglDisableClientState;
extern mglDrawArraysProc						mglDrawArrays;
extern mglDrawElementsProc						mglDrawElements;
extern mglTexCoordPointerProc					mglTexCoordPointer;
extern mglDeleteTexturesProc					mglDeleteTextures;
extern mglGenBuffersProc						mglGenBuffers;
extern mglDeleteBuffersProc						mglDeleteBuffers;
extern mglBindBufferProc						mglBindBuffer;
extern mglBufferSubDataProc						mglBufferSubData;
extern mglBufferDataProc						mglBufferData;
extern mglMapBufferProc							mglMapBuffer;
extern mglUnmapBufferProc						mglUnmapBuffer;
extern mglEnableVertexAttribArrayProc			mglEnableVertexAttribArray;
extern mglDisableVertexAttribArrayProc			mglDisableVertexAttribArray;
extern mglVertexAttribPointerProc				mglVertexAttribPointer;
extern mglVertexAttribDivisorProc				mglVertexAttribDivisor;
extern mglGenVertexArraysProc					mglGenVertexArrays;
extern mglDeleteVertexArraysProc				mglDeleteVertexArrays;
extern mglBindVertexArrayProc					mglBindVertexArray;
extern mglDrawArraysInstancedProc				mglDrawArraysInstanced;
extern mglDrawElementsInstancedProc				mglDrawElementsInstanced;
extern mglClientActiveTextureProc				mglClientActiveTexture;
extern mglIsRenderbufferProc					mglIsRenderbuffer;
extern mglIsFramebufferProc						mglIsFramebuffer;
extern mglCheckFramebufferStatusProc			mglCheckFramebufferStatus;
extern mglFramebufferTextureLayerProc			mglFramebufferTextureLayer;
extern mglGenFramebuffersProc					mglGenFramebuffers;
extern mglDeleteFramebuffersProc				mglDeleteFramebuffers;
extern mglBindFramebufferProc					mglBindFramebuffer;
extern mglFramebufferTexture2DProc				mglFramebufferTexture2D;
extern mglFramebufferRenderbufferProc			mglFramebufferRenderbuffer;
extern mglGenRenderbuffersProc					mglGenRenderbuffers;
extern mglDeleteRenderbuffersProc				mglDeleteRenderbuffers;
extern mglBindRenderbufferProc					mglBindRenderbuffer;
extern mglRenderbufferStorageProc				mglRenderbufferStorage;
extern mglDrawBuffersProc						mglDrawBuffers;
extern mglDrawBufferProc						mglDrawBuffer;
extern mglReadBufferProc						mglReadBuffer;
extern mglBlitFramebufferProc					mglBlitFramebuffer;
extern mglGetProgramBinaryProc					mglGetProgramBinary;
extern mglProgramBinaryProc						mglProgramBinary;
extern mglProgramParameteriProc					mglProgramParameteri;
extern mwglSwapIntervalProc						mwglSwapInterval;
extern mwglGetSwapIntervalProc					mwglGetSwapInterval;

/**
	@brief 
		Initializes OpenGL extensions

	@note 
		This function must be called from a 
		thread with an OpenGL 3.3 context attached
*/
int mglInit();

/**
	@brief
		Check if an OpenGL extension is supported

	@note
		Must be called after mglInit

	@param ext Name of the extension
*/
int mglIsSupported(const char* ext);

/**
	@brief
		Return the maximum anisotropy level 
		supported by OpenGL

	@note
		Must be called after mglInit

	@return Anisotropy from 0.0f to 16.0f
*/
float mglGetMaxAnisotropy();

#endif /*MGL_H*/
