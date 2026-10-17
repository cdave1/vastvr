// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#version 330
uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float uAmplitude;
uniform sampler2D iTexture0;
uniform vec4 iMouse;
in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;
// Original repeatable arithmetic noise; values are in [0, 1).
vec3 nrand3(vec2 coordinate) {
    vec2 cell = floor(coordinate * 4096.0);
    float seed = mod(cell.x * 17.0 + cell.y * 131.0, 1009.0);
    vec3 mixed = mod((seed + vec3(13.0, 47.0, 83.0)) * vec3(73.0, 151.0, 199.0), 997.0);
    return mixed / 997.0;
}
void main() {
    vec2 uv = texCoord;
    vec2 cells = uv * vec2(96.0, 64.0);
    vec3 randomValue = nrand3(floor(cells));
    vec2 center = vec2(0.2) + randomValue.xy * 0.6;
    float distanceToStar = length(fract(cells) - center);
    float star = (1.0 - smoothstep(0.025, 0.09, distanceToStar)) * step(0.90, randomValue.z);
    float pulse = 0.7 + 0.3 * sin(iGlobalTime * 0.6 + randomValue.z * 6.283185307);
    vec3 background = mix(vec3(0.015, 0.02, 0.055), vec3(0.08, 0.04, 0.13), clamp(uv.y, 0.0, 1.0));
    vec4 result = vec4(background + star * pulse * vec3(0.85, 0.91, 1.0), 1.0);
    gl_FragColor = result;
}
