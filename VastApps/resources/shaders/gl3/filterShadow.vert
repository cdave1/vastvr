#version 330

in vec3 iPosition;
in vec4 iDiffuseColor;
in vec2 iTexCoord;

out vec4 position;
out vec2 texCoord;
out vec4 color;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

void main() {
    texCoord = iTexCoord;
    color = iDiffuseColor;
    
    gl_Position = (ProjectionMatrix * ViewMatrix * vec4(iPosition, 1.));
}