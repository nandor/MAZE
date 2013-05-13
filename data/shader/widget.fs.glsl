// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

uniform sampler2D uTexture;
uniform vec3 uColor;

varying vec2 vTexCoord;

void main()
{
    vec4 texColor = texture2D(uTexture, vTexCoord);
    gl_FragColor = vec4(uColor * texColor.rgb, texColor.a);
}