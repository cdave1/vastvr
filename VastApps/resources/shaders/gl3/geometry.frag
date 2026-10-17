#version 330

uniform sampler2D iTexture0;
uniform sampler2D iNormalTexture0;
uniform float Opacity;

uniform vec3 CameraWorldPosition;

in vec4 position;
in lowp vec4 color;
in vec3 normal;
in vec2 texCoord;

in lowp vec4 ambientColor;
in lowp vec4 diffuseColor;
in lowp vec4 specularColor;
in float materialShininess;

in vec3 vertexPosition;
in vec3 viewSpaceNormal;
in vec4 lightPosition;
in vec3 lightDirection;
in float viewSpaceDistance;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;

void main() {
    //gl_FragDepth = viewSpaceDistance;
}