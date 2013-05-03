// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uNormal;
uniform mat2 uTexture;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vUV;

void main()
{
    vVertex = vec3(uModel * gl_Vertex);
    vNormal = vec3(uNormal * vec4(gl_Normal, 1.0));
    vUV = uTexture * gl_MultiTexCoord0.st;

    gl_Position = uProj * uView * vec4(vVertex, 1.0);
}