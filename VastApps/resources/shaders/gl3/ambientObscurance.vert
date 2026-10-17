#version 330

in vec3 iPosition;
in vec4 iDiffuseColor;
in vec3 iNormal;
in vec2 iTexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec4 color;
out vec2 texCoord;
out vec3 cameraPos;

void main() {
    gl_Position = (ProjectionMatrix * ViewMatrix * vec4(iPosition, 1.0));
    texCoord = iTexCoord;
    color = iDiffuseColor;
}