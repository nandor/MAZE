// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

varying vec2 vUV;

void main()
{
    gl_Position = vec4(gl_Vertex.xy, 1.0, 1.0);
    vUV = gl_Vertex.xy / 2.0 + vec2(0.5);
}