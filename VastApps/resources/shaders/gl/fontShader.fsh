#version 330

uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

in vec3 cameraWorldPosition;


void main() {
    //vec4 color = texture2D(iTexture0, texture_coordinate);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); //colorVarying.r, colorVarying.g, colorVarying.b, colorVarying.a);

    float a = texture2D(iTexture0, texCoord).a;

    gl_FragData[0] = vec4(1.0, 1.0, 1.0, a);
    gl_FragData[1] = vec4(0.); //0.5 * vec4(vec3(1., 0., 0.), 1.) + 0.5;

    float depth = length(position.xyz - cameraWorldPosition) / (50000.0 * 2.0);
    gl_FragDepth = depth;
}
