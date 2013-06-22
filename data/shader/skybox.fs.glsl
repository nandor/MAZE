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
    gl_FragData[0].rgb = normalize(vVertex) * uFarPlane + uPosition;
    gl_FragData[1].rgb = textureCube(uTexture, vVertex).rgb;
}