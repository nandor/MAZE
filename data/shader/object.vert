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

mat3 matrixInverse(in mat3 m)
{  
   float det = dot(cross(m[0], m[1]), m[2]);
   mat3 t = transpose(m);
   return mat3(cross(t[1], t[2]), cross(t[2], t[0]), cross(t[0], t[1])) / det;
}

void main()
{
    mat4 aModel = mat4(aModel0, aModel1, aModel2, aModel3);
    
    vVertex = vec3(aModel * gl_Vertex);
    vNormal = vec3(matrixInverse(mat3(aModel)) * gl_Normal);
    vUV = uTexture * gl_MultiTexCoord0.st;

    gl_Position = uProj * uView * vec4(vVertex, 1.0);
}
