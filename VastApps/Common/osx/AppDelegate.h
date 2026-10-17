#import <Cocoa/Cocoa.h>

#import "AppView.h"
#include "core/application/Client.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
@private
	NSObject *_lock;
    hd::application::Client *m_client;
}

@property (assign) AppView *glView;
@property (assign) NSWindow *window;

- (void) dealloc;

- (void) update;
- (void) render;

@end
