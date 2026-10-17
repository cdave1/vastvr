#version 330

uniform float iGlobalTime;
uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

uniform vec4 ColorTint;

out vec4 FragmentColor;
//out vec4 FragmentDepth;

void main() {
    FragmentColor = ColorTint * texture(iTexture0, texCoord);
    //gl_FragDepth = position.z; //vec4(0.0);
}