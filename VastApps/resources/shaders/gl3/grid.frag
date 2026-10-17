#version 330

uniform sampler2D iTexture0;

uniform vec4 iMaterialAmbientColor;
uniform vec3 CameraWorldPosition;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

in vec3 vertexPosition;
in float collisionSpaceDistance;
in float viewSpaceDistance;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;

void main() {
    float depth = (viewSpaceDistance);
    float collisionDepth = (collisionSpaceDistance);

    // The closer the depth and collision depths are, the brighter we want the grid to be.
    //float opacity = smoothstep(0.0f, 0.01f, 1.0 - (abs(depth - collisionDepth) / collisionDepth));
    float opacity = 1.0;// - clamp((abs(depth - collisionDepth) / 200.0f), 0.0, 1.0);

    FragColor = vec4(iMaterialAmbientColor.rgb, opacity);
    FragNormal = vec4(normal, 1.);

    //gl_FragDepth = viewSpaceDistance;
}
