attribute vec4 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec4 color;

varying vec3 positionVarying;
varying vec4 colorVarying;
varying vec3 normalVarying;
varying vec2 textureCoord;

void main() {
    normalVarying = normalize(gl_NormalMatrix * normal);
    textureCoord = texCoord;
    colorVarying = color;
    positionVarying = vec3(gl_ModelViewMatrix * position);
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * position);
}
