// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

uniform sampler2D uTexture;

uniform int uWidth;
uniform int uHeight;
varying vec2 vTexCoord;

void main()
{
    vec4 color = vec4(0.0);
    
    color += 1.0 * texture2D(uTexture, vTexCoord + vec2(1.0 / uWidth,  1.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(1.0 / uWidth,  2.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(1.0 / uWidth,  3.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(1.0 / uWidth,  4.0 / uHeight));
    color += 1.0 * texture2D(uTexture, vTexCoord + vec2(1.0 / uWidth,  5.0 / uHeight));    
    
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(2.0 / uWidth,  1.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(2.0 / uWidth,  2.0 / uHeight));
    color += 4.0 * texture2D(uTexture, vTexCoord + vec2(2.0 / uWidth,  3.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(2.0 / uWidth,  4.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(2.0 / uWidth,  5.0 / uHeight));  
    
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(3.0 / uWidth,  1.0 / uHeight));
    color += 4.0 * texture2D(uTexture, vTexCoord + vec2(3.0 / uWidth,  2.0 / uHeight));
    color += 5.0 * texture2D(uTexture, vTexCoord + vec2(3.0 / uWidth,  3.0 / uHeight));
    color += 4.0 * texture2D(uTexture, vTexCoord + vec2(3.0 / uWidth,  4.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(3.0 / uWidth,  5.0 / uHeight));   
    
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(4.0 / uWidth,  1.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(4.0 / uWidth,  2.0 / uHeight));
    color += 4.0 * texture2D(uTexture, vTexCoord + vec2(4.0 / uWidth,  3.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(4.0 / uWidth,  4.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(4.0 / uWidth,  5.0 / uHeight));
    
    color += 1.0 * texture2D(uTexture, vTexCoord + vec2(5.0 / uWidth,  1.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(5.0 / uWidth,  2.0 / uHeight));
    color += 3.0 * texture2D(uTexture, vTexCoord + vec2(5.0 / uWidth,  3.0 / uHeight));
    color += 2.0 * texture2D(uTexture, vTexCoord + vec2(5.0 / uWidth,  4.0 / uHeight));
    color += 1.0 * texture2D(uTexture, vTexCoord + vec2(5.0 / uWidth,  5.0 / uHeight));
    
    gl_FragColor = color / 65.0;
}
