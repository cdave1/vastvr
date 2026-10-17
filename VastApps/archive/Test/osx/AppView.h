#import <Cocoa/Cocoa.h>

@interface AppView : NSOpenGLView <NSWindowDelegate>
{
}

- (void)update;		// moved or resized
- (void)reshape;	// scrolled, moved or resized

- (void)prepareOpenGL;
- (void) drawRect: (NSRect) bounds;
- (void) blbl;

@end
