#version 330

uniform float iGlobalTime;

uniform sampler2D iDepthTexture;
uniform sampler2D iNormalTexture;
uniform sampler2DShadow iShadowMap;

in vec4 position;
in vec2 texCoord;
in vec4 color;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 inverseProjection;
uniform mat4 viewProjectionInverseMatrix;
uniform vec3 cameraPositionWorldSpace;
uniform vec2 shadowMapSize;

in vec4 lightPositionViewSpace;

out vec4 FragColor;

const int SAMPLES_COUNT = 16;
const float INV_SAMPLES_COUNT = 1.0 / SAMPLES_COUNT;
// Original stratified disk samples: four radii and four rotated quadrants.
vec2 shadowSample(int index) {
    float ring = float(index / 4);
    float angle = float(index % 4) * 1.570796327 + ring * 0.37;
    float radius = (ring + 1.0) * 0.25;
    return vec2(cos(angle), sin(angle)) * radius;
}

vec3 getPositionWS(vec2 uv) {
    float depth = texture(iDepthTexture, uv).x * 60000.;

    vec2 uv2  = uv * 2.0 - vec2(1.0);
    vec4 temp = viewProjectionInverseMatrix * vec4(uv2, -1.0, 1.0);
    vec3 cameraFarPlaneWS = (temp / temp.w).xyz;

    vec3 cameraToPositionRay = normalize(cameraFarPlaneWS - cameraPositionWorldSpace);
    vec3 originWS = cameraToPositionRay * depth + cameraPositionWorldSpace;
    return originWS;
}


vec3 reconstructNormalVS(vec3 positionVS) {
    return normalize(cross(dFdx(positionVS), dFdy(positionVS)));
}


// Original repeatable arithmetic noise; values are in [0, 1).
vec3 nrand3(vec2 coordinate) {
    vec2 cell = floor(coordinate * 4096.0);
    float seed = mod(cell.x * 17.0 + cell.y * 131.0, 1009.0);
    vec3 mixed = mod((seed + vec3(13.0, 47.0, 83.0)) * vec3(73.0, 151.0, 199.0), 997.0);
    return mixed / 997.0;
}


float ShadowContribution(vec4 lightTextureCoord, vec3 normal) {
    return textureProj(iShadowMap, lightTextureCoord);
    /*
    float shadow = 0;
    float fsize = lightTextureCoord.w * shadowMapSize.x;
    vec4 shadowMapCoord = lightTextureCoord;

    for (int i = 0; i < SAMPLES_COUNT; i++) {
        shadowMapCoord.xy = shadowSample(i) * fsize + lightTextureCoord.xy;
        shadow += textureProj(iShadowMap, shadowMapCoord) * INV_SAMPLES_COUNT;
    }

    vec3 N = normalize(normal);
    vec3 L = normalize(lightPositionViewSpace.xyz);
    vec3 R = normalize(-reflect(L, N));

    float NdotL = max(dot(N, L), 0);

    vec4 color = vec4(1.);
    return ((color * NdotL + pow(max(dot(R, L), 0), 64)) * shadow + color * 0.5).x;*/
}


/*
float ShadowColor(vec4 lightTextureCoord, vec3 normal) {
    float shadow = 0;

    const int SAMPLE_CUTOFF_MAX = 4;

    for (int i = 0; i < SAMPLE_CUTOFF_MAX; ++i) {

        vec2 offset = normalize(shadowSample(i))/shadowMapSize;
        shadow += texture(iShadowMap, lightTextureCoord.st + offset).x;
    }

    vec3 N = normalize(normal);
    vec3 L = normalize(

}*/


void main() {
    vec2 adjustedUV = texCoord;

    if (texture(iNormalTexture, adjustedUV) == vec4(0)) {
        FragColor = vec4(1.);
    } else {
        vec4 realPosition = vec4(getPositionWS(adjustedUV), 1.);
        vec4 shadowCoord = (lightProjectionMatrix * lightModelViewMatrix * realPosition);
        float shadow = ShadowContribution(shadowCoord, reconstructNormalVS(realPosition.xyz));
        FragColor = vec4(shadow, shadow, shadow, 1.0);
    }
}
