#version 330

uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

void main() {
    gl_FragData[0] = vec4(vec3(position.z), 1.);
    gl_FragDepth = position.z;
}