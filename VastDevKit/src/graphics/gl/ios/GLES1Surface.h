/* via https://github.com/cdave1/iglview under MIT License */

#import "GLESSurface.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface GLES1Surface : GLESSurface <GLESAbstractSurface>
{
@private
    GLuint _format;
    GLuint _depthFormat;
    GLuint _framebuffer;
    GLuint _renderbuffer;
    GLuint _depthBuffer;
}

- (id) init;
- (id) initWithPixelFormat:(GLuint)format;
- (id) initWithPixelFormat:(GLuint)format 
               depthFormat:(GLuint)depth 
        preserveBackbuffer:(BOOL)retained;

@property(readonly) GLuint framebuffer;
@property(readonly) GLuint pixelFormat;
@property(readonly) GLuint depthFormat;

@end