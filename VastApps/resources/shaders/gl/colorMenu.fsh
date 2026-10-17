#version 330

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

in vec3 cameraWorldPosition;

void main() {
    gl_FragData[0] = color;
    gl_FragData[1] = 0.5 * vec4(normal, 1.) + 0.5;

    float depth = length(position.xyz - cameraWorldPosition) / (60000.0 * 2.0);
    gl_FragDepth = depth;
}