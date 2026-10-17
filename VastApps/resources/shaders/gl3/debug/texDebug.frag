#version 330

uniform sampler2D iTexture0;

in vec2 texCoord;
in vec4 color;

out vec4 FragColor;

void main() {
    FragColor = texture(iTexture0, texCoord);
}