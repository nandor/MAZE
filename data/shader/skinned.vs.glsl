// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_draw_instanced : enable

attribute vec4 aBone;
attribute vec4 aWeight;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uNormal;
uniform mat2 uTexture;
uniform mat4 uSkin[36];

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vUV;

void main()
{
    vec3 vertex = vec3(0.0), normal = vec3(0.0);
    mat3 matrix;
    
    // Apply bone transforms
    matrix = mat3(
        uSkin[int(aBone.x)] * aWeight.x +
        uSkin[int(aBone.y)] * aWeight.y +
        uSkin[int(aBone.z)] * aWeight.z +
        uSkin[int(aBone.w)] * aWeight.w
    );
             
    // Send data to fragment shader
    vVertex = vec3(uModel * vec4(matrix * gl_Vertex.xyz, 1.0));
    vNormal = vec3(uModel * vec4(matrix * gl_Normal, 1.0));
    vUV = uTexture * gl_MultiTexCoord0.st;
    gl_Position = uProj * uView * vec4(vVertex, 1.0);
}
