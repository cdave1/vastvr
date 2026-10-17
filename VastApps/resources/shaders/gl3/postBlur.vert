#version 330

in vec3 iPosition;
in vec4 iColor;
in vec2 iTexCoord;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 texCoord;

void main() {
    gl_Position = (ProjectionMatrix * ViewMatrix * vec4(iPosition, 1.));
    texCoord = iTexCoord;
}