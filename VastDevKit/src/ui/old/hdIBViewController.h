/*
 *  hdIBViewController.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

/*
 * The interface builder saves and loads IB View Controllers.
 *
 * Main purpose is to maintain a list of IBProxy elements, and to
 * produce an hdUIViewController with various interface elements.
 */

#ifndef HD_IB_VIEW_CONTROLLER_H_
#define HD_IB_VIEW_CONTROLLER_H_

#include "hdIBProxy.h"
#include "hdIBImage.h"
#include "hdIBButton.h"
#include "hdIBFontPolygon.h"
#include "hdIBCheckbox.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#define MAX_VIEW_CONTROLLER_PROXIES 128

class hdIBViewController : public hdDrawable
{
public:
	hdIBViewController() {};
	
	hdIBViewController(const char *name);
	
	hdIBViewController(const char *name, const float width, const float height);
	
	~hdIBViewController();
	
	hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * GetProxyList();
	
	void CreateInterfaceObjects();
	
	const int AABBQuery(const hdAABB& aabb, hdIBProxy ** objects, int maxObjects);
	
	const float GetWidth() const;
	
	void SetWidth(const float width);
	
	const float GetHeight() const;
	
	void SetHeight(const float height);
	
	void SetDimensions(const float width, const float height);
	
	const char * GetName() const;
	
	void SetName(const char *name);
	
	/**
	 * Sort by z offset. Uses slow insertion sort :(.
	 */
	void SortProxiesByZOrder();
	
	void Draw() const;
	
private:
	float m_width; 
	float m_height;
	char m_name[256];
	hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * m_proxies;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar.template register_type<hdIBFontPolygon>();
		ar.template register_type<hdIBButton>();
		ar.template register_type<hdIBImage>();
		if (version >= 1)
		{
			ar.template register_type<hdIBCheckbox>();
		}
		ar.template register_type<hdIBProxy>();
		ar.template register_type<hdPolygon>();
		ar & m_width;
		ar & m_height;
		ar & m_name;
		ar & m_proxies;
	}
	
};

BOOST_CLASS_VERSION(hdIBViewController, 1)


inline hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * hdIBViewController::GetProxyList()
{
	return m_proxies;
}


inline const float hdIBViewController::GetWidth() const
{
	return m_width;
}


inline void hdIBViewController::SetWidth(const float width)
{
	m_width = width;
}


inline const float hdIBViewController::GetHeight() const
{
	return m_height;
}


inline void hdIBViewController::SetHeight(const float height)
{
	m_height = height;
}


inline void hdIBViewController::SetDimensions(const float width, const float height)
{
	this->SetWidth(width);
	this->SetHeight(height);
}


inline const char * hdIBViewController::GetName() const
{
	return m_name;
}


inline void hdIBViewController::SetName(const char *name)
{
	snprintf(m_name, 256, "%s", name);
}


#endif