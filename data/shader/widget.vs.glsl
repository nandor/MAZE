// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

uniform mat4 uProj;

varying vec2 vTexCoord;

void main()
{
    vTexCoord = gl_MultiTexCoord0.st;
    gl_Position = uProj * gl_Vertex;
}
