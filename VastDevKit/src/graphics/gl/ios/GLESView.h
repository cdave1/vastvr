/* via https://github.com/cdave1/iglview under MIT License */


#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "GLESSurface.h"

@class GLESView;

@interface GLESView : UIView <GLESSurfaceDelegate>
{
@private
    BOOL _autoresize;
    CGSize _size;
    GLESSurface<GLESAbstractSurface> *_surface;
}

@property BOOL autoresizesSurface;
@property(readonly, nonatomic) CGSize surfaceSize;

- (id) initWithFrame:(CGRect)frame;
- (void) bindFrameBuffer;
- (void) swapBuffers;
- (void) setCurrentContext;

@end
