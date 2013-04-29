// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 110

uniform sampler2D uDiffuseMap;
uniform vec3 uDiffuse;

uniform mat4 uView;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vTexCoord;

void main()
{
    vec3 diffuse = texture2D(uDiffuseMap, vTexCoord).rgb;
    
    gl_FragData[0] = vec4(diffuse, 0.5);
    gl_FragData[1] = vec4((normalize(vNormal) + vec3(1.0)) / 2.0, 0.0);
    gl_FragData[2] = vec4(vVertex, -(uView * vec4(vVertex, 1.0)).z);
    gl_FragData[3] = vec4(0.0, 0.0, 0.0, 0.0);
}
