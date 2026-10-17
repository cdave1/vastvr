#import <Cocoa/Cocoa.h>
#include "hdk.h"

@interface AppView : NSOpenGLView
{
    id renderProxy;
	int colorBits, depthBits;
	BOOL runningFullScreen;
	NSDictionary *originalDisplayMode;
    hd::application::Client *m_client;
}

- (void) awakeFromNib;
- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;

//- (void) reshape;
- (void) nextFrame;
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
- (BOOL) isFullScreen;
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame;
- (void) setRenderProxy:(id)proxy;
- (void) setClient:(hd::application::Client *)client;

@end
