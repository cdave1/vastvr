#version 330

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 textureCoord;
varying vec3 normalVarying;
varying vec4 positionVarying;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

void main() {
    vec4 _position = vec4(position, 1.);
    textureCoord = texCoord;
    colorVarying = color;
    
    normalVarying = (gl_ModelViewMatrix * vec4(normal, 0.)).xyz;
    
    gl_Position = (gl_ModelViewProjectionMatrix * _position);
}