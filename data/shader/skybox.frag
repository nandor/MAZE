// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform samplerCube uTexture;
varying vec3 vVertex;

void main()
{
    gl_FragColor = vec4(textureCube(uTexture, vVertex).rgb, 1.0);
}