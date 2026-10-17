#version 330

uniform float iGlobalTime;

uniform sampler2D iColorTexture;
uniform sampler2D iLightingTexture;
uniform sampler2D iDepthTexture;
uniform sampler2D iNormalTexture;
uniform sampler2D iShadowMap;

in vec4 position;
in vec2 texCoord;
in vec4 color;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 inverseProjectionMatrix;
uniform mat4 inverseViewMatrix;
uniform vec3 cameraPositionWorldSpace;
uniform vec3 lightPositionWorldSpace;
uniform vec2 shadowMapSize;

out vec4 FragColor;

const int sampleCount = 16;
const float shadowStep = 1.0 / sampleCount;
// Original stratified disk samples: four radii and four rotated quadrants.
vec2 shadowSample(int index) {
    float ring = float(index / 4);
    float angle = float(index % 4) * 1.570796327 + ring * 0.37;
    float radius = (ring + 1.0) * 0.25;
    return vec2(cos(angle), sin(angle)) * radius;
}


vec3 getPositionFromLinearWS(vec2 uv) {
    float depth = texture(iDepthTexture, uv).x * 2. - 1.;

    vec2 uv2  = uv * 2.0 - vec2(1.0);
    vec4 temp = inverseProjectionMatrix * inverseViewMatrix * vec4(uv2, -1.0, 1.0);
    vec3 cameraFarPlaneWS = (temp / temp.w).xyz;

    vec3 cameraToPositionRay = normalize(cameraFarPlaneWS - cameraPositionWorldSpace);
    vec3 originWS = cameraToPositionRay * depth + cameraPositionWorldSpace;
    //originWS.z = originWS.z * 60000.;
    return originWS.xyz;
}


// Recover a world point from normalized window coordinates and depth.
vec3 getPositionWS(vec2 uv) {
    vec4 windowPoint = vec4(uv, texture(iDepthTexture, uv).r, 1.0);
    vec4 clipPoint = vec4(windowPoint.xyz * 2.0 - 1.0, 1.0);
    vec4 worldPoint = inverseViewMatrix * (inverseProjectionMatrix * clipPoint);
    return worldPoint.xyz / worldPoint.w;
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


// Accumulate depth comparison between the window space depth buffer, and the pre-calculated light depth buffer.
//
float ShadowContribution(vec4 lightTextureCoord, vec3 n) {
    float distanceFromLight = texture(iShadowMap,lightTextureCoord.st).x;

    vec3 noise = nrand3(texCoord) * 2. - 1.;
    vec2 noiseOffset = noise.xy * 0.002;
    float bias = 0.00003;
    const int cutoff = 4;

    float visibility = 1.0;
    int check = 0;
    for (int i = 0; i < cutoff; i++) {
        float distanceFromLight = texture(iShadowMap, lightTextureCoord.st + shadowSample(i) * noiseOffset).x;
        float diff = step(distanceFromLight, lightTextureCoord.z - bias);
        visibility = visibility - (shadowStep * diff);
        check += int(diff);
    }

    if (check == 0) return 1.0;

    for (int i = cutoff; i < sampleCount; i++) {
        float distanceFromLight = texture(iShadowMap, lightTextureCoord.st + shadowSample(i) * noiseOffset).x;
        visibility = visibility - (shadowStep * step(distanceFromLight, lightTextureCoord.z - bias));
    }

    return visibility; //clamp(visibility, 0.0, 1.0);
}


#define RECONSTRUCT_NORMAL_FROM_DEPTH

void main() {
    vec2 adjustedUV = texCoord;

    vec4 color = texture(iColorTexture, adjustedUV);
    vec4 lighting = texture(iLightingTexture, adjustedUV);

    if (texture(iNormalTexture, adjustedUV) == vec4(0)) {
        FragColor = color + lighting;
    } else {
        vec4 realPosition = vec4(getPositionWS(adjustedUV), 1.);
        vec4 shadowCoord = (lightProjectionMatrix * lightModelViewMatrix * realPosition) * .5 + .5;
        vec4 shadowCoordinateWdivide = (shadowCoord / shadowCoord.w);

        const float shadowTextureCoordLim = 1.0;
        if (shadowCoord.s >= shadowTextureCoordLim || shadowCoord.t >= shadowTextureCoordLim) {
            FragColor = color + lighting;
        } else if (shadowCoord.s <= 0. || shadowCoord.t <= 0.) {
            FragColor = color + lighting;
        } else if (shadowCoord.s <= 0. || shadowCoord.t >= shadowTextureCoordLim) {
            FragColor = color + lighting;
        } else if (shadowCoord.s >= shadowTextureCoordLim || shadowCoord.t <= 0.) {
            FragColor = color + lighting;
        } else {
#ifdef RECONSTRUCT_NORMAL_FROM_DEPTH
            float shadow = ShadowContribution(shadowCoordinateWdivide, reconstructNormalVS(realPosition.xyz));
#else
            float shadow = ShadowContribution(shadowCoordinateWdivide, texture(iNormalTexture, adjustedUV).xyz);
#endif

            ///shadow = 0.4 + (0.6 * ((shadow - 0.1) / 0.9));
            //shadow = smoothstep(0.0, 1.0, shadow);
            FragColor = vec4(shadow * lighting.r, shadow * lighting.g, shadow * lighting.b, 1.0) + color; //(shadow * 0.5 + 0.5) * color;
        }
    }
}
