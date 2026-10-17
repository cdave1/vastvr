#version 330

in vec4 iPosition;
in lowp vec4 iDiffuseColor;
in vec3 iNormal;
in vec2 iTexCoord;

in mat4 iTransform;
in lowp vec4 iMaterialAmbientColor;
in lowp vec4 iMaterialDiffuseColor;
in lowp vec4 iMaterialSpecularColor;
in float iMaterialShininess;

uniform vec4 LightAmbientColor;
uniform vec4 LightDiffuseColor;
uniform vec4 LightSpecularColor;

uniform float iGlobalTime;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraWorldPosition;
uniform vec4 LightPosition;
//uniform int lightSourceIndex;

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
    position = (ProjectionMatrix * ViewMatrix * iTransform * iPosition);
    gl_Position = position;
    viewSpaceDistance = position.z;// / 60000.;
}
