#include "TextureManager.h"
#include "graphics/Texture/PNGLoader.h"
#include "graphics/Texture/TGALoader.h"

#include <string>

using namespace hd;

static hd::TextureManager* m_TextureManagerInstance = NULL;

static std::string m_textureQualityLevel;

hd::TextureManager * hd::TextureManager::Instance() {
	if (m_TextureManagerInstance == NULL) {
		m_TextureManagerInstance = new TextureManager();
	}
	return m_TextureManagerInstance;
}


void hd::TextureManager::TearDown() {
	delete m_TextureManagerInstance;
	m_TextureManagerInstance = NULL;
}


hd::TextureManager::TextureManager() {
	m_maxTextureSize = hdglMaxTexSize;
	m_textures = new hd::Vector<Texture *>(kMaxTextures);
	
	unsigned char *ptr;
	unsigned char *data;
	int x, y;
	
	// create a checkerboard texture
    const unsigned int NULL_TEXTURE_SIZE = 32;
    const unsigned int NULL_TEXTURE_BPP = 4;
	data = (unsigned char*)calloc(1, NULL_TEXTURE_SIZE * NULL_TEXTURE_SIZE * NULL_TEXTURE_BPP);
	for (y = 0; y < NULL_TEXTURE_SIZE; ++y) {
		for (x = 0; x < NULL_TEXTURE_SIZE; ++x) {
			ptr = &data[(y * NULL_TEXTURE_SIZE + x) * NULL_TEXTURE_BPP];
			if ((y < (0.5f * NULL_TEXTURE_SIZE)) ^ (x < (0.5f * NULL_TEXTURE_SIZE))) {
				ptr[0] = ptr[1] = ptr[2] = 0x00;
			} else {
				ptr[0] = ptr[1] = ptr[2] = 0xFF;
			}
            ptr[3] = 0xFF;
		}
	}
	m_nullTexture = this->LoadTexture("***r_notexture***", data, 16, 16, TextureType::Pic, 4);
	free(data);
}


hd::TextureManager::~TextureManager() {
	Texture *t;
	
	while(!m_textures->IsEmpty()) {
		t = m_textures->Pop();
		glDeleteTextures(1, &t->texnum);
		hd::log::e("glDeleteTextures");
	}
	delete m_textures;
}


Texture * hd::TextureManager::GetTexture(const unsigned int location) {
	if (location >= m_textures->GetSize()) return NULL;
	return (Texture *)m_textures->GetItems()[location];
}


Texture * hd::TextureManager::FindTexture(const char* name, TextureType type) {
	Texture	*tex;
	unsigned char	*data;			/* raw texture data */
	unsigned short	width, height;	/* width, height of texture */
	unsigned short	bytes;
	
	if (name == NULL) return NULL;
	if (strlen(name) == 0) return NULL;
	
	for (unsigned i = 0; i < m_textures->Size(); i++) {
        Texture *tex = m_textures->At(i);
		if (strncmp(tex->name, name, kMaxTexturePathSize) == 0) {
			return tex;
		}
	}
	
	hd::log::i("[TextureManager] Looking for texture file: %s\n", name);
	
	if (std::string(name).find(".tga") != std::string::npos) {
		LoadTGA(name, &data, &width, &height, &bytes);
        
        // Flip pixels around horizontal axis.
        unsigned char *tmp = (unsigned char *)calloc(1, bytes * width * height);
        
        for (int y = 0; y < height; ++y) {
            memcpy(tmp + (width * bytes * (height - y - 1)), data + (width * bytes * y), bytes * width);
        }
        hdFreeSafely(data)
        data = tmp;
        
		if (data) {
			tex = this->LoadTexture(name, data, width, height, type, bytes);
			hdFreeSafely(data);
			return tex;
		}
	}
	
	if (std::string(name).find(".png") != std::string::npos) {
		LoadPNG(name, &data, &width, &height, &bytes);
		if (data) {
			tex = this->LoadTexture(name, data, width, height, type, bytes);
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
			tex->isPremultipliedAlpha = true;
#endif
			hdFreeSafely(data);
			return tex;
		}
	}
    
	hd::log::i("[TextureManager] ERROR: could not find texture file: %s\n", name);
	return m_nullTexture;
}


Texture* hd::TextureManager::ReloadTexture(Texture *texture, TextureType type) {
	// There's no point resampling if nothing is going to change.
	if (texture->type != type) {
		char fname[kMaxTexturePathSize];
		snprintf(fname, kMaxTexturePathSize, "%s", texture->name);
		DestroyTexture(texture);
		return FindTexture(fname, type);
	}
	else
	{
		return texture;
	}
}


Texture* hd::TextureManager::LoadTexture(const char* name, unsigned char* data,
                                         unsigned short width, unsigned short height,
                                         TextureType type, unsigned short bytes) {
	Texture	*tex;
	unsigned char *scaled;
	unsigned char *downSized;
	unsigned short scaled_width, scaled_height, downSizedWidth, downSizedHeight;
	unsigned short* shortScaled;
	bool downSample;
	
	tex = new Texture(name);
	
	m_textures->Add(tex);
	tex->texnum = m_textures->GetSize();

    if (hdglError("A")) {
        hd::log::e("A");
    }

    glEnable(GL_TEXTURE_2D);

    if (hdglError("B")) {
        hd::log::e("B");
    }

	glGenTextures(1, &tex->texnum);

    if (hdglError("C")) {
        hd::log::e("C");
    }

    glBindTexture(GL_TEXTURE_2D, tex->texnum);

    if (hdglError("D")) {
        hd::log::e("D");
    }

	tex->width = width;
	tex->height = height;
	tex->type = type;
	tex->bytes = bytes;
	tex->isPremultipliedAlpha = false;
	snprintf(tex->name, kMaxTexturePathSize, "%s", name);
//	tex->name = name;
	
	switch(type) {
		case TextureType::Default:
			tex->MipMap = false;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapNearest;
			tex->MagFilter = TextureMagFilter::Linear;
			break;
		case TextureType::Pic:
			tex->MipMap = false;	
			tex->WrapS = TextureWrapMode::Clamp;
			tex->WrapT = TextureWrapMode::Clamp;
			tex->MinFilter = TextureMinFilter::NearestMipMapOff;
			tex->MagFilter = TextureMagFilter::Nearest;
			break;
			
		case TextureType::Wall:
		case TextureType::Wall16:
			tex->MipMap = false;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapNearest;
			tex->MagFilter = TextureMagFilter::Linear;
			break;
		case TextureType::FastWall:
			tex->MipMap = true;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapOff;
			tex->MagFilter = TextureMagFilter::Nearest;
			break;
		case TextureType::Background:
			tex->MipMap = true; //false;	
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapLinear;
			tex->MagFilter = TextureMagFilter::Linear;
			break;	
		case TextureType::Background16:
			tex->bpp = 16;
			tex->MipMap = true;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapLinear;
			tex->MagFilter = TextureMagFilter::Linear;
			break;
		case TextureType::Sprite:
		case TextureType::Pic16:
			tex->bpp = 16;
			tex->MipMap = false;
			tex->WrapS = TextureWrapMode::Clamp;
			tex->WrapT = TextureWrapMode::Clamp;
			tex->MinFilter = TextureMinFilter::LinearMipMapOff;
			tex->MagFilter = TextureMagFilter::Linear;
			break;
        case TextureType::RenderTarget:
        case TextureType::RenderTargetMultisample:
			tex->MipMap = false;
			tex->WrapS = TextureWrapMode::ClampToBorder;
			tex->WrapT = TextureWrapMode::ClampToBorder;
			tex->MinFilter = TextureMinFilter::LinearMipMapOff;
			tex->MagFilter = TextureMagFilter::Linear;
            break;
        case TextureType::DepthBuffer:
        case TextureType::DepthBuffer16:
        case TextureType::DepthStencilBuffer:
        case TextureType::DataBuffer:
            tex->MipMap = false;
            tex->WrapS = TextureWrapMode::ClampToBorder;
            tex->WrapT = TextureWrapMode::ClampToBorder;
            tex->MinFilter = TextureMinFilter::LinearMipMapOff;
            tex->MagFilter = TextureMagFilter::Linear;
            break;
		default:
			tex->MipMap = true;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapOff;
			tex->MagFilter = TextureMagFilter::Nearest;
			break;
	}
	
#ifdef OPENGLES2
	for(scaled_width = 1 ; scaled_width < tex->width ; scaled_width <<= 1) {}
	
	if(scaled_width > tex->width && tex->MipMap) {
		scaled_width >>= 1;
	}
	
	for(scaled_height = 1 ; scaled_height < tex->height ; scaled_height <<= 1) {}
	
	if(scaled_height > tex->height && tex->MipMap) {
		scaled_height >>= 1;
	}
	
	if(scaled_width < 1) {
		scaled_width = 1;
	}
    
	if(scaled_height < 1) {
		scaled_height = 1;
	}
#else
    scaled_width = tex->width;
    scaled_height = tex->height;
#endif
	
	/**
	 * Copy the contents of "data" to our own variable.
	 */
	unsigned int initialDataSize = sizeof(unsigned char) * tex->bytes * tex->width * tex->height;
	if (!(scaled = (unsigned char*)calloc(1, initialDataSize))) {
		hd::log::i("OUT OF MEM\n");
		return m_nullTexture;
	}
	memcpy(scaled, data, initialDataSize);
	
	/**
	 * How many down size iterations? m_minTextureDownsizeIterations
	 */
	unsigned short tmp_scaled_width = scaled_width;
	unsigned short tmp_scaled_height = scaled_height;
	
	unsigned short downSizeIterations = 0; //m_minTextureDownsizeIterations;
	while(tmp_scaled_width > m_maxTextureSize ||
		  tmp_scaled_height > m_maxTextureSize) {
		++downSizeIterations;
		tmp_scaled_width = hd::clamp(tmp_scaled_width >> 1, 1, 1 << 16);
		tmp_scaled_height  = hd::clamp(tmp_scaled_height >> 1, 1, 1 << 16);
	}
	
	/**
	 * We want to downsize the pixels if the texture is bigger than the maximum allowed size.
	 */
	while(downSizeIterations-- > 0) {
		unsigned char *tmpDownSized = NULL;
		
		if (-1 == DownSizePixels(tmpDownSized, scaled, tex->bytes, scaled_width, scaled_height, downSizedWidth, downSizedHeight, 0)) {
			hd::log::i("Could not downsize texture: %s\n", name);
			hdFreeSafely(tmpDownSized);
            hdFreeSafely(scaled);
			return m_nullTexture ;
		}
		
		scaled_width = downSizedWidth;
		scaled_height = downSizedHeight;
		
		unsigned int downsizedDataSize = sizeof(unsigned char) * tex->bytes * scaled_width * scaled_height;
		
		free(scaled);
		if (!(scaled = (unsigned char*)calloc(1, downsizedDataSize))) {
			hd::log::i("OUT OF MEM\n");
			hdFreeSafely(tmpDownSized);
			hdFreeSafely(scaled);
			return m_nullTexture;
		}
		
		memcpy(scaled, tmpDownSized, downsizedDataSize);
		hdFreeSafely(tmpDownSized);
	}
	
	tex->upload_width = scaled_width;
	tex->upload_height = scaled_height;

    GLenum target = GL_TEXTURE_2D;
	GLenum pixelDataType = GL_UNSIGNED_BYTE;
	GLenum internalFormat = GL_RGBA;
	GLenum externalFormat = GL_RGBA;
 
	switch (tex->bytes) {
		case 1:
			internalFormat = GL_ALPHA;
			externalFormat = GL_ALPHA;
			break;
		case 2:
#ifdef GL_LUMINANCE_ALPHA
			internalFormat = GL_LUMINANCE_ALPHA;
			externalFormat = GL_LUMINANCE_ALPHA;
#else
            internalFormat = GL_RG;
            externalFormat = GL_RG;
#endif
			break;
		case 3:
			internalFormat = GL_RGB;
			externalFormat = GL_RGB;
			break;
		case 4:
			internalFormat = GL_RGBA;
			externalFormat = GL_RGBA;
			break;
		default:
			internalFormat = GL_RGB;
			externalFormat = GL_RGB;
			break;
	}
    
    if (tex->type == TextureType::DepthBuffer) {
        internalFormat = GL_DEPTH_COMPONENT;
        externalFormat = GL_DEPTH_COMPONENT;
        pixelDataType = GL_FLOAT;
        
        hdFreeSafely(scaled);
        scaled = NULL;
        
    } else if (tex->type == TextureType::DepthBuffer16) {
        internalFormat = GL_DEPTH_COMPONENT16;
        externalFormat = GL_DEPTH_COMPONENT;
        pixelDataType = GL_SHORT;
        
        hdFreeSafely(scaled);
        scaled = NULL;
        
    } else if (tex->type == TextureType::GeometryBuffer) {
        internalFormat = GL_RGBA;
        externalFormat = GL_RGBA;
        pixelDataType = GL_FLOAT;
        
        hdFreeSafely(scaled);
        scaled = NULL;
        
    } else if (tex->type == TextureType::DepthStencilBuffer) {
        internalFormat = GL_DEPTH24_STENCIL8;
        externalFormat = GL_DEPTH_STENCIL;
        pixelDataType = GL_UNSIGNED_INT_24_8;
    } else if (tex->type == TextureType::DataBuffer) {
        internalFormat = GL_RGBA;
        externalFormat = GL_RGBA;
        pixelDataType = GL_FLOAT;

        hdFreeSafely(scaled);
        scaled = NULL;
    }

    if (tex->type == TextureType::RenderTargetMultisample) {
        //target = GL_TEXTURE_2D_MULTISAMPLE;
    }

    if (hdglError("Before glTexImage2D")) {
        hd::log::e("Errorbreak");
    }
    
#if TARGET_GL_OPENGL
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled);
#else
	if (tex->MipMap) {
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    } else {
        glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled);
#endif
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hd::gl::WrapToGL(tex->WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, hd::gl::WrapToGL(tex->WrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, hd::gl::MinFilterToGL(tex->MipMap, tex->MinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, hd::gl::MagFilterToGL(tex->MagFilter));
	
	hdFreeSafely(scaled);
	
	hdglError("glTexImage2D");
	
#if TARGET_GL_OPENGL == 1
	if (tex->MipMap) {
	//	glGenerateMipmapEXT(GL_TEXTURE_2D);
	}
    
#else
	if (tex->MipMap) {
		glGenerateMipmapOES(GL_TEXTURE_2D);
	}
#endif
    
	hd::log::i("[TextureManager] Finished Loading texture: %s\n\n", tex->name);
	return tex;
}


Texture * hd::TextureManager::GenerateRenderTexture(const Size2 &size) {
    static char name[256];
    snprintf(name, sizeof(name), "RenderTexture%d", m_textures->Size());
    
    const unsigned int BYTES_PER_PIXEL = 4;
    unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
    if (!data) {
        return NULL;
    }
    
    Texture *texture = LoadTexture(name, data, size[0], size[1], TextureType::RenderTarget, BYTES_PER_PIXEL);
    free(data);
    return texture;
}


Texture * hd::TextureManager::GenerateRenderTextureMultisample(const Size2 &size) {
    static char name[256];
    snprintf(name, sizeof(name), "RenderTextureMultisample%d", m_textures->Size());

    const unsigned int BYTES_PER_PIXEL = 4;
    unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
    if (!data) {
        return NULL;
    }

    Texture *texture = LoadTexture(name, data, size[0], size[1], TextureType::RenderTargetMultisample, BYTES_PER_PIXEL);
    free(data);
    return texture;
}


Texture * hd::TextureManager::GenerateGeometryBuffer(const Size2 &size) {
    return GenerateBuffer(size, hd::TextureType::GeometryBuffer);
}


Texture * hd::TextureManager::GenerateDepthBuffer(const Size2 &size, const hd::TextureType textureType) {
    return GenerateBuffer(size, textureType);
}


Texture * hd::TextureManager::GenerateDataBuffer(const Size2 &size) {
    static char name[256];
    snprintf(name, sizeof(name), "DataBuffer%d", m_textures->Size());

    const unsigned int BYTES_PER_PIXEL = 4;
    unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
    if (!data) {
        return NULL;
    }

    Texture *texture = LoadTexture(name, data, size[0], size[1], TextureType::DataBuffer, BYTES_PER_PIXEL);
    free(data);
    return texture;
}


Texture * hd::TextureManager::GenerateBuffer(const Size2 &size, const hd::TextureType textureType) {
    static char name[256];
    snprintf(name, sizeof(name), "DepthBuffer%d", m_textures->Size());
    
    unsigned int BYTES_PER_PIXEL = 4;
    if (textureType == hd::TextureType::DepthBuffer16) {
        BYTES_PER_PIXEL = 2;
    } else if (textureType == hd::TextureType::GeometryBuffer) {
        BYTES_PER_PIXEL = sizeof(float) * 4;
    }
    
    unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
    if (!data) {
        return NULL;
    }
    
    Texture *texture = LoadTexture(name, data, size[0], size[1], textureType, BYTES_PER_PIXEL);
    free(data);
    return texture;
}


Texture * hd::TextureManager::GenerateDepthStencil(const Size2 &size) {
    static char name[256];
    snprintf(name, sizeof(name), "DepthStencil%d", m_textures->Size());
    
    const unsigned int BYTES_PER_PIXEL = 4;
    unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
    if (!data) {
        return NULL;
    }
    
    Texture *texture = LoadTexture(name, data, size[0], size[1], TextureType::DepthStencilBuffer, BYTES_PER_PIXEL);
    free(data);
    return texture;
}


Texture * hd::TextureManager::GenerateNoiseTexture(int noiseType, float z) {
    Texture *tex = new Texture("noise");
    
	m_textures->Add(tex);
	
    tex->texnum = m_textures->GetSize();
	
	glGenTextures(1, &tex->texnum);
	
	tex->width = 0;
	tex->height = 0;
	tex->type = TextureType::Background;
	tex->bytes = 0;
	tex->isPremultipliedAlpha = false;
    
    hdMakeNoiseTexture(1024, 1024, noiseType, z, tex);
    
    return tex;
}


int hd::TextureManager::DownSizePixels(unsigned char*& dest, unsigned char *src, unsigned short srcBPP, 
									 unsigned short srcWidth, unsigned short srcHeight, 
									 unsigned short& destWidth, unsigned short& destHeight, 
									 GLenum format) {
	//unsigned short destWidth, destHeight;
	unsigned col, row, byte;
	unsigned char *ppOdd, *ppEven;
	unsigned short pixelSum;
	size_t sz;
	
	if (srcWidth <= 1 || srcHeight <= 1) {
		hd::log::i("hd::TextureManager::DownSizePixels: Can't downsize any further.");
		return -1;		
	}
	
	sz = sizeof(unsigned char);
	
	destWidth = srcWidth >> 1;
	destHeight = srcHeight >> 1;
	
	if (!(dest = (unsigned char*)calloc(1, sz * srcBPP * destWidth * destHeight))) {
		hd::log::i("hd::TextureManager::DownSizePixels OUT OF MEMORY.");
		return -1;
	}
	
	//memset(dest, 0, sz * srcBPP * destWidth * destHeight);
	
	ppOdd = src;
	ppEven = src + (sz * srcWidth * srcBPP);
	
	row = 0;
	while(row++ < destHeight) {
		col = 0;
		while (col++ < destWidth) {
			byte = 0;
			
			// Condense a 2x2 pixel square in source into 
			// a single pixel on dest. We need to do this
			// for each byte of the pixels, according to
			// the number of bytes per pixel. 
			// We don't care
			// too much about the format; this will work
			// whether the bytes are alpha, luminosity, RGB
			// RGBA, ABGR etc etc.
			while(byte++ < srcBPP) {
				pixelSum = (
							*ppOdd + 
							*(ppOdd + (sz * srcBPP)) +
							*ppEven +
							*(ppEven + (sz * srcBPP))
							);
				*dest = (unsigned char)(pixelSum >> 2); // divide by 4
				
				dest++;
				ppOdd++;
				ppEven++;
			}
			ppOdd = ppOdd + (sz * srcBPP);
			ppEven = ppEven + (sz * srcBPP);
		}
		ppOdd = ppOdd + (sz * srcWidth * srcBPP);
		ppEven = ppEven + (sz * srcWidth * srcBPP);
	}
	
	dest -= (sz * srcBPP * destWidth * destHeight);
	
	return 0;
}


/*
 * Translate pixels from separate bytes to a single short.
 *
 * We have pixels stored in 4 adjacent bytes, in RGBA configuration.
 *
 * Need to combine these bytes into a single short,
 * preserving the position of each color.
 */
int hd::TextureManager::DownSamplePixels(unsigned short *dest, unsigned char *src, 
									   unsigned short width, unsigned short height, 
									   unsigned short srcBytes, GLenum destFormat) {
	int pixels, i, j;
	
	pixels = (width * height);
	j = 0;
	
	if (srcBytes == 4 && destFormat == GL_UNSIGNED_SHORT_4_4_4_4) {
		// Format: RRRRGGGGBBBBAAAA
		for (i = 0; i < pixels; ++i) {
			j = i << 2;
			dest[i] =  (src[j] & 0xF0);
			dest[i] |= (src[j + 1] >> 4);
			dest[i] =  dest[i] << 8;
			dest[i] |= (src[j + 2] & 0xF0);
			dest[i] |= (src[j + 3] >> 4);
		}
		return 0;
	} else if (srcBytes == 4 && destFormat == GL_UNSIGNED_SHORT_5_5_5_1) {
		// Format: RRRRRGGGGGBBBBBA
		for (i = 0; i < pixels; ++i) {
			j = i << 2;
			dest[i] =  (src[j] >> 3);
			dest[i] =  dest[i] << 5;
			dest[i] |= (src[j + 1] >> 3);
			dest[i] =  dest[i] << 5;
			dest[i] |= (src[j + 2] >> 3);
			dest[i] =  dest[i] << 1;
			dest[i] |= (src[j + 3] >> 7);
		}
		return 0;
	} else if (srcBytes == 3 && destFormat == GL_UNSIGNED_SHORT_5_6_5) {
		// Format: RRRRRGGGGGGBBBBB
		for (i = 0; i < pixels; ++i) {
			j = i * 3;
			dest[i] =  (src[j] >> 3);
			dest[i] =  dest[i] << 6;
			dest[i] |= (src[j + 1] >> 2);
			dest[i] =  dest[i] << 5;
			dest[i] |= (src[j + 2] >> 3);
		}
		return 0;
	} else {
		return -1;
	}
}


void hd::TextureManager::DestroyTexture(Texture *texture) {
	if (texture != NULL) {
		glDeleteTextures(1, &texture->texnum);
		hdglError("glDeleteTextures");
		m_textures->Remove(texture);
        DeleteSafely(texture);
		//delete texture;
		//texture = NULL;
	}
}