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
uniform vec4 lightPositionWorldSpace;

out vec3 viewSpaceNormal;
out vec4 lightPositionViewSpace;
out vec3 lightDirection;

void main() {
    texCoord = iTexCoord;
    color = iDiffuseColor;

    //lightPosition = gl_LightSource[lightSourceIndex].position;
    //lightDirection = gl_LightSource[lightSourceIndex].spotDirection;
    //lightDirection = normalize((lightModelViewMatrix * LightPosition).xyz);
    lightPositionViewSpace = lightModelViewMatrix * lightPositionWorldSpace;
    
    gl_Position = (ProjectionMatrix * ViewMatrix * vec4(iPosition, 1.));
}