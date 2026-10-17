//
//  GLWindowController.h
//  skeleton
//
//  Created by David Petrie on 24/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <OpenGL/OpenGL.h>
#include "glimp.h"

@interface GLView : NSOpenGLView
{
    BOOL animating;
    BOOL displayLinkSupported;
    
    GLuint tex0;
    GLuint tex1;
     
    GLint texCoordLocation;
    GLint cameraLocation;
    GLint sampleLocation;
    GLint resolutionLocation;
    
    GLuint shaderProgram;
    GLuint cameraUniform;
    
    camera_t camera;
    float zRotate;
    float cameraMatrix[16]; // column major order
    float rotationMatrix[16];
    vec4_t translationVector;
}
- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
@end
