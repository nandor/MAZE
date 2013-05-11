// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120

// Geometry buffer
uniform sampler2D uGPosition;
uniform sampler2D uGColor;

// Noise parameters
uniform sampler2D uNoisePerm;
uniform sampler1D uNoiseSimplex;
uniform sampler2D uNoiseGrad;

// Camera position
uniform vec3 uViewDir;
uniform vec3 uPosition;
uniform float uFarPlane;

// Fog parameters
uniform vec3 uFogColor;
uniform float uFogDensity;
uniform float uFogHeight;
uniform float uTime;

varying vec2 vTexCoord;
  
void main()
{
    vec3 position = texture2D(uGPosition, vTexCoord).xyz;
    vec3 color = texture2D(uGColor, vTexCoord).xyz;  
    vec3 dir = position - uPosition;  
    
    float time = uTime / 700.0;
    float off = vTexCoord.x + atan(uViewDir.z, uViewDir.x);
    float height = sin(off * 20.0 + time) * cos(off * 40.0 - time);
    
    float distAtt = length(dir) / (uFarPlane - 5.0);
    distAtt = clamp(pow(distAtt, 2), 0.0, 1.0);
    
    float heightAtt = (position.y - uPosition.y) / (uFogHeight + height);
    heightAtt = heightAtt < 0.0 ? 1.0 : clamp(1.0 - pow(heightAtt, 1.5), 0.0, 1.0);
        
    gl_FragColor = vec4(mix(color, uFogColor, distAtt * heightAtt), 1.0);
}
