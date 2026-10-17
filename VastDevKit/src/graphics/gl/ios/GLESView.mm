/* via https://github.com/cdave1/iglview under MIT License */


#import "GLESView.h"
#import "GLES1Surface.h"
#import "GLES2Surface.h"

@implementation GLESView

@synthesize autoresizesSurface = _autoresize,
                    surfaceSize=_size;


+ (Class)layerClass
{
    return [CAEAGLLayer class];
}


- (id) initWithFrame:(CGRect)frame
{
    if((self = [super initWithFrame:frame])) 
    { 
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
        
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:YES], 
                                        kEAGLDrawablePropertyRetainedBacking, 
                                        kEAGLColorFormatRGB565, 
                                        kEAGLDrawablePropertyColorFormat, 
                                        nil];
        
        _surface = [[GLES1Surface alloc] init];
        if(!_surface) 
        {
            _surface = [[GLES2Surface alloc] init];
            if (!_surface)
            {
                [self release];
                return nil;
            }
        }
        
        _surface.delegate = self;
        
        if([[UIScreen mainScreen] respondsToSelector:@selector(scale)] &&
           [self respondsToSelector:@selector(contentScaleFactor)]) 
        {
            self.contentScaleFactor = MAX(1.0f, [[UIScreen mainScreen] scale]);
        }
        
        if (![_surface createSurface])
        {
            [self release];
            return nil;
        }
        
        self.opaque = YES;
        _size = frame.size;
    }

    return self;
}


- (void) dealloc
{
    [_surface release];
    [super dealloc];
}


- (void) layoutSubviews
{
    CGRect bounds = [self bounds];
    
    if(_autoresize && ((roundf(bounds.size.width) != _size.width) || (roundf(bounds.size.height) != _size.height))) 
    {
        [_surface destroySurface];
        [_surface createSurface];
        _size = bounds.size;
    }
}


- (void) setAutoresizesEAGLSurface:(BOOL)autoresizesEAGLSurface
{
    _autoresize = autoresizesEAGLSurface;
    if(_autoresize)
        [self layoutSubviews];
}


- (void) bindFrameBuffer
{
    [_surface bindFrameBuffer];
}


- (void) swapBuffers
{
    [_surface swapBuffers];
}


- (void) setCurrentContext
{
    [_surface setCurrentContext];
}


- (CAEAGLLayer *) getLayer
{
    return (CAEAGLLayer*)[self layer];
}


- (CGRect) getBounds
{
    return [self bounds];
}



@end
