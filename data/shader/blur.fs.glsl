// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

const float BIAS = 0.6;
const float CLAMP = 1.5;

uniform sampler2D uPosition;
uniform sampler2D uColor;

uniform int uWidth;
uniform int uHeight;
uniform float uFocus = 10.0;

varying vec2 vTexCoord;

void main()
{
    float depth = texture2D(uPosition, vTexCoord).w;
    float focus = min(texture2D(uPosition, vec2(0.5, 0.5)).w, uFocus);
    
    vec2 off = vec2(1.0 / uWidth, 1.0 / uHeight) * clamp((focus - depth) * BIAS, -CLAMP, CLAMP);
    
    vec4 color = vec4(0.0);    
    
    color +=  1.0 * texture2D(uColor, vTexCoord + vec2(-1.5, -1.5) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2(-1.5, -0.7) * off);
    color +=  7.0 * texture2D(uColor, vTexCoord + vec2(-1.5,  0.0) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2(-1.5,  0.7) * off);
    color +=  1.0 * texture2D(uColor, vTexCoord + vec2(-1.5,  1.5) * off);    
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2(-0.7, -1.5) * off);
    color += 16.0 * texture2D(uColor, vTexCoord + vec2(-0.7, -0.7) * off);
    color += 26.0 * texture2D(uColor, vTexCoord + vec2(-0.7,  0.0) * off);
    color += 16.0 * texture2D(uColor, vTexCoord + vec2(-0.7,  0.7) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2(-0.7,  1.5) * off);    
    color +=  7.0 * texture2D(uColor, vTexCoord + vec2( 0.0, -1.5) * off);
    color += 26.0 * texture2D(uColor, vTexCoord + vec2( 0.0, -0.7) * off);
    color += 41.0 * texture2D(uColor, vTexCoord + vec2( 0.0,  0.0) * off);
    color += 26.0 * texture2D(uColor, vTexCoord + vec2( 0.0,  0.7) * off);
    color +=  7.0 * texture2D(uColor, vTexCoord + vec2( 0.0,  1.5) * off);    
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2( 0.7, -1.5) * off);
    color += 16.0 * texture2D(uColor, vTexCoord + vec2( 0.7, -0.7) * off);
    color += 26.0 * texture2D(uColor, vTexCoord + vec2( 0.7,  0.0) * off);
    color += 16.0 * texture2D(uColor, vTexCoord + vec2( 0.7,  0.7) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2( 0.7,  1.5) * off);    
    color +=  1.0 * texture2D(uColor, vTexCoord + vec2( 1.5, -1.5) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2( 1.5, -0.7) * off);
    color +=  7.0 * texture2D(uColor, vTexCoord + vec2( 1.5,  0.0) * off);
    color +=  4.0 * texture2D(uColor, vTexCoord + vec2( 1.5,  0.7) * off);
    color +=  1.0 * texture2D(uColor, vTexCoord + vec2( 1.5,  1.5) * off);
    
    gl_FragColor = color / 273.0;
}
