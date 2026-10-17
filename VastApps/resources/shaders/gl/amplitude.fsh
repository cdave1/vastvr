#version 330

uniform vec3      iResolution;           // viewport resolution (in pixels)
//uniform float     iChannelTime[4];       // channel playback time (in seconds)
//uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
//uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
//uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
//uniform vec4      iDate;                 // (year, month, day, time in seconds)

uniform float iGlobalTime;
uniform int uAmplitudeSize;
uniform float uAmplitudeSet[512];

in vec2 textureCoord;
in vec4 colorVarying;
in vec3 normalVarying;

void main() {
    vec2 loc = vec2(textureCoord[0], textureCoord[1]) * 0.5 + 0.5;
    
    int xPos = int(loc.x * float(min(512., float(uAmplitudeSize))));
    /*
    if (xPos > 100) {//uAmplitudeSet[xPos] <= loc.y) {
        gl_FragData[0] = vec4(uAmplitudeSet[xPos]);
    } else {
        gl_FragData[0] = vec4(loc.x * 0.25, loc.x * 0.5, loc.y * 0.75, 1.0);
    }*/
    gl_FragData[0] = vec4(uAmplitudeSet[xPos]);
    gl_FragData[1] = vec4(0.0);
}
