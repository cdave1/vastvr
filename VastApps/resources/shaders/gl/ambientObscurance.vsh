#version 330

in vec3 iPosition;
in vec4 iDiffuseColor;
in vec3 iNormal;
in vec2 iTexCoord;

out vec4 color;
out vec2 texCoord;
out vec3 cameraPos;

void main() {
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(iPosition, 1.0));
    texCoord = iTexCoord;
    color = iDiffuseColor;
}