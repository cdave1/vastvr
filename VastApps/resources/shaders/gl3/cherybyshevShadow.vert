#version 330

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

out vec4 colorVarying;
out vec2 textureCoord;
out vec3 normalVarying;
out vec4 positionVarying;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

void main() {
    vec4 _position = vec4(position, 1.);
    textureCoord = texCoord;
    colorVarying = color;
    
    normalVarying = (gl_ModelViewMatrix * vec4(normal, 0.)).xyz;
    
    gl_Position = (gl_ModelViewProjectionMatrix * _position);
}