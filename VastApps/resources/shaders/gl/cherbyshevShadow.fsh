#version 330

uniform float iGlobalTime;

uniform sampler2D iColorTexture;
uniform sampler2D iPositionTexture;
uniform sampler2D iNormalTexture;
uniform sampler2D iDepthTexture;

uniform sampler2D iShadowMap;

in vec4 colorVarying;
in vec3 normalVarying;
in vec2 textureCoord;
in vec4 shadowCoord;

uniform mat4 lightModelViewMatrix;
uniform mat4 lightProjectionMatrix;

uniform mat4 inverseViewProjMatrix;

uniform vec3 cameraPositionWorldSpace;


float linstep (float min, float max, float v) {
    return clamp((v - min) / (max - min), 0., 1.);
}


float ChebyshevUpperBound(vec2 moments, float t, vec4 lightTextureCoord) {
    const float _minVariance = 0.001;
    float p = float(t <= moments.x);//(t < lightTextureCoord.z) ? 0.0 : 1.0;
    float variance = moments.y - (moments.x * moments.x);
    variance = max(variance, _minVariance);
    float d = t - moments.x;
    float p_max = variance / (variance + d * d);
    //p_max = linstep(.001, 1., p_max);
    return max(p, p_max);
}


vec2 ComputeMoments(float Depth) {
    vec2 Moments;
    // First moment is the depth itself.
    Moments.x = Depth;
    // Compute partial derivatives of depth.
    float dx = dFdx(Depth);
    float dy = dFdy(Depth);
    // Compute second moment over the pixel extents.
    Moments.y = Depth*Depth + 0.1*(dx*dx + dy*dy);
    return Moments;
}


float ShadowContribution(float distanceToLight, vec4 lightTextureCoord) {
    vec2 moments = texture2D(iShadowMap, lightTextureCoord.st).xy;
    //vec2 moments = ComputeMoments(texture2D(iDepthTexture, textureCoord).x);// * 2. - 1.);
    
    return ChebyshevUpperBound(moments, distanceToLight, lightTextureCoord);
    /*
    if (distanceToLight < (1.1 * lightTextureCoord.z)) {
        return 0.5;
    } else {
        return 1.;
    }*/
}


void main() {
    float viewDistance = texture2D(iDepthTexture, textureCoord).x;
    vec4 realPosition = texture2D(iPositionTexture, textureCoord) * 2. - 1.;
    
    
    vec4 depth = texture2D(iDepthTexture, textureCoord);
    
    vec4 shadowCoord = lightProjectionMatrix * lightModelViewMatrix * realPosition ;
    
    vec4 color = texture2D(iColorTexture, textureCoord);
    
    vec4 shadowCoordinateWdivide = (shadowCoord / shadowCoord.w) * .5 + .5;
    
    shadowCoordinateWdivide.z -= 0.005;
    
    float distanceFromLight = texture2D(iShadowMap,shadowCoordinateWdivide.st).z * .5 + .5;
    
//    float shadow = ShadowContribution(shadowCoordinateWdivide.st, shadowCoordinateWdivide.z);
    ///*
    float shadow = ShadowContribution(distanceFromLight, shadowCoordinateWdivide);
    
    shadow =  linstep(.3, 1., shadow);

//    gl_FragColor = shadow * color;
    gl_FragColor = vec4(vec3(shadow), 1.);
}