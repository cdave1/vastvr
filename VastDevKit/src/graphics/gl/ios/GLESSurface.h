/* via https://github.com/cdave1/iglview under MIT License */

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol GLESSurfaceDelegate <NSObject>
@required
- (CAEAGLLayer *) getLayer;
- (CGRect) getBounds;
@end


@protocol GLESAbstractSurface <NSObject>
@required
- (BOOL) createSurface;
- (void) destroySurface;
- (void) bindFrameBuffer;
- (void) bindRenderBufferAndPresent;
- (void) swapBuffers;
@end


@interface GLESSurface : NSObject
{
@protected
    EAGLContext *_context;
    BOOL _hasBeenCurrent;
    id<GLESSurfaceDelegate> _delegate;
}

- (void) setCurrentContext;
- (BOOL) isCurrentContext;
- (void) clearCurrentContext;

@property(readonly) EAGLContext *context;
@property(assign) id<GLESSurfaceDelegate> delegate;

@end