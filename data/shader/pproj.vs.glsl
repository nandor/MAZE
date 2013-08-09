// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_draw_instanced : enable

uniform mat4 uVP;

attribute vec4 aModel0;
attribute vec4 aModel1;
attribute vec4 aModel2;
attribute vec4 aModel3;

void main()
{        
    gl_Position = uVP * mat4(aModel0, aModel1, aModel2, aModel3) * gl_Vertex;
}
