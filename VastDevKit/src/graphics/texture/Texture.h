#ifndef _VAST_TEXTURE_H_
#define _VAST_TEXTURE_H_

#define		kMaxTextures	1024
#define kMaxTexturePathSize 256

namespace hd {
    enum class TextureType {
        Default,
		Sprite,
        RenderTarget,
        RenderTargetMultisample,
        DataBuffer, /* 32 bit signed integer */
        DepthBuffer,
		DepthBufferMultisample,
        DepthBuffer16,
        DepthStencilBuffer,
        GeometryBuffer,

        Wall /*obsolete!*/
    };


    enum class TextureWrapMode {
        Repeat = 0,
        Clamp,
        ClampToBorder,
    };


    enum class TextureDataFormat {
        Auto = 0,
        RGBA8888,
        RGBA4444,
        RGBA5551,
        RGB888,
        RGB565,
        DXTC1,
        DXTC1Alpha,
        DXTC3,
        DXTC5,
        Luminance,
        Alpha,
        AlphaLuminance,
        DsDt
    };


    enum class TextureMagFilter
    {
        Nearest = 0,
        Linear
    };

    
    enum class TextureMinFilter
    {
        NearestMipMapOff = 0,
        NearestMipMapNearest,
        NearestMipMapLinear,
        LinearMipMapOff,
        LinearMipMapNearest,
        LinearMipMapLinear
    };


    class Texture {
	public:
		bool		MipMap = false;
		bool		isTextureCube = false;
		bool		isPremultipliedAlpha = false;
		TextureDataFormat		UploadFormat = TextureDataFormat::Auto;
		TextureWrapMode	WrapS = TextureWrapMode::Repeat;
		TextureWrapMode	WrapT = TextureWrapMode::Repeat;
		TextureWrapMode	WrapR = TextureWrapMode::Repeat;
		TextureMagFilter		MagFilter = TextureMagFilter::Linear;
		TextureMinFilter		MinFilter = TextureMinFilter::NearestMipMapOff;
		unsigned int		registration_sequence = 0;		// 0 = free
		unsigned short width = 0;
		unsigned short height = 0;
		unsigned short upload_width = 0;
		unsigned short upload_height = 0;
		unsigned int texnum = 0;
		unsigned short bytes = 0;
		unsigned short bpp = 32;
		TextureType type = TextureType::Default;
		std::string name;

    public:
        Texture(const std::string name) : name(name) {}
        
        ~Texture() {}
        
    };
}


#endif