/*
 *  hdIBContainer.h
 *  hdkOSX
 *
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef HD_IB_CONTAINER_H_
#define HD_IB_CONTAINER_H_

#include "hdIBProxy.h"

class hdIBContainer : public hdIBProxy
{
private:
	hdTypedefList<hdIBProxy *, 32>* m_proxyItems;
};


#endif