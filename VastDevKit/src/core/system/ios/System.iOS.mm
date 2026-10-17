#include "System.h"
#import <UIKit/UIKit.h>
#import "QuartzCore/QuartzCore.h"

#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>


@interface UIDevice (Hardware)
- (NSString *) getSysInfoByName:(const char *)typeSpecifier;
- (NSString *) platform;
@end



@implementation UIDevice (Hardware)

- (NSString *) getSysInfoByName:(const char *)typeSpecifier
{
    size_t size;
    sysctlbyname(typeSpecifier, NULL, &size, NULL, 0);
    
    char *answer = (char *)malloc(size);
    sysctlbyname(typeSpecifier, answer, &size, NULL, 0);
    
    NSString *results = [NSString stringWithCString:answer encoding: NSUTF8StringEncoding];
    
    free(answer);
    return results;
}

- (NSString *) platform
{
    return [self getSysInfoByName:"hw.machine"];
}

@end


const char * hdk::System::GetRootDirectory()
{
    const char *bundleResourcePath = 
        [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSASCIIStringEncoding];
    return bundleResourcePath;
}


const char * hdk::System::GetWriteDirectory()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    return [documentsDirectory UTF8String];
}


void hdk::System::ScheduleUserNotification(const char * msg, 
                                        const char * msg2, 
                                        const float delaySeconds)
{
    UILocalNotification * notification = [[UILocalNotification alloc] init];
    if (!notification)
    {
        hdError("Error creating notification.\n");
        return;
    }
    
    NSDate * date = [NSDate dateWithTimeIntervalSinceNow:(NSTimeInterval)delaySeconds];
    
    notification.fireDate = date;
    
    notification.timeZone = [NSTimeZone defaultTimeZone];
    
    notification.alertBody = [NSString stringWithUTF8String:msg];
    
    notification.alertAction = [NSString stringWithUTF8String:msg2];
    
    notification.soundName = UILocalNotificationDefaultSoundName;
    
    notification.applicationIconBadgeNumber = 1;
    
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    
    [notification release];
}


extern void hdk::System::CancelUserNotifications()
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}


const char * hdk::System::GetPlatformName()
{
    if ([UIDevice currentDevice] &&
        [[UIDevice currentDevice] respondsToSelector:@selector(platform)])
    {
        return [[[UIDevice currentDevice] platform] UTF8String];
    }
    return "???";
}


static char m_os[64];
const char * hdk::System::GetOSVersion()
{
    if ([UIDevice currentDevice]
        && [[UIDevice currentDevice] respondsToSelector:@selector(systemVersion)]
        && [[UIDevice currentDevice] respondsToSelector:@selector(systemName)])
    {
        snprintf(m_os, 64, "%s %s", [[[UIDevice currentDevice] systemVersion] UTF8String],
                 [[[UIDevice currentDevice] systemName] UTF8String]);
        return m_os;
    }
    return "???";
}


const char * hdk::System::GetLocaleCountryCode()
{
    if ([NSLocale currentLocale]
        && [[NSLocale currentLocale] objectForKey: NSLocaleCountryCode] )
    {
        return [[[NSLocale currentLocale] objectForKey: NSLocaleCountryCode] UTF8String];
    }
    return "";
}


extern void hdk::System::Alert(const char * title, const char * msg)
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:title]
                                                        message:[NSString stringWithUTF8String:msg]
													   delegate: nil
											  cancelButtonTitle: @"OK"
											  otherButtonTitles: nil];
    [alert show];
    [alert release];
}
