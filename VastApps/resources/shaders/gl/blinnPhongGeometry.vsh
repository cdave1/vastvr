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
uniform float EntityId;
//uniform int lightSourceIndex;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;

varying vec3 vertexPosition;
varying vec3 worldViewProjectionPosition;
varying vec4 lightPosition;
varying vec3 lightDirection;

varying vec3 cameraWorldPosition;

void main() {
    position = (WorldMatrix * iPosition);
    color = iDiffuseColor;
    normal = normalize(WorldMatrix * vec4(iNormal, 0.)).xyz;
    texCoord = iTexCoord;
    
    vec4 worldViewPosition = gl_ModelViewMatrix * WorldMatrix * iPosition;
    vertexPosition = vec3(worldViewPosition) / worldViewPosition.w;
    worldViewProjectionPosition = (gl_ModelViewProjectionMatrix * iPosition).xyz;
    gl_Position = (gl_ModelViewProjectionMatrix * WorldMatrix * iPosition);

    //lightPosition = gl_LightSource[lightSourceIndex].position;
    //lightDirection = gl_LightSource[lightSourceIndex].spotDirection;
    lightPosition = gl_LightSource[0].position;
    lightDirection = normalize(lightPosition.xyz - vertexPosition);

    cameraWorldPosition = CameraWorldPosition;
}
