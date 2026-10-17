#version 330

attribute vec4 iPosition;
attribute vec4 iColor;
attribute vec3 iNormal;
attribute vec2 iTexCoord;

uniform mat4 WorldMatrix;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;

varying float logz;
varying float Fcoef_half;

const float far = 10000.;

void main() {
    position = WorldMatrix * iPosition;
    texCoord = iTexCoord;
    color = iColor;

	gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
}