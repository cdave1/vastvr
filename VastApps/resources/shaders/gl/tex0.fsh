#version 330

uniform float iGlobalTime;
uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

void main() {
    gl_FragData[0] = texture2D(iTexture0, texCoord);
    gl_FragData[1] = vec4(0.0);
}