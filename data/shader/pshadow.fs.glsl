// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
precision mediump float;

// Geometry buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGPosition;

// Camera position
uniform vec3 uEyePosition;

uniform vec3 lDiffuse;	
uniform vec3 lSpecular;
uniform vec4 lPosition;

uniform int uWidth;
uniform int uHeight;

void main()
{
    vec2 texel       = gl_FragCoord.xy / vec2(uWidth, uHeight);
    vec4 geomDiffuse = texture2D(uGDiffuse, texel);
    vec3 geomNormal	 = texture2D(uGNormal, texel).xyz * 2.0 - vec3(1.0);
    vec3 geomPos	 = texture2D(uGPosition, texel).xyz;
            
    vec3 color = vec3(0.0);   
    vec3 lightDir = lPosition.xyz - geomPos;
    vec3 viewDir = normalize(uEyePosition - geomPos);
    
    float dist = length(lightDir);
    lightDir /= dist;    
    
    float angle = max(dot(geomNormal, lightDir), 0.0); 
    float att = max(1.0 - dist / lPosition.w, 0.0) * angle;
                
    float spec = max(dot(reflect(-lightDir, geomNormal), viewDir), 0.0);
    
    color += geomDiffuse.xyz * lDiffuse * att;
    color += lSpecular * pow(spec, geomDiffuse.w * 50.0) * att;
    
    gl_FragData[0].rgb = color;
}
