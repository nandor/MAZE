// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform mat4 uMVP;
varying vec3 vVertex;

void main()
{
    vVertex = gl_Vertex.xyz;
    gl_Position = uMVP * vec4(vVertex, 1.0);
}