#version 330

uniform sampler2D iTexture0;
uniform float Opacity;

uniform vec4 iMaterialAmbientColor;
uniform vec4 iMaterialDiffuseColor;
uniform vec4 iMaterialSpecularColor;
uniform vec3 CameraWorldPosition;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;
in float viewSpacePosition;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;

void main() {
    FragColor = vec4(iMaterialAmbientColor.rgb, Opacity);
    FragNormal = vec4(normal, 1.0);
    //gl_FragDepth = viewSpacePosition;
}
