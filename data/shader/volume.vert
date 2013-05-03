// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform mat4 uMVP;

void main()
{    
    gl_Position = uMVP * gl_Vertex;
}
