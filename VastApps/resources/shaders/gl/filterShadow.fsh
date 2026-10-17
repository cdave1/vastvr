#version 330

uniform float iGlobalTime;

uniform sampler2D iDepthTexture;
uniform sampler2D iNormalTexture;
uniform sampler2D iShadowMap;

in vec4 position;
in vec2 texCoord;
in vec4 color;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 projection;
uniform mat4 inverseProjection;
uniform mat4 viewMatrix;
uniform mat4 viewProjectionInverseMatrix;
uniform vec3 cameraPositionWorldSpace;

const int sampleCount = 16;
const float shadowStep = 1.0 / sampleCount;
// Original stratified disk samples: four radii and four rotated quadrants.
vec2 shadowSample(int index) {
    float ring = float(index / 4);
    float angle = float(index % 4) * 1.570796327 + ring * 0.37;
    float radius = (ring + 1.0) * 0.25;
    return vec2(cos(angle), sin(angle)) * radius;
}

vec3 getPositionWS(vec2 uv) {
    float depth = abs(texture2D(iDepthTexture, uv).x) * 2.0 * 60000.;

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


float ShadowContribution(vec4 lightTextureCoord, vec3 n) {
    float distanceFromLight = texture2D(iShadowMap,lightTextureCoord.st).z * .5 + .5;

    vec3 noise = nrand3(texCoord);
    float noiseOffset = smoothstep(0.1, 1.0, noise.x * .5 + .5) * 1900.0;
    float bias = 0.00003;
    const int cutoff = 0;

    float visibility = 1.0;
    int check = 0;
    for (int i = 0; i < cutoff; i++) {
        float distanceFromLight = texture2D(iShadowMap, lightTextureCoord.st + shadowSample(i)/noiseOffset).z * .5 + .5;
        visibility = visibility - (shadowStep * step(distanceFromLight, lightTextureCoord.z - bias));
        check += int(step(distanceFromLight, lightTextureCoord.z - bias));
    }

   // if (check == 0) return 1.0;

    for (int i = cutoff; i < sampleCount; i++) {
        float distanceFromLight = texture2D(iShadowMap, lightTextureCoord.st + shadowSample(i)/noiseOffset).z * .5 + .5;
        visibility = visibility - (shadowStep * step(distanceFromLight, lightTextureCoord.z - bias));
    }
    return clamp(smoothstep(0.1, 0.9, visibility), 0.4, 1.0);
}


void main() {
    vec2 adjustedUV = texCoord; //vUV * vec2(2.0, 1.0);

    if (texture2D(iNormalTexture, adjustedUV) == vec4(0)) {
        gl_FragColor = vec4(1.);
    } else {
        vec4 realPosition = vec4(getPositionWS(adjustedUV), 1.);
    
        vec4 shadowCoord = (lightProjectionMatrix * lightModelViewMatrix * realPosition) * .5 + .5;

        vec4 shadowCoordinateWdivide = (shadowCoord / shadowCoord.w);

        const float shadowTextureCoordLim = 1.0;
        if (shadowCoord.s >= shadowTextureCoordLim || shadowCoord.t >= shadowTextureCoordLim) {
            gl_FragColor = vec4(1.);
        } else if (shadowCoord.s <= 0. || shadowCoord.t <= 0.) {
            gl_FragColor = vec4(1.);
        //} else if (shadowCoord.s <= 0. || shadowCoord.t >= shadowTextureCoordLim) {
        //    gl_FragColor = vec4(1.);
        //} else if (shadowCoord.s >= shadowTextureCoordLim || shadowCoord.t <= 0.) {
        //    gl_FragColor = vec4(1.);
        } else {
            float shadow = ShadowContribution(shadowCoordinateWdivide, reconstructNormalVS(realPosition.xyz));
            gl_FragColor = vec4(shadow, shadow, shadow, 1.0);
        }
    }
}
