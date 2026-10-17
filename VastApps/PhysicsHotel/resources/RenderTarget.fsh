uniform float iGlobalTime;
uniform sampler2D iTexture0;

varying vec2 textureCoord;
varying vec4 colorVarying;

void main() {
    gl_FragColor = colorVarying * texture2D(iTexture0, textureCoord);
}