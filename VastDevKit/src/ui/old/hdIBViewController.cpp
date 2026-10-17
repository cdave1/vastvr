/*
 *  hdIBViewController.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBViewController.h"
#include "UI/hdUIContext.h"

hdIBViewController::hdIBViewController(const char *name)
{
	this->SetName(name);
	this->SetDimensions(480.0f, 320.0f);
	m_proxies = new hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES>();
}


hdIBViewController::hdIBViewController(const char *name, const float width, const float height)
{
	this->SetName(name);
	this->SetDimensions(width, height);
	m_proxies = new hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES>();
}


hdIBViewController::~hdIBViewController()
{
	delete m_proxies;
}


void hdIBViewController::CreateInterfaceObjects()
{
	for (unsigned i = 0; i < m_proxies->GetItemCount(); ++i)
	{
		if (m_proxies->GetItems()[i])
		{
			hdUIReceiver *receiver = m_proxies->GetItems()[i]->CreateInterfaceElement();
			receiver->SetTag((int)strtol(m_proxies->GetItems()[i]->GetTag(), NULL, 0));
		}
	}
	SortProxiesByZOrder();
}


const int hdIBViewController::AABBQuery(const hdAABB& aabb, hdIBProxy ** objects, int maxObjects)
{
	int count = 0;
	
	hdIBProxy * object = NULL;
	
	for (unsigned i = 0; i < m_proxies->GetItemCount(); ++i)
	{
		if (m_proxies->GetItems()[i])
		{
			object = m_proxies->GetItems()[i];
			if (object->AABBIntersection(aabb))
			{
				objects[(count++)%maxObjects] = object;
			}
		}
	}
	return count;
}


void hdIBViewController::SortProxiesByZOrder()
{
	/*
	hdTypedefList<hdIBProxy *, MAX_VIEW_CONTROLLER_PROXIES> *tmpProxies = 
		new hdTypedefList<hdIBProxy *, MAX_VIEW_CONTROLLER_PROXIES>();
	*/
	hdPrintf("Sorting Proxies...\n");
	for (unsigned i = 0; i < m_proxies->GetItemCount(); ++i)
	{
		hdPrintf("\t%d. Z offset: %3.5f\n", i, m_proxies->GetItems()[i]->GetZOffset());
	}
	
	for (int j = 1; j < (int)m_proxies->GetItemCount(); j++)
	{
		hdIBProxy *proxy = m_proxies->GetItems()[j];
		
		int i = j - 1;
		
		while (i >= 0 && m_proxies->GetItems()[i]->GetZOffset() > proxy->GetZOffset())
		{
			m_proxies->Replace(m_proxies->GetItems()[i], i + 1);
			i = i - 1;
		}
		
		m_proxies->Replace(proxy, i + 1);
	}
	
	hdPrintf("Proxies sorted. New order is:\n");
	for (unsigned i = 0; i < m_proxies->GetItemCount(); ++i)
	{
		hdPrintf("\t%d. Z offset: %3.5f\n", i, m_proxies->GetItems()[i]->GetZOffset());
	}
}


void hdIBViewController::Draw() const
{
	// sort proxies
	for (unsigned i = 0; i < m_proxies->GetItemCount(); ++i)
	{
		if (m_proxies->GetItems()[i])
		{
			m_proxies->GetItems()[i]->Draw();
		}
	}
}