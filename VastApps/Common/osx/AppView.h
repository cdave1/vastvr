#import <Cocoa/Cocoa.h>
#include <Vast/Vast.h>

@interface AppView : NSOpenGLView
{
    id renderProxy;
    hd::application::Client *m_client;
    NSTimer *timer;
    NSTimer *updateTimer;
}

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;

- (void)prepareOpenGL;
- (void) drawRect: (NSRect) bounds;
- (void) render;

- (void) drawRect: (NSRect) bounds;
- (void) setRenderProxy:(id)proxy;
- (void) setClient:(hd::application::Client *)client;
- (void)keyDown:(NSEvent *)event;

- (void)keyUp:(NSEvent *)event;


- (void)mouseDown:(NSEvent *)event;


- (void)mouseUp:(NSEvent *)event;


- (void) mouseEntered:(NSEvent *)theEvent;


- (void) mouseExited:(NSEvent *)theEvent;


- (void)mouseMoved:(NSEvent *)event;


- (void)mouseDragged:(NSEvent *)event;


- (void)scrollWheel:(NSEvent *)theEvent;

@end
