#version 330

in vec3 iPosition;
in vec4 iDiffuseColor;
in vec3 iNormal;
in vec2 iTexCoord;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 position;
out vec2 texCoord;
out vec4 color;

void main() {
    gl_Position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(iPosition, 1.);
    texCoord = iTexCoord;
    color = iDiffuseColor;
}