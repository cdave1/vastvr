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
	m_nullTexture = this->LoadTexture("***r_notexture***", data, 16, 16, TextureType::Default, 4);
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
	if (location >= m_textures->GetSize()) {
		return NULL;
	}
	return (Texture *)m_textures->GetItems()[location];
}


Texture * hd::TextureManager::FindTexture(const std::string &name, TextureType type) {
	Texture	*tex;
	unsigned char	*data;			/* raw texture data */
	unsigned short	width, height;	/* width, height of texture */
	unsigned short	bytes;
	
	if (name.empty()) {
		return NULL;
	}
	
	for (unsigned i = 0; i < m_textures->Size(); i++) {
        Texture *t = m_textures->At(i);
		if (t->name.compare(name) == 0) {
			return t;
		}
	}
	
	hd::log::i("[TextureManager] Looking for texture file: %s\n", name.c_str());
	
	if (name.find(".tga") != std::string::npos) {
		LoadTGA(name.c_str(), &data, &width, &height, &bytes);
        
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
		LoadPNG(name.c_str(), &data, &width, &height, &bytes);
		if (data) {
			tex = this->LoadTexture(name, data, width, height, type, bytes);
			hdFreeSafely(data);
			return tex;
		}
	}
    
	hd::log::i("[TextureManager] ERROR: could not find texture file: %s\n", name.c_str());
	return m_nullTexture;
}


Texture* hd::TextureManager::ReloadTexture(Texture *texture, TextureType type) {
	// There's no point resampling if nothing is going to change.
	if (texture->type != type) {
		char fname[kMaxTexturePathSize];
		snprintf(fname, kMaxTexturePathSize, "%s", texture->name.c_str());
		DestroyTexture(texture);
		return FindTexture(fname, type);
	} else {
		return texture;
	}
}


Texture* hd::TextureManager::LoadTexture(const std::string &name, unsigned char* data,
                                         unsigned short width, unsigned short height,
                                         TextureType type, unsigned short bytes) {
	Texture	*tex;
	unsigned char *scaled;
	unsigned short scaled_width, scaled_height, downSizedWidth, downSizedHeight;
	
	tex = new Texture(name);
	glGenTextures(1, &tex->texnum);
	m_textures->Add(tex);

	tex->width = width;
	tex->height = height;
	tex->type = type;
	tex->bytes = bytes;
	tex->isPremultipliedAlpha = false;
	tex->name = name;
	
	switch(type) {
		case TextureType::Default:
			tex->MipMap = true;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapNearest;
			tex->MagFilter = TextureMagFilter::Linear;
			break;
		case TextureType::Sprite:
			tex->MipMap = false;
			tex->WrapS = TextureWrapMode::Clamp;
			tex->WrapT = TextureWrapMode::Clamp;
			tex->MinFilter = TextureMinFilter::LinearMipMapLinear;
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
		case TextureType::DepthBufferMultisample:
        case TextureType::DepthBuffer16:
        case TextureType::DepthStencilBuffer:
        case TextureType::DataBuffer:
            tex->MipMap = false;
            tex->WrapS = TextureWrapMode::Clamp;
            tex->WrapT = TextureWrapMode::Clamp;
            tex->MinFilter = TextureMinFilter::NearestMipMapOff;
            tex->MagFilter = TextureMagFilter::Nearest;
            break;
		default:
			tex->MipMap = true;
			tex->WrapS = TextureWrapMode::Repeat;
			tex->WrapT = TextureWrapMode::Repeat;
			tex->MinFilter = TextureMinFilter::LinearMipMapLinear;
			tex->MagFilter = TextureMagFilter::Nearest;
			break;
	}

    scaled_width = tex->width;
    scaled_height = tex->height;
	
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
			hd::log::i("Could not downsize texture: %s\n", name.c_str());
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
			internalFormat = GL_ALPHA;
			externalFormat = GL_ALPHA;
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
        internalFormat = GL_DEPTH_COMPONENT32F;
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
        internalFormat = GL_DEPTH_COMPONENT32F;
        externalFormat = GL_DEPTH_COMPONENT;
        pixelDataType = GL_FLOAT;
        hdFreeSafely(scaled);
        scaled = NULL;

    } else if (tex->type == TextureType::DataBuffer) {
        internalFormat = GL_RGBA;
        externalFormat = GL_RGBA;
        pixelDataType = GL_FLOAT;

        hdFreeSafely(scaled);
        scaled = NULL;
	} else if (tex->type == TextureType::RenderTargetMultisample) {
		internalFormat = GL_RGBA8;
		target = GL_TEXTURE_2D_MULTISAMPLE;
	}
	else if (tex->type == TextureType::DepthBufferMultisample) {
		internalFormat = GL_DEPTH_COMPONENT32F;
		target = GL_TEXTURE_2D_MULTISAMPLE;
	}
	glBindTexture(target, tex->texnum);
	
    if (target == GL_TEXTURE_2D_MULTISAMPLE) {
        //target = GL_TEXTURE_2D_MULTISAMPLE;
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 2, internalFormat, scaled_width, scaled_height, true);

	} else {
		glTexImage2D(target, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled);

	}

	glTexParameteri(target, GL_TEXTURE_WRAP_S, hd::gl::WrapToGL(tex->WrapS));
	glTexParameteri(target, GL_TEXTURE_WRAP_T, hd::gl::WrapToGL(tex->WrapT));
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, hd::gl::MinFilterToGL(tex->MipMap, tex->MinFilter));
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, hd::gl::MagFilterToGL(tex->MagFilter));

	if (tex->MipMap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	hdFreeSafely(scaled);
    
	hd::log::i("[TextureManager] Finished Loading texture: %s\n\n", tex->name.c_str());
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

Texture * hd::TextureManager::GenerateDepthBufferMultisample(const Size2 &size) {
	static char name[256];
	snprintf(name, sizeof(name), "DepthBufferMultisample%d", m_textures->Size());
	unsigned int BYTES_PER_PIXEL = 4;
	
	unsigned char *data = (unsigned char *)calloc(1, sizeof(unsigned char) * BYTES_PER_PIXEL * size[0] * size[1]);
	if (!data) {
		return NULL;
	}

	Texture *texture = LoadTexture(name, data, size[0], size[1], TextureType::DepthBufferMultisample, BYTES_PER_PIXEL);
	free(data);
	return texture;
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
	tex->type = TextureType::Default;
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
        hd::log::i("Remove texture: %s\n", texture->name.c_str());
		m_textures->Remove(texture);
        DeleteSafely(texture);
		//delete texture;
		//texture = NULL;
	}
}
