// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_texture_array : enable

precision mediump float;

// Geometry buffer
uniform sampler2D uDiffuse;
uniform sampler2D uNormal;
uniform sampler2D uPosition;

// Shadowing
uniform sampler2DArrayShadow uShadow;
uniform mat4 uShadowMVP0;
uniform mat4 uShadowMVP1;
uniform mat4 uShadowMVP2;
uniform mat4 uShadowMVP3;
uniform vec4 uShadowZ;

// Eye position for specular highlights
uniform vec3 uEyePosition;

// Light parameters
uniform vec3 lDiffuse;
uniform vec3 lSpecular;
uniform vec3 lAmbient;

uniform vec4 lDirection;

// Fragment position
varying vec2 vTexCoord;

/**
    Entry point of the fragment shader
*/
void main()
{
    vec4 geomDiffuse = texture2D(uDiffuse,  vTexCoord);
    vec4 geomPos	 = texture2D(uPosition, vTexCoord);
    vec3 geomNormal	 = texture2D(uNormal,   vTexCoord).xyz * 2.0 - vec3(1.0);
    
    vec3 color = vec3(0.0), viewDir, ambient;
    float angle, spec;
    
    // Ambient & diffuse component
    angle = max(dot(geomNormal, -lDirection.xyz), 0.0);    
    viewDir = normalize(uEyePosition - vec3(geomPos));
    spec = max(dot(reflect(lDirection.xyz, geomNormal), viewDir), 0.0);   
    
    ambient = lAmbient * geomDiffuse.xyz;
    color += geomDiffuse.xyz * lDiffuse;
    color += pow(spec, geomDiffuse.w * 100.0) * lSpecular;
    
    // Shadow intensity    
    float shadowLevel = 0.0;
    float shadow = 0.0;
    vec4 shadowPos;
        
    if (uShadowZ.w < geomPos.w)
    {   
        shadowLevel = 3.0;
        shadowPos = uShadowMVP3 * vec4(geomPos.xyz, 1.0);
    }
    else if (uShadowZ.z < geomPos.w)
    {
        shadowLevel = 2.0;
        shadowPos = uShadowMVP2 * vec4(geomPos.xyz, 1.0);
    }
    else if (uShadowZ.y < geomPos.w)
    {
        shadowLevel = 1.0;
        shadowPos = uShadowMVP1 * vec4(geomPos.xyz, 1.0);
    }
    else
    {
        shadowLevel = 0.0;
        shadowPos = uShadowMVP0 * vec4(geomPos.xyz, 1.0);
    }
    
    shadowPos.w = shadowPos.z;
    shadowPos.z = shadowLevel;
        
    for (float x = -1.5; x <= 1.5; x += 1.0) 
    {
        for (float y = -1.5; y <= 1.5; y += 1.0)
        {
            shadow += shadow2DArray(uShadow, shadowPos + vec4(x / 2000.0, y / 2000.0, 0.0, -0.001)).x;
        }
    }
   
    gl_FragColor.rgb = ambient + color * angle * shadow / 16.0;
}
