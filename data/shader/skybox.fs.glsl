// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform samplerCube uTexture;
uniform vec3 uPosition;
uniform float uFarPlane;
varying vec3 vVertex;

void main()
{
    // Position data is relevant for fog generation
    gl_FragData[0] = vec4(uPosition + normalize(vVertex) * uFarPlane, 1.0);
    gl_FragData[1] = vec4(textureCube(uTexture, vVertex).rgb, 1.0);
}