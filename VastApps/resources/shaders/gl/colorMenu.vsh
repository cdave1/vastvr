#version 330

attribute vec4 iPosition;
attribute vec4 iDiffuseColor;
attribute vec3 iNormal;
attribute vec3 iTangent;
attribute vec2 iTexCoord;

uniform float iGlobalTime;
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraWorldPosition;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;

void main() {
    position = (WorldMatrix * iPosition);
    color = iDiffuseColor;
    normal = normalize(WorldMatrix * vec4(iNormal, 0.)).xyz;
    texCoord = iTexCoord;
    
    gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);

    cameraWorldPosition = CameraWorldPosition;
}
