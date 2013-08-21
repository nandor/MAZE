// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_draw_instanced : enable

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uNormal;
uniform mat2 uTexture;

attribute vec4 aModel0;
attribute vec4 aModel1;
attribute vec4 aModel2;
attribute vec4 aModel3;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vUV;

void main()
{
    mat4 aModel = mat4(aModel0, aModel1, aModel2, aModel3);
    
    vVertex = vec3(aModel * gl_Vertex);
    vNormal = mat3(aModel) * gl_Normal;
    vUV = uTexture * gl_MultiTexCoord0.st;

    gl_Position = uProj * uView * vec4(vVertex, 1.0);
}
