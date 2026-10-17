#version 330

/*
Adapted from SAO_AO.pix by Morgan McGuire and Michael Mara, NVIDIA Research.
Source: https://research.nvidia.com/publication/2012-06_scalable-ambient-obscurance
Project adaptations use a different depth reconstruction, radius, bias, and output.

Copyright (c) 2011-2012, NVIDIA
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define NUM_SAMPLES      16
#define NUM_SPIRAL_TURNS 9
#define VARIATION  1

uniform float iGlobalTime;

uniform sampler2D iNormalTexture;
uniform sampler2D iDepthTexture;
uniform sampler2D iNoiseTexture;

uniform mat4 projection;
uniform mat4 inverseProjection;
uniform mat4 viewMatrix;
uniform mat4 viewProjectionInverseMatrix;

in vec4 color;
in vec2 texCoord;

uniform float uIntensity;
uniform vec2  uNoiseScale;
uniform float uSampleRadiusWS;
uniform float uBias;
uniform vec3 cameraPositionWorldSpace;

// reconstructs view-space unit normal from view-space position
vec3 reconstructNormalVS(vec3 positionVS) {
    return normalize(cross(dFdx(positionVS), dFdy(positionVS)));
}


vec3 getPositionVS(vec2 uv) {
    float depth = abs(texture2D(iDepthTexture, uv).x) * 2.0 * 60000.;

    vec2 uv2  = uv * 2.0 - vec2(1.0);
    vec4 temp = viewProjectionInverseMatrix * vec4(uv2, -1.0, 1.0);
    vec3 cameraFarPlaneWS = (temp / temp.w).xyz;

    vec3 cameraToPositionRay = normalize(cameraFarPlaneWS - cameraPositionWorldSpace);
    vec3 originWS = cameraToPositionRay * depth + cameraPositionWorldSpace;
    return originWS; // * .5 + .5;
}


// returns a unit vector and a screen-space radius for the tap on a unit disk
// (the caller should scale by the actual disk radius)
vec2 tapLocation(int sampleNumber, float spinAngle, out float radiusSS) {
    // radius relative to radiusSS
    float alpha = (float(sampleNumber) + 0.5) * (1.0 / float(NUM_SAMPLES));
    float angle = alpha * (float(NUM_SPIRAL_TURNS) * 6.28) + spinAngle;

    radiusSS = alpha;
    return vec2(cos(angle), sin(angle));
}


vec3 getOffsetPositionVS(vec2 uv, vec2 unitOffset, float radiusSS) {
    uv = uv + radiusSS * unitOffset * (1.0 / vec2(1920., 1080.));

    return getPositionVS(uv);
}


float sampleAO(vec2 uv, vec3 positionVS, vec3 normalVS, float sampleRadiusSS,
               int tapIndex, float rotationAngle)
{
    const float epsilon = 500.000001;
    float radius2 = uSampleRadiusWS * uSampleRadiusWS;

    // offset on the unit disk, spun for this pixel
    float radiusSS;
    vec2 unitOffset = tapLocation(tapIndex, rotationAngle, radiusSS);
    radiusSS *= sampleRadiusSS;

    vec3 Q = getOffsetPositionVS(uv, unitOffset, radiusSS);
    vec3 v = Q - positionVS;

    float vv = dot(v, v);
    float vn = dot(v, normalVS) - uBias;

#if VARIATION == 0

    // (from the HPG12 paper)
    // Note large epsilon to avoid overdarkening within cracks

    return float(vv < radius2) * max(vn / (epsilon + vv), 0.0);

#elif VARIATION == 1 // default / recommended

    // Smoother transition to zero (lowers contrast, smoothing out corners). [Recommended]
    float f = max(radius2 - vv, 0.0) / radius2;
    return f * f * f * max(vn / (epsilon + vv), 0.0);

#elif VARIATION == 2

    // Medium contrast (which looks better at high radii), no division.  Note that the
    // contribution still falls off with radius^2, but we've adjusted the rate in a way that is
    // more computationally efficient and happens to be aesthetically pleasing.
    float invRadius2 = 1.0 / radius2;
    return 4.0 * max(1.0 - vv * invRadius2, 0.0) * max(vn, 0.0);

#else

    // Low contrast, no division operation
    return 2.0 * float(vv < radius2) * max(vn, 0.0);

#endif
}

// Original repeatable arithmetic noise; values are in [0, 1).
vec3 nrand3(vec2 coordinate) {
    vec2 cell = floor(coordinate * 4096.0);
    float seed = mod(cell.x * 17.0 + cell.y * 131.0, 1009.0);
    vec3 mixed = mod((seed + vec3(13.0, 47.0, 83.0)) * vec3(73.0, 151.0, 199.0), 997.0);
    return mixed / 997.0;
}


#define PI 3.14159265359
void main() {
    vec2 adjustedUV = texCoord;

    if (texture2D(iNormalTexture, adjustedUV) == vec4(0)) {
        gl_FragColor = vec4(1.);
    } else {
        vec3 originVS = getPositionVS(adjustedUV);
        vec3 normalVS = reconstructNormalVS(originVS);

        vec3 sampleNoise = nrand3(adjustedUV * uNoiseScale);// * 2. - 1.;

        float randomPatternRotationAngle = 2.0 * PI * sampleNoise.x;

        float radiusSS  = 0.0; // radius of influence in screen space
        float radiusWS  = 0.0; // radius of influence in world space
        float occlusionNarrow = 0.0;
        float occlusionWide = 0.0;

        float projScale = 10.0;
        radiusWS = uSampleRadiusWS * 3.0;
        radiusSS = projScale * radiusWS / originVS.y;

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            occlusionNarrow += sampleAO(adjustedUV, originVS, normalVS, radiusSS, i, randomPatternRotationAngle);
        }

        occlusionNarrow = 1.0 - (occlusionNarrow / (float(NUM_SAMPLES)));
        occlusionNarrow = clamp(pow(occlusionNarrow, 30.0 + uIntensity), 0.0, 1.0);
/*
        radiusWS = uSampleRadiusWS * 3.0;
        radiusSS = projScale * radiusWS / originVS.y;

        for (int i = 0; i < NUM_SAMPLES; ++i) {
            occlusionWide += sampleAO(adjustedUV, originVS, normalVS, radiusSS, i, randomPatternRotationAngle);
        }

        occlusionWide = 1.0 - (occlusionWide / (float(NUM_SAMPLES)));
        occlusionWide = clamp(pow(occlusionWide, 14.0 + uIntensity), 0.0, 1.0);

        float occlusion = mix(occlusionNarrow, occlusionWide, 0.5);
*/
        float occlusion = occlusionNarrow;
        gl_FragColor = vec4(occlusion, occlusion, occlusion, 1.0);
    }
}
