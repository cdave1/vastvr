/*
 *  HDIBImage.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef HD_IB_IMAGE_H_
#define HD_IB_IMAGE_H_

#include "hdIBProxy.h"

class hdIBImage : public hdIBProxy
{
public:
	hdIBImage();
	
	hdIBImage(const char *texture, 
			  const float x, const float y, 
			  const float w, const float h);
	
	hdIBImage(const char *texture, 
			  const hdVec3 &lower, const hdVec3 &upper);
	
	~hdIBImage();
	
	hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();
	
	hdUIReceiver * CreateInterfaceElement();
	
	void Draw() const;
	
	char * GetTexture();
	
	void SetTexture(const char *texture);
	
	const hdColor4f GetTint() const;
	
	void SetTint(const hdColor4f& tint);

private:
	char m_texture[kMaxTexturePathSize + 16];
	friend class boost::serialization::access;
	hdColor4f m_tint;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdIBProxy>(*this);
		ar & m_texture;
	}
};

inline hdInterfaceBuilderProxyType_t hdIBImage::GetInterfaceBuilderProxyType() 
{
	return hdInterfaceBuilderProxyTypeImage; 
}


inline char * hdIBImage::GetTexture()
{
	return m_texture;
}


inline void hdIBImage::SetTexture(const char *texture)
{
	snprintf(m_texture, kMaxTexturePathSize, "%s", texture);
}


inline const hdColor4f hdIBImage::GetTint() const
{
	return m_tint;
}

#endif