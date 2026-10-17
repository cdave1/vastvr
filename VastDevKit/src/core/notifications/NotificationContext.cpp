#include "NotificationContext.h"

static NotificationContext * g_currentNotificationContext = NULL;


NotificationContext * NotificationContext::CurrentContext()
{
    return g_currentNotificationContext;
}


void NotificationContext::SetCurrentContext(NotificationContext * context)
{
    g_currentNotificationContext = context;
}


NotificationContext::NotificationContext()
{
	memset(m_postedNotifications, 0, sizeof(m_postedNotifications));
	memset(m_observers, 0, sizeof(m_observers));
	m_postedNotificationsCount = 0;
	
	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&m_copyPostedNotificationsMutex, NULL);
}


NotificationContext::~NotificationContext()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_mutex_destroy(&m_copyPostedNotificationsMutex);
}


bool NotificationContext::RemoveObserver(const char *name)
{
	return RemoveObserver(name, NULL);
}


bool NotificationContext::RemoveObserver(const char *name, const void *obj)
{
	if (IsLocked())
	{
		hd::log::i("Cannot remove observer. Notification center is locked.\n");
		return false;
	}
	
	Lock();
	int hashIndex = GetObserverTableHashIndex(name);
	if (hashIndex == -1)
	{
		hd::log::e("Could not remove observer -- no hash match.\n");
		Unlock();		
		return false;
	}
	
	hdNotificationObservers_t *observerEntry = &m_observers[hashIndex];
	if (!observerEntry->observerList) return true;
	
	AbstractNotificationCallback *lst, *target;
	
	if (!(target = FindObserver(observerEntry, obj)))
	{
		hd::log::e("Could not remove observer -- no observer for object.\n");
		Unlock();		
		return false;
	}
	
	if (target == observerEntry->observerList)
	{
		observerEntry->observerList = target->m_next;
	}
	else 
	{
		for (lst = observerEntry->observerList; lst->m_next && lst->m_next != target; lst = lst->m_next) ;
		lst->m_next = target->m_next;
	}
	
	hdDeleteSafely(target);
	Unlock();
	return true;
}


bool NotificationContext::PostNotification(const char *name)
{
	return PostNotification(name, "", NULL);
}


bool NotificationContext::PostNotification(const char *name, const char *msg)
{
	return PostNotification(name, msg, NULL);
}


bool NotificationContext::PostNotification(const char *name, const char *msg, void *sender)
{
    LockPosts();
    
    if (m_postedNotificationsCount >= _VAST_NOTIFICATION_CONTEXT_POSTS_SIZE)
	{
		//hd::log::i("Cannot post a notification. Notification storage is full.\n");
        UnlockPosts();
		return false;		
	}
    
    assert(m_postedNotificationsCount < _VAST_NOTIFICATION_CONTEXT_POSTS_SIZE);

	NotificationInfo_t *notification = &m_postedNotifications[m_postedNotificationsCount];
	snprintf(notification->name, _VAST_NOTIFICATION_INFO_KEY_LEN, "%s", name);
	snprintf(notification->message, _VAST_NOTIFICATION_INFO_MESSAGE_LEN, "%s", msg);
	notification->sender = sender;
	++m_postedNotificationsCount;
    
	UnlockPosts();
	
    return true;
}


bool NotificationContext::IsLocked()
{
	if (0 != pthread_mutex_trylock(&m_mutex))
	{
		return true;
	}
	pthread_mutex_unlock(&m_mutex);
	return false;
}


void NotificationContext::Lock()
{
#ifdef DEBUG
    assert(!IsLocked());
#endif
    pthread_mutex_lock(&m_mutex);
}


void NotificationContext::Unlock()
{
#ifdef DEBUG
    assert(IsLocked());
#endif
    pthread_mutex_unlock(&m_mutex);
}


void NotificationContext::LockPosts()
{
    int lockRes = 0;
    if (0 != (lockRes = pthread_mutex_lock(&m_copyPostedNotificationsMutex)))
	{
		hd::log::i("pthread lock error for m_copyPostedNotificationsMutex (%d)!\n", lockRes);
	}
}


void NotificationContext::UnlockPosts()
{
    pthread_mutex_unlock(&m_copyPostedNotificationsMutex);
}


bool NotificationContext::AddObserverInternal(const char *name, AbstractNotificationCallback *callback)
{
	if (IsLocked())
	{
		hd::log::i("Cannot add observer. Notification center is locked.\n");
		return false;
	}
	
	Lock();
	int hashIndex = GetObserverTableHashIndex(name);
	if (hashIndex == -1)
	{
		hd::log::e("Could not add observer -- name string was empty.\n");
		Unlock();		
		return false;
	}
	
	hdNotificationObservers_t *observerEntry = &m_observers[hashIndex];	
	
	callback->m_next = observerEntry->observerList;
	observerEntry->observerList = callback;
	
	Unlock();
	
	return true;
}


AbstractNotificationCallback * NotificationContext::FindObserver(const hdNotificationObservers_t *observers, const void *obj) const
{
	AbstractNotificationCallback *lst = observers->observerList;
	while(lst && lst->ContainsFunctionObject(obj))
	{
		lst = lst->m_next;
	}
	return lst;	
}


/**
 * For each notification in notification_t, find an observer with the
 * matching key, and then call the appropriate callback.
 */
bool NotificationContext::ProcessNotifications()
{
	NotificationInfo_t notificationsCopy[_VAST_NOTIFICATION_CONTEXT_POSTS_SIZE];
	unsigned int notificationsCopyCount = 0;

	LockPosts();
	
	// Copy the notifications list.
	memset(notificationsCopy, 0, sizeof(notificationsCopy));
	memcpy(notificationsCopy, m_postedNotifications, sizeof(NotificationInfo_t) * m_postedNotificationsCount);
	notificationsCopyCount = m_postedNotificationsCount;
	
	// Zero the notifications list.
	memset(m_postedNotifications, 0, sizeof(m_postedNotifications));
	m_postedNotificationsCount = 0;
	
	UnlockPosts();
	
	if (IsLocked())
	{
		hd::log::i("Cannot process notifications. Notification center is locked. Do not add/remove observers inside callbacks!\n");
		return false;
	}
	
	unsigned calledNotifications = 0;
	Lock();
	for (unsigned i = 0; i < notificationsCopyCount; ++i)
	{
		NotificationInfo_t *notification = &notificationsCopy[i];
		
		int hashIndex = GetObserverTableHashIndex(notification->name);
		if (hashIndex >= 0)
		{
			hdNotificationObservers_t *observerEntry = &m_observers[hashIndex];
			AbstractNotificationCallback *lst = observerEntry->observerList;
            
			while(lst)
			{
				lst->DoCallback(notification);
				lst = lst->m_next;
				++calledNotifications;
			}
		}
	}
	
	if (calledNotifications > 0)
		hd::log::i("[NotificationContext] %d notification callbacks were processed.\n", calledNotifications);
	
	Unlock();
	return true;
}


int NotificationContext::GetObserverTableHashIndex(const char *name)
{
	if (strlen(name) == 0) return -1;
	
	unsigned short sanity = 0;
	unsigned long hash = hd::hash((unsigned char *)name) & (_VAST_NOTIFICATION_CONTEXT_HASH_TABLE_SIZE - 1);
	
	hdNotificationObservers_t observerEntry = m_observers[hash];
	
	// collisions -- if names don't actually match then
	// go to the next slot.
	while ((strcmp(observerEntry.name, name) != 0) &&
		   (++sanity < 16))
	{
		hash = (hash+1) & (_VAST_NOTIFICATION_CONTEXT_HASH_TABLE_SIZE - 1);
		observerEntry = m_observers[hash];
	}
	hdAssert(hash < _VAST_NOTIFICATION_CONTEXT_HASH_TABLE_SIZE);
	return (int) hash;
}
