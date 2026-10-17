/*
 *  hdIBController.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 * A collection of interface entities.
 */

#ifndef HD_VIEW_CONTROLLER_H_
#define HD_VIEW_CONTROLLER_H_

#include "hdk.h"
#include "UI/hdIBViewController.h"
#include "UI/hdIBLoader.h"

class hdUIViewController : public hdUIContainer
{
public:
    
    hdUIViewController();
	
	hdUIViewController(const float width, const float height);
	
	hdUIViewController(const char *hdIBViewControllerFilePath);
	
	~hdUIViewController();
	
    void InitWithViewControllerFilePath(const char *hdIBViewControllerFilePath);
    
	virtual void RefreshLayout(const e_hdInterfaceOrientation &orientation);
	
	void SetMouseDownCallbackForAllItems(void *obj, void (*func)(void *, void *));
	
	void SetMouseUpCallbackForAllItems(void *obj, void (*func)(void *, void *));
	
	void SetMouseDownCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *));
	
	void SetMouseUpCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *));
	
	
	/**
	 * For all items with tag, add the attached listener for mouse down
	 * events
	 */
	void SetMouseDownCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *)); 
	
	/**
	 * Same as SetMouseDownCallbackForTaggedItems, but for mouse up events.
	 */
	void SetMouseUpCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *));
	
	/**
	 * For anything (checkbox) that sends value changed events
	 */
	void SetValueChangedCallbackForGroupedItems(const char *group, void *obj, void (*func)(void *, void *));
	
	/**
	 * First interface item for the given tag
	 */
	const hdUIReceiver * GetInterfaceItemWithTag(const char *tag);
	
	/**
	 * First interface item for the given group
	 */
	const hdUIReceiver * GetInterfaceItemWithGroup(const char *group);
	
	
	const uint32 GetInterfaceItemsForGroup(const char *group, hdUIReceiver ** items, uint32 maxItems) const;
	
	
	void SetCloseListener(void *obj, void (*func)(void *, void *));
	
	void Draw();
	
	void ScaleToScreenDimensions(const float screenWidth, const float screenHeight);
	
protected:
	
	virtual void InitInterface();
	
	virtual void InitSounds();
	
	virtual void DoClosePrefsCallback();
	
private:
	void WillDraw() const;
	
	void DidDraw() const;
	
	void (*m_closePrefsCallback)(void *, void *);
	
	void *m_closePrefsCallbackObject;
	
	hdIBViewController *m_hdIBViewController;
	
	float m_width;
	
	float m_height;
	
};


#endif