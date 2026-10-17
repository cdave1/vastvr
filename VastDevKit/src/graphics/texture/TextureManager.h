#ifndef _VAST_TEXTURE_MANAGER_H_
#define _VAST_TEXTURE_MANAGER_H_

#include <Vast/VastCore.h>
#include <Vast/VastMath.h>
#include "graphics/Util/hdNoise.h"

namespace hd {
    class Texture;
    
    class TextureManager {
        
    public:
        
        static TextureManager * Instance();
        
        static void TearDown();
        
        Texture * FindTexture(const std::string &name, TextureType type = TextureType::Default);
        
        Texture * ReloadTexture(Texture *texture, TextureType type);
        
        Texture * LoadTexture(const std::string &name, unsigned char *data, unsigned short width, unsigned short height, TextureType type, unsigned short bytes);
        
        Texture * GenerateRenderTexture(const Size2 &size);

        Texture * GenerateRenderTextureMultisample(const Size2 &size);
        
        Texture * GenerateGeometryBuffer(const Size2 &size);
        
        Texture * GenerateDepthBuffer(const Size2 &size2, const hd::TextureType textureType = hd::TextureType::DepthBuffer);

		Texture * GenerateDepthBufferMultisample(const Size2 &size2);

        Texture * GenerateDataBuffer(const Size2 &size2);
        
        Texture * GenerateBuffer(const Size2 &size, const hd::TextureType textureType);
        
        Texture * GenerateDepthStencil(const Size2 &size);
        
        Texture * GenerateNoiseTexture(int noiseType, float z);
        
        Texture * GetTexture(const unsigned int location);
        
        Texture * GetNullTexture() {
            return m_nullTexture;
        }
        
        void DestroyTexture(Texture *texture);
        
        void DestroyTextures();
        
        
    protected:
        
        TextureManager();
        
        ~TextureManager();
        
        
    private:
        
        hd::Vector<Texture *> *m_textures;
        
        Texture *m_nullTexture;
        
        int DownSamplePixels(unsigned short *dest, unsigned char *src,
                             unsigned short width, unsigned short height,
                             unsigned short srcBytes, GLenum destFormat);
        
        int DownSizePixels(unsigned char*& dest, unsigned char *src, unsigned short srcBPP,
                           unsigned short srcWidth, unsigned short srcHeight, 
                           unsigned short& destWidth, unsigned short& destHeight, 
                           GLenum format);
        
        int m_maxTextureSize;
    };
}

#endif