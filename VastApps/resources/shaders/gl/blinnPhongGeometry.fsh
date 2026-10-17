#version 330

uniform sampler2D iTexture0;
uniform sampler2D iNormalTexture0;
uniform float Opacity;

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

in vec3 vertexPosition;
in vec3 worldViewProjectionPosition;
in vec4 lightPosition;
in vec3 lightDirection;
in vec3 cameraWorldPosition;

void main() {
    vec3 N = normalize(normal);
    
    float lambertian = max(dot(lightDirection, N), 0.0);
    float specular = 0.0;
    
    if (lambertian > 0.0) {
        vec3 viewDirection = normalize(-vertexPosition);
        vec3 halfDirection = normalize(lightDirection + viewDirection);
        float specAngle = max(dot(halfDirection, N), 0.0);
        specular = pow(specAngle, 16.0);
    }
    
    vec4 ambientColor = gl_FrontLightProduct[0].ambient;
    vec4 diffuseColor = gl_FrontLightProduct[0].diffuse;
    vec4 specColor = gl_FrontLightProduct[0].specular;

    vec3 rgb = (ambientColor + lambertian * diffuseColor + specular * specColor).rgb;

    gl_FragData[0] = vec4(rgb, Opacity);
    gl_FragData[1] = vec4(normal, 1.);

    float depth = length(position.xyz - cameraWorldPosition) / (60000.0 * 2.0);
    gl_FragDepth = depth;
}
