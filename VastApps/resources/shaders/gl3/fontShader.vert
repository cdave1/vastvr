#version 330

in vec4 iPosition;
in vec3 iNormal;
in vec2 iTexCoord;
in vec4 iColor;

uniform mat4 WorldMatrix;
uniform vec3 CameraWorldPosition;

out vec4 position;
out vec3 normal;
out vec2 texCoord;
out vec4 color;
out vec3 viewSpacePosition;

void main() {
    position = (WorldMatrix * iPosition);
    normal = normalize(gl_NormalMatrix * iNormal);
    texCoord = iTexCoord;
    color = iColor;

    viewSpacePosition = position.xyz - CameraWorldPosition;
	gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
}