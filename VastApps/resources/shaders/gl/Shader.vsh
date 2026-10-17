#version 330

attribute vec3 iPosition;
attribute vec4 iColor;
attribute vec3 iNormal;
attribute vec2 iTexCoord;

varying vec4 position;
varying vec3 normal;
varying vec2 texCoord;
varying vec4 color;

void main() {
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(iPosition, 1.));
    position = vec4(iPosition, 1.);
    normal = normalize(gl_NormalMatrix * iNormal);
    texCoord = iTexCoord;
    color = iColor;
}
