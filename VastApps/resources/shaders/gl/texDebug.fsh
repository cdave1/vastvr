#version 330

uniform sampler2D iTexture0;

in vec2 texCoord;
in vec4 color;

void main() {
    gl_FragColor = texture2D(iTexture0, texCoord);
}