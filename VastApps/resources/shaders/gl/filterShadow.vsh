#version 330

attribute vec3 iPosition;
attribute vec4 iDiffuseColor;
attribute vec2 iTexCoord;

varying vec4 position;
varying vec2 texCoord;
varying vec4 color;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

void main() {
    texCoord = iTexCoord;
    color = iDiffuseColor;
    
    gl_Position = (gl_ModelViewProjectionMatrix * vec4(iPosition, 1.));
}