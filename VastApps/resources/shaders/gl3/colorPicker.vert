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

out vec3 vertexPosition;
out vec3 worldViewProjectionPosition;
out float viewSpaceDistance;

void main() {
    mat4 viewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    position = (viewProjectionMatrix * WorldMatrix * iPosition);
    color = iDiffuseColor;
    normal = normalize(WorldMatrix * vec4(iNormal, 0.)).xyz;
    texCoord = iTexCoord;


    vec4 worldViewPosition = ViewMatrix * WorldMatrix * iPosition;
    vertexPosition = vec3(worldViewPosition) / worldViewPosition.w;
    worldViewProjectionPosition = (viewProjectionMatrix * iPosition).xyz;
    gl_Position = position;
    
    viewSpaceDistance = position.z;// / 60000.;
}
