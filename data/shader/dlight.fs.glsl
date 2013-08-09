// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
#extension GL_EXT_texture_array : enable
precision mediump float;

// Geometry buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGPosition;

// Eye position
uniform vec3 uPosition;

// Light parameters
uniform vec3 lDiffuse;
uniform vec3 lSpecular;
uniform vec3 lAmbient;
uniform vec4 lDirection;

varying vec2 vUV;

void main()
{
    // Retrieve data from gbuffer
    vec4 geomDiffuse  = texture2D(uGDiffuse,  vUV);
    vec4 geomPosition = texture2D(uGPosition, vUV);
    vec4 geomNormal	  = texture2D(uGNormal,   vUV);
    
    // Unpack data
    vec3 normal   = geomNormal.xyz * 2.0 - vec3(1.0);
    vec3 diffuse  = geomDiffuse.xyz;
    vec3 position = geomPosition.xyz;
    float depth   = geomPosition.w;
    float specExp = geomDiffuse.a * 255.0;
    
    // View dir for specular highlights
    vec3 viewDir = normalize(uPosition - position);
    
    // Compute intensities
    float angle = max(dot(-lDirection.xyz, normal), 0.0);    
    float spec = dot(reflect(lDirection.xyz, normal), viewDir);
    
    // Compute color components    
    vec3 ia = diffuse * lAmbient;
    vec3 id = diffuse * lDiffuse * angle;
    vec3 is = diffuse * lSpecular * angle * max(pow(spec, specExp), 0.0);
       
    // Compute the final color
    gl_FragData[0] = vec4(ia + id + is, 1.0);
}
