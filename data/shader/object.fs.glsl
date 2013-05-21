// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#version 120
precision mediump float;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;
uniform mat4 uView;
uniform vec3 uPosition;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec2 vUV;

mat3 matrixInverse(in mat3 m)
{  
   float det = dot(cross(m[0], m[1]), m[2]);
   mat3 t = transpose(m);
   return mat3(cross(t[1], t[2]), cross(t[2], t[0]), cross(t[0], t[1])) / det;
}

void main()
{
    // Retrieve texture
    vec3 diffuse = texture2D(uDiffuseMap, vUV).rgb;
    vec4 bump    = texture2D(uNormalMap, vUV);
    
    // Compute the normal
    vec3 normal = bump.xyz * 2.0 - 1.0;
    
    vec3 dpx = dFdx(vVertex), dpy = dFdy(vVertex);
    vec2 dtx = dFdx(vUV), dty = dFdy(vUV);       
    vec3 t = normalize(dpx * dty.t - dpy * dtx.s);
    vec3 b = normalize(dpy * dty.t - dpx * dtx.s);
    
    normal = (normalize(mat3(t, b, vNormal) * normal) + 1.0) * 0.5;
    
    // Write data to the gbuffer
    gl_FragData[0] = vec4(diffuse, bump.a);
    gl_FragData[1] = vec4(normal, 0.0);
    gl_FragData[2] = vec4(vVertex, -(uView * vec4(vVertex, 1.0)).z);
    gl_FragData[3] = vec4(0.0, 0.0, 0.0, 0.0);
}
