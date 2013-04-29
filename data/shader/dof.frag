// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_texture_array : enable

const float BIAS = 0.6;
const float CLAMP = 2.0;
const vec3 KERNEL[9] = vec3[](
    vec3(-1.0, -1.0, 1),  vec3(0.0, -1.0, 2), vec3( 1.0, -1.0, 1),
    vec3(-1.0,  0.0, 2),  vec3(0.0,  0.0, 4), vec3( 1.0,  0.0, 2),
    vec3(-1.0,  1.0, 1),  vec3(0.0,  1.0, 2), vec3( 1.0,  1.0, 1)
);

uniform sampler2D uPosition;
uniform sampler2D uColor;

uniform int uWidth;
uniform int uHeight;

uniform float uFocus = 10.0;

varying vec2 vTexCoord;

void main()
{
    float focus = texture2D(uPosition, vec2(0.5)).w;
    float depth = texture2D(uPosition, vTexCoord).w;
    
    vec2 off = vec2(1.0 / uWidth, 1.0 / uHeight) * clamp((focus - depth) * BIAS, -CLAMP, CLAMP);
    vec4 color = vec4(0.0);
    
    for (int i = 0; i < 9; ++i)
    {
        color += texture2D(uColor, vTexCoord + KERNEL[i].xy * off) * KERNEL[i].z;
    }
    
    gl_FragColor = color / 16.0;
}
