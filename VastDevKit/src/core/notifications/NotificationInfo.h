#ifndef _VAST_NOTIFICATION_INFO_H_
#define _VAST_NOTIFICATION_INFO_H_

#define _VAST_NOTIFICATION_INFO_KEY_LEN 64
#define _VAST_NOTIFICATION_INFO_MESSAGE_LEN 256
#define _VAST_NOTIFICATION_CONTEXT_OBSERVER_COUNT 16

class AbstractNotificationCallback;

enum class NotificationResult {
    Default     = 0,  // Default response
    Cancel      = 1,
    Resubmit    = 2,
};

typedef struct {
	char name[_VAST_NOTIFICATION_INFO_KEY_LEN];
	char message[_VAST_NOTIFICATION_INFO_MESSAGE_LEN];
	void * sender;
    NotificationResult result;
} NotificationInfo_t;

typedef struct {
	char name[_VAST_NOTIFICATION_INFO_KEY_LEN];
	AbstractNotificationCallback * observerList;
} hdNotificationObservers_t;

#endif