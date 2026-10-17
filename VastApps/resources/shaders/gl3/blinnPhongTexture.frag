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
in float viewSpaceDistance;

layout (location = 0) out lowp vec4 FragColor;
layout (location = 1) out lowp vec4 FragLighting;
layout (location = 2) out lowp vec4 FragNormal;
//layout (location = 2) out vec4 FragDepth;

void main() {
    vec3 N = normalize(viewSpaceNormal);
    vec3 lightDir = normalize(lightPosition.xyz);

    float lambertian = max(dot(lightDir,N), 0.0);
    float specular = 0.0;

    if (lambertian > 0.0) {

        vec3 reflectDir = reflect(-lightDir, N);
        vec3 viewDir = normalize(-vertexPosition);

        float specAngle = max(dot(reflectDir, viewDir), 0.0);
        specular = pow(specAngle, materialShininess);
    }

    vec3 rgb = texture(iTexture0, texCoord).rgb + (lambertian * diffuseColor.rgb + specular * specularColor.xyz);


    FragColor = vec4(rgb, Opacity);
    FragNormal = vec4(normal, 1.);

    //gl_FragDepth = viewSpaceDistance;
}
