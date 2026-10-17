#version 330

uniform sampler2D iTexture0;
uniform sampler2D iNormalTexture0;
uniform float Opacity;

uniform vec4 iMaterialAmbientColor;
uniform vec4 iMaterialDiffuseColor;
uniform vec4 iMaterialSpecularColor;
uniform vec3 CameraWorldPosition;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

in vec3 vertexPosition;
in vec3 worldViewProjectionPosition;
in float viewSpaceDistance;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;

void main() {
    if (texture(iTexture0, texCoord).a < 0.5) {
        discard;
    } else {
        vec3 rgb = iMaterialAmbientColor.rgb * texture(iTexture0, texCoord).rgb;
        FragColor = vec4(rgb, Opacity * texture(iTexture0, texCoord).a);
        FragNormal = vec4(normal, 1.);

        //gl_FragDepth = viewSpaceDistance;
    }
}
