// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MGL_H
#define MGL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "MZPlatform.h"
	
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
typedef ptrdiff_t		MGLsizeiptr;
	
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
#define MGL_NEVER								0x0200
#define MGL_LESS								0x0201
#define MGL_EQUAL								0x0202
#define MGL_LEQUAL								0x0203
#define MGL_GREATER								0x0204
#define MGL_NOTEQUAL							0x0205
#define MGL_GEQUAL								0x0206
#define MGL_ALWAYS								0x0207
#define MGL_SRC_ALPHA							0x0302
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
typedef MGLenum (APIENTRY *mglGetErrorProc) (void);
typedef const MGLubyte* (APIENTRY *mglGetStringProc) (MGLenum name);
typedef void (APIENTRY *mglClearProc) (MGLbitfield);
typedef void (APIENTRY *mglClearColorProc) (MGLfloat, MGLfloat, MGLfloat, MGLfloat);
typedef void (APIENTRY *mglGetIntegervProc) (MGLenum, MGLint*);
typedef void (APIENTRY *mglGetFloatvProc) (MGLenum, MGLfloat*);
typedef void (APIENTRY *mglViewportProc) (MGLint, MGLint, MGLsizei, MGLsizei);
typedef void (APIENTRY *mglDepthMaskProc) (MGLboolean);
typedef void (APIENTRY *mglColorMaskProc) (MGLboolean, MGLboolean, MGLboolean, MGLboolean);
typedef void (APIENTRY *mglStencilMaskProc) (MGLuint);
typedef void (APIENTRY *mglEnableProc) (MGLenum);
typedef void (APIENTRY *mglDisableProc) (MGLenum);
typedef void (APIENTRY *mglFrontFaceProc) (MGLenum);
typedef void (APIENTRY *mglCullFaceProc) (MGLenum);
typedef void (APIENTRY *mglFinishProc) (void);
typedef void (APIENTRY *mglFlushProc) (void);
typedef void (APIENTRY *mglBlendFuncProc) (MGLenum, MGLenum);
typedef void (APIENTRY *mglStencilFuncProc) (MGLenum, MGLint, MGLuint);
typedef void (APIENTRY *mglStencilOpProc) (MGLenum, MGLenum, MGLenum);
typedef void (APIENTRY *mglClearStencilProc) (MGLint);
typedef void (APIENTRY *mglPolygonOffsetProc) (MGLfloat, MGLfloat);
typedef MGLuint (APIENTRY *mglCreateProgramProc) ();
typedef MGLuint (APIENTRY *mglCreateShaderProc) (MGLenum);
typedef MGLint (APIENTRY *mglGetUniformLocationProc) (MGLuint, const MGLchar*);
typedef MGLint (APIENTRY *mglGetAttribLocationProc) (MGLuint, const MGLchar*);
typedef void (APIENTRY *mglUseProgramProc) (MGLuint);;
typedef void (APIENTRY *mglShaderSourceProc) (MGLuint, MGLsizei, const MGLchar**, const MGLint*);
typedef void (APIENTRY *mglCompileShaderProc) (MGLuint);
typedef void (APIENTRY *mglGetShaderivProc) (MGLuint, MGLenum, MGLint*);
typedef void (APIENTRY *mglGetProgramivProc) (MGLuint, MGLenum, MGLint*);
typedef void (APIENTRY *mglGetShaderInfoLogProc) (MGLuint, MGLsizei, MGLsizei*, MGLchar*);
typedef void (APIENTRY *mglGetProgramInfoLogProc) (MGLuint, MGLsizei, MGLsizei*, MGLchar*);
typedef void (APIENTRY *mglAttachShaderProc) (MGLuint, MGLuint);
typedef void (APIENTRY *mglDetachShaderProc) (MGLuint, MGLuint);
typedef void (APIENTRY *mglLinkProgramProc) (MGLuint);
typedef void (APIENTRY *mglValidateProgramProc) (MGLuint);
typedef void (APIENTRY *mglGetAttachedShadersProc) (MGLuint, MGLsizei, MGLsizei*, MGLuint*);
typedef void (APIENTRY *mglDeleteShaderProc) (MGLuint);
typedef void (APIENTRY *mglDeleteProgramProc) (MGLuint);
typedef void (APIENTRY *mglUniformMatrix2fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (APIENTRY *mglUniformMatrix3fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (APIENTRY *mglUniformMatrix4fvProc) (MGLint, MGLsizei, MGLboolean, const MGLfloat*);
typedef void (APIENTRY *mglUniform1iProc) (MGLint, MGLint);
typedef void (APIENTRY *mglUniform1fProc) (MGLint, MGLfloat);
typedef void (APIENTRY *mglUniform1fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (APIENTRY *mglUniform3fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (APIENTRY *mglUniform4fvProc) (MGLint, MGLsizei, const MGLfloat *);
typedef void (APIENTRY *mglGetActiveUniformProc) (MGLuint, MGLuint, MGLsizei, MGLsizei*, MGLint*, MGLenum*, MGLchar*);
typedef void (APIENTRY *mglGetActiveAttribProc) (MGLuint, MGLuint, MGLsizei, MGLsizei*, MGLint*, MGLenum*, MGLchar*);
typedef void (APIENTRY *mglGenTexturesProc) (MGLsizei, MGLuint*);
typedef void (APIENTRY *mglBindTextureProc) (MGLenum, MGLuint);
typedef void (APIENTRY *mglTexParameterfProc) (MGLenum, MGLenum, MGLfloat);
typedef void (APIENTRY *mglTexParameteriProc) (MGLenum, MGLenum, MGLint);
typedef void (APIENTRY *mglTexImage2DProc) (MGLenum, MGLint, MGLint, MGLsizei, MGLsizei, MGLint, MGLenum, MGLenum, const MGLvoid*);
typedef void (APIENTRY *mglTexImage3DProc) (MGLenum, MGLint, MGLint, MGLsizei, MGLsizei, MGLsizei, MGLint, MGLenum, MGLenum, const MGLvoid*);
typedef void (APIENTRY *mglGenerateMipmapProc) (MGLenum);
typedef void (APIENTRY *mglActiveTextureProc) (MGLenum);
typedef void (APIENTRY *mglClearDepthProc) (MGLdouble);
typedef void (APIENTRY *mglDepthFuncProc) (MGLenum);
typedef void (APIENTRY *mglVertexPointerProc) (MGLint, MGLenum, MGLsizei, const MGLvoid *);
typedef void (APIENTRY *mglNormalPointerProc) (MGLenum, MGLsizei, const MGLvoid *);
typedef void (APIENTRY *mglTexCoordPointerProc) (MGLint, MGLenum, MGLsizei, const MGLvoid *);
typedef void (APIENTRY *mglEnableClientStateProc) (MGLenum);
typedef void (APIENTRY *mglDisableClientStateProc) (MGLenum);
typedef void (APIENTRY *mglDrawArraysProc) (MGLenum, MGLint, MGLsizei);
typedef void (APIENTRY *mglDeleteTexturesProc) (MGLsizei, const MGLuint*);
typedef void (APIENTRY *mglGenBuffersProc) (MGLsizei, MGLuint*);
typedef void (APIENTRY *mglDeleteBuffersProc) (MGLsizei, const MGLuint*);
typedef void (APIENTRY *mglBindBufferProc) (MGLenum, MGLuint);
typedef void (APIENTRY *mglBufferSubDataProc) (MGLenum, MGLintptr, MGLsizeiptr size, const MGLvoid *);
typedef void (APIENTRY *mglBufferDataProc) (MGLenum, MGLsizeiptr, const MGLvoid *, MGLenum);
typedef void* (APIENTRY *mglMapBufferProc) (MGLenum, MGLenum);
typedef MGLboolean (APIENTRY *mglUnmapBufferProc) (MGLenum);
typedef void (APIENTRY *mglEnableVertexAttribArrayProc) (MGLuint);
typedef void (APIENTRY *mglDisableVertexAttribArrayProc) (MGLuint);
typedef void (APIENTRY *mglVertexAttribPointerProc) (MGLuint, MGLint, MGLenum, MGLboolean, MGLsizei, const MGLvoid *);
typedef void (APIENTRY *mglVertexAttribDivisorProc) (MGLuint, MGLuint);
typedef void (APIENTRY *mglDrawArraysInstancedProc) (MGLenum, MGLint, MGLsizei, MGLsizei);
typedef void (APIENTRY *mglClientActiveTextureProc) (MGLenum);
typedef void (APIENTRY *mglBeginConditionalRenderProc) (MGLuint, MGLenum);
typedef void (APIENTRY *mglEndConditionalRenderProc) ();
typedef void (APIENTRY *mglBeginQueryProc) (MGLenum, MGLuint);
typedef void (APIENTRY *mglEndQueryProc) (MGLenum);
typedef void (APIENTRY *mglGetQueryObjectuivProc) (MGLenum, MGLenum, MGLuint *);
typedef void (APIENTRY *mglDeleteQueriesProc) (MGLsizei, const MGLuint*);
typedef void (APIENTRY *mglGenQueriesProc) (MGLsizei, MGLuint*);
typedef MGLboolean (APIENTRY *mglIsRenderbufferProc) (MGLuint);
typedef MGLboolean (APIENTRY *mglIsFramebufferProc) (MGLuint);
typedef MGLenum (APIENTRY *mglCheckFramebufferStatusProc) (MGLenum);
typedef void (APIENTRY *mglFramebufferTextureLayerProc) (MGLenum, MGLenum, MGLuint, MGLint, MGLint);
typedef void (APIENTRY *mglGenFramebuffersProc) (MGLsizei, MGLuint*);
typedef void (APIENTRY *mglDeleteFramebuffersProc) (MGLsizei, MGLuint*);
typedef void (APIENTRY *mglBindFramebufferProc) (MGLenum, MGLuint);
typedef void (APIENTRY *mglFramebufferTexture2DProc) (MGLenum, MGLenum, MGLenum, MGLuint, MGLint);
typedef void (APIENTRY *mglFramebufferRenderbufferProc) (MGLenum, MGLenum, MGLenum, MGLuint);
typedef void (APIENTRY *mglGenRenderbuffersProc) (MGLsizei, MGLuint*);
typedef void (APIENTRY *mglDeleteRenderbuffersProc) (MGLsizei, const MGLuint *);
typedef void (APIENTRY *mglBindRenderbufferProc) (MGLenum, MGLuint);
typedef void (APIENTRY *mglRenderbufferStorageProc) (MGLenum, MGLenum, MGLsizei, MGLsizei);
typedef void (APIENTRY *mglDrawBuffersProc) (MGLsizei, const MGLenum*);
typedef void (APIENTRY *mglDrawBufferProc) (MGLenum);
typedef void (APIENTRY *mglReadBufferProc) (MGLenum);
typedef void (APIENTRY *mglBlitFramebufferProc) (MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLint, MGLbitfield, MGLenum);

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
extern mglDrawArraysInstancedProc				mglDrawArraysInstanced;
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
extern mglBeginConditionalRenderProc			mglBeginConditionalRender;
extern mglEndConditionalRenderProc				mglEndConditionalRender;
extern mglBeginQueryProc						mglBeginQuery;
extern mglEndQueryProc							mglEndQuery;
extern mglGetQueryObjectuivProc					mglGetQueryObjectuiv;
extern mglDeleteQueriesProc						mglDeleteQueries;
extern mglGenQueriesProc						mglGenQueries;
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

#ifdef __cplusplus
}
#endif

#endif /*MGL_H*/
