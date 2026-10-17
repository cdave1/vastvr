#ifndef _VAST_NOTIFICATION_CALLBACK
#define _VAST_NOTIFICATION_CALLBACK

#include "NotificationInfo.h"

class AbstractNotificationCallback {
public:
	AbstractNotificationCallback() {
		m_next = NULL;
	}
	
	virtual void DoCallback(NotificationInfo_t * notification) = 0;
	
	virtual bool ContainsFunctionObject(const void *obj) const = 0;

	AbstractNotificationCallback *m_next;
};


template<class T>
class NotificationCallback : AbstractNotificationCallback {
public:
	
    NotificationCallback() : AbstractNotificationCallback() {
		m_func = NULL;
        m_staticFunc = NULL;
		m_obj = NULL;
	}
	
	~NotificationCallback() {}
	
	void DoCallback(NotificationInfo_t * notification);
	
	void SetFunction(void (T::*func)(NotificationInfo_t *));
	
    void SetStaticFunction(void (*func)(NotificationInfo_t *));
    
	void SetFunctionObject(const T* obj);
	
	bool ContainsFunctionObject(const void *obj) const;
	
    
private:
    
	void (T::*m_func)(NotificationInfo_t *);
    
    void (*m_staticFunc)(NotificationInfo_t *);
	
	T* m_obj;

};


template<class T>
inline void NotificationCallback<T>::SetFunction(void (T::*func)(NotificationInfo_t *)) {
	m_func = func;
}


template<class T>
inline void NotificationCallback<T>::SetStaticFunction(void (*func)(NotificationInfo_t *)) {
	m_staticFunc = func;
    m_obj = NULL;
}


template<class T>
inline void NotificationCallback<T>::SetFunctionObject(const T* obj) {
	hdAssert((void*)obj != (void*)this);
	m_obj = (T*)obj;
}


template<class T>
void NotificationCallback<T>::DoCallback(NotificationInfo_t * notification) {
	if (m_obj == NULL) {
        (*m_staticFunc)(notification);
	} else {
        (m_obj->*m_func)(notification);
    }
}


template<class T>
bool NotificationCallback<T>::ContainsFunctionObject(const void *obj) const {
	if (!m_obj) return true;
	return m_obj == obj;
}

#endif
