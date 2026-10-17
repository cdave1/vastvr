#version 330

attribute vec4 iPosition;
attribute vec3 iNormal;
attribute vec2 iTexCoord;
attribute vec4 iColor;

uniform mat4 WorldMatrix;
uniform vec3 CameraWorldPosition;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;
varying vec3 cameraWorldPosition;

void main() {
    position = (WorldMatrix * iPosition);
    normal = normalize(gl_NormalMatrix * iNormal);
    texCoord = iTexCoord;
    color = iColor;

    cameraWorldPosition = CameraWorldPosition;

	gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
}