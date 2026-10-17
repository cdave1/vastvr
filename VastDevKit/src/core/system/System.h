#ifndef _VAST_SYSTEM_H_
#define _VAST_SYSTEM_H_

namespace hdk {
    class System {
#ifdef ANDROID
        void SetJavaClassRef(void * ref);
#endif
        
        const char * GetRootDirectory();
        
        void SetWriteDirectory(const char * dir);
        
        const char * GetWriteDirectory();
        
        void ScheduleUserNotification(const char * msg,
                                      const char * msg2,
                                      const float delaySeconds);
        
        void CancelUserNotifications();
        
        void Alert(const char * title, const char * msg);
        
        void UpdateAlert(const char * title, const char * txt, const char * updateUrl);
        
        
        const char * GetPlatformName();
        
        const char * GetOSVersion();
        
        const char * GetLocaleCountryCode();
    };
}

#endif
