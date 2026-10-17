#version 330
// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
uniform float iGlobalTime;
uniform sampler2D iColorTexture;
uniform sampler2D iDepthTexture;
uniform sampler2D iShadowTexture;
uniform sampler2D iAmbientOcclusionTexture;
uniform vec2 uTextureSize;
uniform int uBlurSize;
in vec2 texCoord;
out vec4 FragColor;

void main() {
    int width = clamp(uBlurSize, 1, 16);
    vec2 stepSize = 1.0 / max(uTextureSize, vec2(1.0));
    vec4 total = vec4(0.0);
    for (int row = 0; row < width; ++row) {
        for (int column = 0; column < width; ++column) {
            vec2 offset = (vec2(column, row) - vec2(float(width - 1) * 0.5)) * stepSize;
            total += texture(iShadowTexture, texCoord + offset);
        }
    }
    vec4 average = total / float(width * width);
    FragColor = average;
}
