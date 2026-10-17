/*
 *  hdIBFontPolygon.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef HD_IB_FONT_POLYGON_H_
#define HD_IB_FONT_POLYGON_H_

#include "hdIBProxy.h"

typedef enum
{
	hdIBFontPolygonAlignmentLeft = 0,
	hdIBFontPolygonAlignmentCenter,
	hdIBFontPolygonAlignmentRight,
	hdIBFontPolygonAlignmentCount,
} hdIBFontPolygonAlignment_t;


class hdIBFontPolygon : public hdIBProxy
{
public:
	hdIBFontPolygon();
	
	hdIBFontPolygon(const char *fontFileName, 
					const hdVec3 &lower,
					const hdVec3 &upper);
	
	~hdIBFontPolygon();
	
	hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();
	
	hdUIReceiver * CreateInterfaceElement();
	
	void Draw() const;
	
	const char * GetText() const;
	
	void SetText(const char *text);
	
	const char * GetFontFileName() const;
	
	void SetFontFileName(const char *fontFileName);
	
	const hdColor4f GetTint() const;
	
	void SetTint(const hdColor4f& tint);
	
	const hdIBFontPolygonAlignment_t GetFontAlignment() const;
	
	void SetFontAlignment(const hdIBFontPolygonAlignment_t alignment);
	
private:
	char m_fontFileName[kMaxTexturePathSize];
	char m_text[2048];
	hdColor4f m_tint;
	hdIBFontPolygonAlignment_t m_fontAlignment;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdIBProxy>(*this);
		ar & m_fontFileName;
		ar & m_text;
		
		if (version >= 1)
		{
			ar & m_tint.r;
			ar & m_tint.g;
			ar & m_tint.b;
			ar & m_tint.a;
			ar & m_fontAlignment;
		}
	}
};


BOOST_CLASS_VERSION(hdIBFontPolygon, 1)


inline hdInterfaceBuilderProxyType_t hdIBFontPolygon::GetInterfaceBuilderProxyType() 
{
	return hdInterfaceBuilderProxyTypeFontPolygon; 
}


inline const char * hdIBFontPolygon::GetFontFileName() const
{
	return m_fontFileName;
}


inline void hdIBFontPolygon::SetFontFileName(const char *fontFileName)
{
	snprintf(m_fontFileName, kMaxTexturePathSize, "%s", fontFileName);
}


inline const char * hdIBFontPolygon::GetText() const
{
	return m_text;
}


inline const hdColor4f hdIBFontPolygon::GetTint() const
{
	return m_tint;
}


inline const hdIBFontPolygonAlignment_t hdIBFontPolygon::GetFontAlignment() const
{
	return m_fontAlignment;
}






#endif