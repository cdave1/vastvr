#version 330

in vec4 iPosition;
in vec4 iDiffuseColor;
in vec3 iNormal;
in vec3 iTangent;
in vec2 iTexCoord;

uniform float iGlobalTime;
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraWorldPosition;

out vec4 position;
out vec3 normal;
out vec2 texCoord;
out vec4 color;

out float viewSpacePosition;

void main() {
    position = (ProjectionMatrix * ViewMatrix * WorldMatrix * iPosition);
    color = iDiffuseColor;
    normal = normalize(WorldMatrix * vec4(iNormal, 0.)).xyz;
    texCoord = iTexCoord;
    
    gl_Position = position;

    viewSpacePosition = position.z;// / 60000.;
}
