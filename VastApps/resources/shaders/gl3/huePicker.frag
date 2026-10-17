#version 330

uniform sampler2D iNormalTexture0;
uniform float Opacity;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

in vec3 vertexPosition;
in vec3 worldViewProjectionPosition;
in float viewSpaceDistance;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragNormal;
layout (location = 2) out vec4 FragDepth;

// Original six-sector HSV color wheel (full saturation/value).
vec3 hueGradient(float hue) {
    float segment = fract(hue) * 6.0;
    float fraction = fract(segment);
    int sector = int(floor(segment));
    if (sector == 0) return vec3(1.0, fraction, 0.0);
    if (sector == 1) return vec3(1.0 - fraction, 1.0, 0.0);
    if (sector == 2) return vec3(0.0, 1.0, fraction);
    if (sector == 3) return vec3(0.0, 1.0 - fraction, 1.0);
    if (sector == 4) return vec3(fraction, 0.0, 1.0);
    return vec3(1.0, 0.0, 1.0 - fraction);
}


void main() {
    vec3 rgb = hueGradient(texCoord.y);
    FragColor = vec4(rgb, Opacity);
    FragNormal = vec4(normal, 1.);

    //gl_FragDepth = viewSpaceDistance;
}
