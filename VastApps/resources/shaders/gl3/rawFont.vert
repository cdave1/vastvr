#version 330

in vec4 iPosition;
in vec4 iColor;
in vec3 iNormal;
in vec2 iTexCoord;

uniform mat4 WorldMatrix;

out vec4 position;
out vec3 normal;
out vec2 texCoord;
out vec4 color;

out float logz;
out float Fcoef_half;

const float far = 10000.;

void main() {
    position = WorldMatrix * iPosition;
    texCoord = iTexCoord;
    color = iColor;

	gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
}