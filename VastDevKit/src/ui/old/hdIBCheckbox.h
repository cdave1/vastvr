//
//  hdIBCheckbox.h
//  hdk.1.5.OSX
//
//  Copyright 2010 n/a. All rights reserved.
//

#ifndef HD_IB_CHECKBOX_H
#define HD_IB_CHECKBOX_H

#include "hdIBProxy.h"

/*
 * This is so lazy...
 */
typedef enum
{
	hdIBCheckboxTextureType_OnNormal = 0,
	hdIBCheckboxTextureType_OnOver,
	hdIBCheckboxTextureType_OffNormal,
	hdIBCheckboxTextureType_OffOver,
	hdIBCheckboxTextureType_Count,
} hdIBCheckboxTextureType_t;


class hdIBCheckbox : public hdIBProxy
{
public:
	hdIBCheckbox();
	
	hdIBCheckbox(const char *textureOnNormal, 
				 const char *textureOnOver, 
				 const char *textureOffNormal,
				 const char *textureOffOver,
				 const hdVec3 &lower, 
				 const hdVec3 &upper);
	
	~hdIBCheckbox();
	
	hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();
	
	hdUIReceiver * CreateInterfaceElement();
	
	void Draw() const;
	
	char * GetTexture(hdIBCheckboxTextureType_t type);
	
	void SetTexture(const char *texture, hdIBCheckboxTextureType_t type);
	
private:
	char m_textureOnNormal[kMaxTexturePathSize]; 
	char m_textureOnOver[kMaxTexturePathSize]; 
	char m_textureOffNormal[kMaxTexturePathSize];
	char m_textureOffOver[kMaxTexturePathSize];
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdIBProxy>(*this);
		ar & m_textureOnNormal;
		ar & m_textureOnOver;
		ar & m_textureOffNormal;
		ar & m_textureOffOver;
	}
};


inline hdInterfaceBuilderProxyType_t hdIBCheckbox::GetInterfaceBuilderProxyType()
{
	return hdInterfaceBuilderProxyTypeCheckbox;
}


inline char * hdIBCheckbox::GetTexture(hdIBCheckboxTextureType_t type)
{
	switch (type) 
	{
		case hdIBCheckboxTextureType_OnNormal:
			return m_textureOnNormal;
			break;
		case hdIBCheckboxTextureType_OnOver:
			return m_textureOnOver;
			break;
		case hdIBCheckboxTextureType_OffNormal:
			return m_textureOffNormal;
			break;
		case hdIBCheckboxTextureType_OffOver:
			return m_textureOffOver;
			break;
		default:
			return NULL;
			break;
	}
}


inline void hdIBCheckbox::SetTexture(const char *texture, hdIBCheckboxTextureType_t type)
{
	switch (type) 
	{
		case hdIBCheckboxTextureType_OnNormal:
			snprintf(m_textureOnNormal, kMaxTexturePathSize, "%s", texture);
			break;
		case hdIBCheckboxTextureType_OnOver:
			snprintf(m_textureOnOver, kMaxTexturePathSize, "%s", texture);
			break;
		case hdIBCheckboxTextureType_OffNormal:
			snprintf(m_textureOffNormal, kMaxTexturePathSize, "%s", texture);
			break;
		case hdIBCheckboxTextureType_OffOver:
			snprintf(m_textureOffOver, kMaxTexturePathSize, "%s", texture);
			break;
		default:
			break;
	}
}

#endif //HD_IB_CHECKBOX_H