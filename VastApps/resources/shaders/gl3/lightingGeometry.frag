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
in vec4 lightPosition;
in vec3 lightDirection;
in vec3 viewSpacePosition;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;

void main() {
    vec3 N = normal; //normalize(normal);
    
    float lambertian = max(dot(lightDirection, N), 0.0);
    float specular = 0.0;
    
    if (lambertian > 0.0) {
        vec3 viewDirection = normalize(-vertexPosition);
        vec3 halfDirection = normalize(lightDirection + viewDirection);
        float specAngle = max(dot(halfDirection, N), 0.0);
        specular = pow(specAngle, 16.0);
    }

    vec3 rgb = (iMaterialAmbientColor + lambertian * iMaterialDiffuseColor + specular * iMaterialSpecularColor).rgb;

    FragColor = vec4(rgb, Opacity);
    FragNormal = vec4(normal, 1.);

    float depth = length(viewSpacePosition) / 60000.0;
    gl_FragDepth = depth;
}
