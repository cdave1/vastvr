//
// Basic vertex shader for the render target.
//

attribute vec4 position;
attribute vec2 texCoord;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 textureCoord;

void main() {
    gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix * position);
    textureCoord = texCoord;
    colorVarying = color;
}
