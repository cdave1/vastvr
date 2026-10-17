#include "ExtrusionRenderer.h"
#include "graphics/components/Polygon.h"
#include "graphics/components/Material.h"
#include "graphics/components/ExtrusionData.h"
#include "graphics/GL/GraphicsInterface.h"

using namespace hd::graphics;

ExtrusionRenderer::ExtrusionRenderer(unsigned int ID) : Component(ID) {}


#define FLAT_FRONT_VERTEX_SHADE 0.85f
#define FLAT_BACK_VERTEX_SHADE 0.55f

void ExtrusionRenderer::RenderFlatQuad(const Polygon *polygon,
                                       const ExtrusionData *extrusionData,
                                       const Material *material) {
    const Vector<Vertex> *vertices = polygon->GetVertices();
    const Vector<Vec2>   *textureCoords = extrusionData->GetTextureCoords();
    
	Color4ub frontColor, backColor;
    float depth = extrusionData->GetDepth();
	
	frontColor.Setf(material->GetAmbient()[0] * FLAT_FRONT_VERTEX_SHADE,
					material->GetAmbient()[1] * FLAT_FRONT_VERTEX_SHADE,
					material->GetAmbient()[2] * FLAT_FRONT_VERTEX_SHADE,
					material->GetAmbient()[3]);
	
	backColor.Setf(material->GetAmbient()[0] * FLAT_BACK_VERTEX_SHADE,
				   material->GetAmbient()[1] * FLAT_BACK_VERTEX_SHADE,
				   material->GetAmbient()[2] * FLAT_BACK_VERTEX_SHADE,
				   material->GetAmbient()[3]);
    
    if (material->GetTexture()) {
        hd::gl::BindTexture(material->GetTexture());
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        hd::gl::BindTexture(NULL);
    }
	
    hd::gl::Begin(GL_QUADS);
	for (int i = 0; i < vertices->Size(); ++i) {
		int i1 = i;
		int i2 = ((i+1) >= vertices->Size()) ? 0 : (i+1);
        int stride = i * 4;
        
        Vertex v1 = vertices->At(i1);
        Vertex v2 = vertices->At(i2);
        
        if (material->GetTexture()) {
            Vec2 texCoord = textureCoords->At(0 + stride);
            hd::gl::TexCoord2f(texCoord);
        }
        
        hd::gl::Color4ub(frontColor.r, frontColor.g, frontColor.b, frontColor.a);
        hd::gl::Vertex3f(v1.origin[0], v1.origin[1], v1.origin[2] - depth);
        
        if (material->GetTexture()) {
            Vec2 texCoord = textureCoords->At(1 + stride);
            hd::gl::TexCoord2f(texCoord[0], texCoord[1]);
        }
        
        hd::gl::Color4ub(backColor.r, backColor.g, backColor.b, backColor.a);
        hd::gl::Vertex3f(v1.origin[0], v1.origin[1], v1.origin[2] + depth);
        
        if (material->GetTexture()) {
            Vec2 texCoord = textureCoords->At(2 + stride);
            hd::gl::TexCoord2f(texCoord[0], texCoord[1]);
        }
        
        hd::gl::Color4ub(backColor.r, backColor.g, backColor.b, backColor.a);
        hd::gl::Vertex3f(v2.origin[0], v2.origin[1], v2.origin[2] + depth);
        
        if (material->GetTexture()) {
            Vec2 texCoord = textureCoords->At(3 + stride);
            hd::gl::TexCoord2f(texCoord[0], texCoord[1]);
        }
        
        hd::gl::Color4ub(frontColor.r, frontColor.g, frontColor.b, frontColor.a);
        hd::gl::Vertex3f(v2.origin[0], v2.origin[1], v2.origin[2] - depth);
	}
    hd::gl::End();
}

/*

static GLint envMode, combineRGB, src0RGB, src1RGB;
void StartTextureEffect(uint8 effect)
{
	glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &envMode);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_COMBINE_RGB, &combineRGB);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC0_RGB, &src0RGB);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC1_RGB, &src1RGB);
	
	if (effect == HDGL_EFFECT_DOUBLE_INTENSITY)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0f);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
	}
}


void StopTextureEffect(uint8 effect)
{
	if (effect == HDGL_EFFECT_DOUBLE_INTENSITY)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envMode);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, combineRGB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, src0RGB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, src1RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
	}
}*/


#define FRONT_VERTEX_SHADE 0.85f
#define BACK_VERTEX_SHADE 0.55f
#define SHADING_MODIFIER 0.5f
/*
 * This is a very crappy per vertex shader.
 *
 * Code below takes a normal for each vertex pair
 * (perp. to line made by pair) and then calculates the light
 * for the quad as a modification to the tint. Light is
 * an ambient value pointing along the negative y axis.
 *
 * TODO: Variable light angle.
 *
 * Funky normal smoothing by taking the
 * average normal of adjacent vertex pairs and using a
 * texture combine at 2x the rgb scale so brights look
 * brighter and darks look darker.
 */
/*
void DrawExtrusionLighting(const Polygon *polygon,
                           const ExtrusionData *extrusionData,
                           const Material *material)
{
	unsigned i;
	short i1, i2, i3, i4, stride;
	
	hdMatrix inColors, normals, outColors;
	float fAlpha;
	float lightNormal;
	float prevNormal, currNormal, nextNormal;
	float norm0, norm1;
	
	unsigned char precalcAlpha;
	
	lightNormal = 0.0f;
	
	prevNormal = Get2DNormal(vertices[vertexCount - 1], vertices[0]);
	currNormal = Get2DNormal(vertices[0], vertices[1]);
	nextNormal = Get2DNormal(vertices[1], vertices[2]);
	
	float firstVertexNormal, secondVertexNormal;
	
	// Precalcs
	precalcAlpha = hd::clamp((int)(255 * alpha), 0, (int)(255 * color.a));
	fAlpha = hd::clamp(alpha, 0.0f, color.a);
	
	MatrixIdentity(inColors);
	MatrixIdentity(normals);
	MatrixIdentity(outColors);
    
	// HACK HACK HACK
	//
	// Retrofitted code: previously, color information was passed in as a 4-array. We then used
	// index hacks to get the right values. Here we convert the new struct to an array to do
	// the same thing.
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1) && defined(__ARM_NEON__)
	for(i=0; i < 16; ++i)
	{
		inColors.f[i] = ((float *)&color)[i >> 2];
	}
#else
	for(i=0; i < 16; ++i)
	{
		inColors.f[i] = ((float *)&color)[i & 3];
	}
#endif
	
	for (int i = 0; i < vertexCount; ++i)
	{
		i1 = i;
		i2 = ((i+1) >= vertexCount) ? (i+1)-vertexCount : (i+1); // shitty modulus
		i3 = ((i+2) >= vertexCount) ? (i+2)-vertexCount : (i+2);
		i4 = ((i+3) >= vertexCount) ? (i+3)-vertexCount : (i+3);
		stride = i * 4;
        
		hdVec3 normal = vertices[i2] - vertices[i1];
		//int cross = hdCrossProductSign(screenCenter, vertices[i1], (vertices[i1]-normal));
		
		//if ((cross <= 0) || (reflected && cross > 0))
		{
			// no smoothing for objects with sharp angles
			// TODO: use angle beween i1, i2, i3 for this.
			if (vertexCount < 6)
			{
				lightNormal = hd::clamp((1.0f + currNormal) * SHADING_MODIFIER, 0.25f, 1.0f);
				norm0 = FRONT_VERTEX_SHADE * lightNormal;
				norm1 = BACK_VERTEX_SHADE * lightNormal;
				
				normals.f[__11] = norm0;
				normals.f[__22] = norm1;
				normals.f[__33] = norm1;
				normals.f[__44] = norm0;
				
				hdMatrixMultiply(outColors, inColors, normals);
			}
			else
			{
				firstVertexNormal = hd::clamp((1.0f + (prevNormal + currNormal) * 0.5f) * SHADING_MODIFIER, 0.2f, 1.0f);
				secondVertexNormal = hd::clamp((1.0f + (currNormal + nextNormal) * 0.5f) * SHADING_MODIFIER, 0.2f, 1.0f);
				
				normals.f[__11] = FRONT_VERTEX_SHADE * firstVertexNormal;
				normals.f[__22] = BACK_VERTEX_SHADE * firstVertexNormal;
				normals.f[__33] = BACK_VERTEX_SHADE * secondVertexNormal;
				normals.f[__44] = FRONT_VERTEX_SHADE * secondVertexNormal;
				
				hdMatrixMultiply(outColors, inColors, normals);
			}
            
			
			
			
			
			//BIG HACK
			// HACK HACK HACK
			// Vals come out of NEON in the wrong order.
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1) && defined(__ARM_NEON__)
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[0 + stride][0],
							   textureCoordinates[0 + stride][1]);
			}
			
			hd::gl::Color4f(outColors.f[__11], outColors.f[__12], outColors.f[__13], fAlpha);
			hd::gl::Vertex3f(vertices[i1][0], vertices[i1][1], vertices[i1][2] - depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[1 + stride][0],
							   textureCoordinates[1 + stride][1]);
			}
			
			hd::gl::Color4f(outColors.f[__21], outColors.f[__22], outColors.f[__23], fAlpha);
			
			hd::gl::Vertex3f(vertices[i1][0], vertices[i1][1], vertices[i1][2] + depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[2 + stride][0],
							   textureCoordinates[2 + stride][1]);
			}
			
			
			hd::gl::Color4f(outColors.f[__31], outColors.f[__32], outColors.f[__33], fAlpha);
			hd::gl::Vertex3f(vertices[i2][0], vertices[i2][1], vertices[i2][2] + depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[3 + stride][0],
							   textureCoordinates[3 + stride][1]);
			}
			hd::gl::Color4f(outColors.f[__41], outColors.f[__42], outColors.f[__43], fAlpha);
			
			hd::gl::Vertex3f(vertices[i2][0], vertices[i2][1], vertices[i2][2] - depth);
#else
            
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[0 + stride][0],
							   textureCoordinates[0 + stride][1]);
			}
			hd::gl::Color4f(outColors.f[__11], outColors.f[__21], outColors.f[__31], fAlpha);
			hd::gl::Vertex3f(vertices[i1][0], vertices[i1][1], vertices[i1][2] - depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[1 + stride][0],
							   textureCoordinates[1 + stride][1]);
			}
			hd::gl::Color4f(outColors.f[__12], outColors.f[__22], outColors.f[__32], fAlpha);
			hd::gl::Vertex3f(vertices[i1][0], vertices[i1][1], vertices[i1][2] + depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[2 + stride][0],
							   textureCoordinates[2 + stride][1]);
			}
			hd::gl::Color4f(outColors.f[__13], outColors.f[__23], outColors.f[__33], fAlpha);
			hd::gl::Vertex3f(vertices[i2][0], vertices[i2][1], vertices[i2][2] + depth);
			
			if (texture != NULL)
			{
				hd::gl::TexCoord2f(textureCoordinates[3 + stride][0],
							   textureCoordinates[3 + stride][1]);
			}
			hd::gl::Color4f(outColors.f[__14], outColors.f[__24], outColors.f[__34], fAlpha);
			hd::gl::Vertex3f(vertices[i2][0], vertices[i2][1], vertices[i2][2] - depth);
#endif
			
		}
        
		
		prevNormal = currNormal;
		currNormal = nextNormal;
		nextNormal = Get2DNormal(vertices[i3], vertices[i4]);
	}
}


*/
