#version 330

in vec4 iPosition;
in lowp vec4 iDiffuseColor;
in vec3 iNormal;
in vec2 iTexCoord;

uniform float iGlobalTime;
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraWorldPosition;
uniform vec4 LightPosition;
//uniform int lightSourceIndex;

uniform vec4 iMaterialAmbientColor;
uniform vec4 iMaterialDiffuseColor;
uniform vec4 iMaterialSpecularColor;
uniform float iMaterialShininess;

uniform vec4 LightAmbientColor;
uniform vec4 LightDiffuseColor;
uniform vec4 LightSpecularColor;

out vec4 position;
out vec3 normal;
out vec2 texCoord;
out lowp vec4 color;

out lowp vec4 ambientColor;
out lowp vec4 diffuseColor;
out lowp vec4 specularColor;
out float materialShininess;

out vec3 vertexPosition;
out vec3 viewSpaceNormal;
out vec4 lightPosition;
out vec3 lightDirection;
out float viewSpaceDistance;

void main() {
    position = (ProjectionMatrix * ViewMatrix * WorldMatrix * iPosition);
    color = iDiffuseColor;
    normal = normalize(WorldMatrix * vec4(iNormal, 0.)).xyz;
    texCoord = iTexCoord;

    vec4 worldViewPosition = ViewMatrix * WorldMatrix * iPosition;
    vertexPosition = vec3(worldViewPosition) / worldViewPosition.w;
    viewSpaceNormal = normalize(ViewMatrix  * WorldMatrix * vec4(iNormal, 0.)).xyz;
    gl_Position = position;

    lightDirection = normalize((ViewMatrix * LightPosition).xyz);
    lightPosition = ViewMatrix * LightPosition;

    ambientColor = LightAmbientColor * iMaterialAmbientColor;
    diffuseColor = LightDiffuseColor * iMaterialDiffuseColor;
    specularColor = LightSpecularColor * iMaterialSpecularColor;
    materialShininess = iMaterialShininess;
    viewSpaceDistance = position.z;// / 60000.;
}
