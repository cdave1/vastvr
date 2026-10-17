#version 330

attribute vec3 iPosition;
attribute vec4 iColor;
attribute vec2 iTexCoord;

varying vec2 texCoord;

void main() {
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(iPosition, 1.));
    texCoord = iTexCoord;
}