#import <Cocoa/Cocoa.h>

#import "AppView.h"

@interface AppDelegate : NSResponder <NSApplicationDelegate>
{
@private
	NSObject *_lock;
}

@property (assign) AppView *glView;
@property (assign) NSWindow *window;


- (IBAction) setFullScreen:(id)sender;
- (void) dealloc;
- (void)keyDown:(NSEvent *)event;
@end
