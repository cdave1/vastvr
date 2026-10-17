#version 330

in vec3 iPosition;
in vec4 iColor;
in vec3 iNormal;
in vec2 iTexCoord;

out vec4 position;
out vec3 normal;
out vec2 texCoord;
out vec4 color;

void main() {
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(iPosition, 1.));
    texCoord = iTexCoord;
    color = iColor;
}
