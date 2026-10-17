#version 330
//in vec4 colorVarying;
//in vec2 texture_coordinate;
uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

in float logz;
in float Fcoef_half;

void main() {
    vec4 textureColor = texture2D(iTexture0, texCoord);
    gl_FragColor = vec4(1., 1., 1., textureColor.a); //textureColor.a);
}