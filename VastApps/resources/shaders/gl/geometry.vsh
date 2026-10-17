#version 330

attribute vec4 iPosition;
attribute vec4 iDiffuseColor;
attribute vec3 iNormal;
attribute vec2 iTexCoord;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;

varying vec4 worldPosition;

void main() {
    position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
    normal = iNormal;
    texCoord = iTexCoord;
    color = iDiffuseColor;
    
    gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);
}
