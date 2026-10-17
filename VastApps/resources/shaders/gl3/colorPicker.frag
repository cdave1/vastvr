#version 330

uniform sampler2D iNormalTexture0;
uniform vec4 iMaterialDiffuseColor;
uniform float Opacity;

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

vec3 colorGradient(vec2 uv) {
    vec3 strength = mix(iMaterialDiffuseColor.xyz, vec3(1.), smoothstep(0.05, 0.95, uv.x));
    vec3 brightness = mix(strength, vec3(0.), 1.0 - uv.y);
    return brightness;
}


void main() {
    vec3 rgb = colorGradient(texCoord);
    FragColor = vec4(rgb, Opacity);
    FragNormal = vec4(normal, 1.);

    //gl_FragDepth = viewSpaceDistance;
}
