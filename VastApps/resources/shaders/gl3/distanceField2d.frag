#version 330

uniform sampler2D iTexture0;

in vec4 position;
in vec3 normal;
in vec2 texCoord;
in vec4 color;

in vec3 viewSpacePosition;

//
// Distance field generation idea for 2D glyph generation.
//
// This is probably best done on the CPU.  With full access to edge data for each glyph, generate a probable distance
// field for each pixel in an XY texture.  Copy that data to a final atlas.  We'll need to record this information to
// retain information about each glyph for post distance field rendering.
//
// For 3D distance fields, in real time.  There are several problems to solve:
// - Continuous vs. discrete world space positions?
// - For a position in wo

float segmentDistance(in vec2 uv, in vec2 p1, in vec2 p2) {
    vec2 line = p2 - p1;

    float u = ((uv.x - p1.x) * line.x + (uv.y - p1.y) * line.y) / (line.x * line.x + line.y * line.y);
    u = clamp(u, 0., 1.);

    float x = (p1.x + u * line.x) - uv.x;
    float y = (p1.y + u * line.y) - uv.y;

    return sqrt(x * x + y * y) * (10. + (sin(iGlobalTime) * 0.5 + 0.5) * 20.);
}


void main() {
    //vec4 color = texture2D(iTexture0, texture_coordinate);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); //colorVarying.r, colorVarying.g, colorVarying.b, colorVarying.a);

    float a = texture(iTexture0, texCoord).a;

    gl_FragData[0] = vec4(1.0, 1.0, 1.0, a);
    gl_FragData[1] = vec4(0.); //0.5 * vec4(vec3(1., 0., 0.), 1.) + 0.5;

    //float depth = length(viewSpacePosition) / (60000.0);
    //gl_FragDepth = depth;
}
