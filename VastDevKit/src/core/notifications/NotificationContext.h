#ifndef _VAST_NOTIFICATION_CONTEXT
#define _VAST_NOTIFICATION_CONTEXT

#include <vast/Vast.h>
#include "NotificationCallback.h"
#include "NotificationInfo.h"
#include <pthread.h>

#define _VAST_NOTIFICATION_CONTEXT_POSTS_SIZE 64
#define _VAST_NOTIFICATION_CONTEXT_HASH_TABLE_SIZE 2048

class NotificationContext {
public:
	static NotificationContext * CurrentContext();
    
    static void SetCurrentContext(NotificationContext * context);
    
    NotificationContext();
	
	~NotificationContext();
	
	template<class T>
	bool AddObserver(const char *name, T *obj, void (T::*callbackFunc)(NotificationInfo_t *));
    
    template<class T>
    bool AddStaticObserver(const char *name, void (*callbackFunc)(NotificationInfo_t *));
	
	bool RemoveObserver(const char *name);
	
	bool RemoveObserver(const char *name, const void *obj);
	
	bool PostNotification(const char *name);
	
	bool PostNotification(const char *name, const char *msg);
	
	bool PostNotification(const char *name, const char *msg, void *sender);
	
    
private:
    
	friend class hdApplication;
	
	bool ProcessNotifications(); // can only be called by hdApplication!
	
	int GetObserverTableHashIndex(const char *name);
	
	bool AddObserverInternal(const char *name, AbstractNotificationCallback *callback);
	
	void GetPostedNotifications();
	
    void Lock();
    
    void Unlock();
    
	bool IsLocked();
    
    void LockPosts();
    
    void UnlockPosts();
    
	
	AbstractNotificationCallback * FindObserver(const hdNotificationObservers_t *observers, const void *obj) const;
	
	 // need a hashtable for multiple observers.
	hdNotificationObservers_t m_observers[_VAST_NOTIFICATION_CONTEXT_HASH_TABLE_SIZE];
	
	NotificationInfo_t m_postedNotifications[_VAST_NOTIFICATION_CONTEXT_POSTS_SIZE];
	
	unsigned int m_postedNotificationsCount;
	
	pthread_mutex_t m_mutex;
	
	pthread_mutex_t	m_copyPostedNotificationsMutex;
    
};



template<class T>
bool NotificationContext::AddStaticObserver(const char *name, void (*callbackFunc)(NotificationInfo_t *)) {
	NotificationCallback<T> *callback = new NotificationCallback<T>();
	callback->SetStaticFunction(callbackFunc);
	
	if (!this->AddObserverInternal(name, (AbstractNotificationCallback *)callback)) {
		hdDeleteSafely(callback);
		return false;
	}
	
	return true;    
}


template<class T>
bool NotificationContext::AddObserver(const char *name, T *obj, void (T::*callbackFunc)(NotificationInfo_t *)) {
	NotificationCallback<T> *callback = new NotificationCallback<T>();
	callback->SetFunction(callbackFunc);
	callback->SetFunctionObject(obj);
	
	if (!this->AddObserverInternal(name, (AbstractNotificationCallback *)callback)) {
		hdDeleteSafely(callback);
		return false;
	}
	
	return true;
}

#endif
