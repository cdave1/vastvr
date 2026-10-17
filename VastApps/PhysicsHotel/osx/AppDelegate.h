#import <Cocoa/Cocoa.h>

#import "AppView.h"
#include "core/application/Client.h"

@interface AppDelegate : NSResponder <NSApplicationDelegate>
{
@private
    IBOutlet NSWindow *_glWindow;
	IBOutlet AppView *_glView;
	NSObject *_lock;
    hd::application::Client *m_client;
}

- (void) awakeFromNib;
- (void) keyDown:(NSEvent *)theEvent;
- (IBAction) setFullScreen:(id)sender;
- (IBAction) resize:(id)sender;
- (void) dealloc;
- (void) render;

@end
