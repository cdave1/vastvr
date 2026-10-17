// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
uniform sampler2D iTexture0;
varying vec2 textureCoord;
varying vec4 colorVarying;
void main() { gl_FragColor = texture2D(iTexture0, textureCoord); }
