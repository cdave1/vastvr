//
//  GLWindowController.m
//  skeleton
//
//  Created by David Petrie on 24/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#import "GLView.h"

@interface GLView (PrivateMethods)
- (GLuint) loadTexture:(NSString *)filename;
- (GLuint) compileShader:(NSString *)filePath withType:(GLenum)type;
- (void) nextFrame;
- (void) render;
@end


@implementation GLView

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format
{
    return self = [super initWithFrame:frameRect pixelFormat:format];
}


static GLint timeLocation = 0;

- (void)prepareOpenGL
{
    glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
    
    shaderProgram = glCreateProgram();
    
    GLuint vertexShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"] withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:[[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"] withType:GL_FRAGMENT_SHADER];
    assert(vertexShader && fragmentShader);
    
    glLinkProgram(shaderProgram);
    
    cameraUniform = glGetUniformLocation(shaderProgram, "camera");
    
    // Release vertex and fragment shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    cameraLocation = glGetUniformLocation(shaderProgram, "camera");
    texCoordLocation = glGetAttribLocation(shaderProgram, "texCoord");
    resolutionLocation = glGetUniformLocation(shaderProgram, "resolution");
    
    timeLocation = glGetUniformLocation(shaderProgram, "time");
    
    tex0 = [self loadTexture:[[NSBundle mainBundle] pathForResource:@"wood" ofType:@"png"]];
    tex1 = [self loadTexture:[[NSBundle mainBundle] pathForResource:@"wood" ofType:@"png"]];
    
    //animationTimer = 
    [NSTimer scheduledTimerWithTimeInterval:0.016 target:self selector:@selector(nextFrame) userInfo:nil repeats:TRUE];
}


- (void) nextFrame
{
    [self setNeedsDisplay: YES];
    [self drawRect:[self bounds]];
}


- (void) drawRect: (NSRect) bounds;
{
    [self render];
}


- (GLuint)loadTexture:(NSString *)filename
{
    GLuint          handle;
    CGImageRef      spriteImage;
    int             width, height;
    CGContextRef    spriteContext;
    GLubyte         *spriteData;
    
    NSData * fileData = [[NSData alloc] initWithContentsOfFile:filename];
    NSBitmapImageRep * imageRep = [[NSBitmapImageRep alloc] initWithData:fileData];
    [fileData release];
    spriteImage = [imageRep CGImage];
    
    width = (int)CGImageGetWidth(spriteImage);
    height = (int)CGImageGetHeight(spriteImage);
    
    if(spriteImage)
    {
        spriteData = (GLubyte *)calloc(1, width * height * 4);
        spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4,
                                              CGImageGetColorSpace(spriteImage),
                                              kCGImageAlphaPremultipliedLast);
        CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
        CGContextRelease(spriteContext);
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        free(spriteData);
    }
    [imageRep release];
    return handle;
}


- (GLuint)compileShader:(NSString *)filePath withType:(GLenum)type
{
    GLuint shader;
    GLint status;
    const GLchar *source;
    
    if (!(source = (GLchar *)[[NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil] UTF8String]))
    {
        NSLog(@"Failed to load vertex shader");
        return FALSE;
    }
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    {

    }
    
    if (status == 0)
    {
        GLchar log[2048];
        GLsizei len;
        
        glGetShaderInfoLog(shader, 2048, &len, log);
        printf("%s\n", log);
        return 0;
    }
    else 
    {
    glAttachShader(shaderProgram, shader);
    return shader;
    }
}


static float zMove = 0.0f;
- (void)render
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader program
    glUseProgram(shaderProgram);
    zMove += 0.01f;
    zRotate += 0.05f;
    
#if 0
    vec3Set(camera.eye, 0.0f, 0.0f, -5.0f + sin(zMove));
    vec3Set(camera.center, sin(zMove), 0.0f, 10.0f);
    vec3Set(camera.up, 0.0f, 1.0f, 0.0f);
    aglMatrixLookAtRH(cameraMatrix, camera.eye, camera.center, camera.up);
#else
    aglOrtho(cameraMatrix, -1.333333f, 1.333333f, -1.0f, 1.0f, -10000.0f, 10000.0f);
#endif
    
    aglMatrixRotationZ(rotationMatrix, zRotate);
    glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, cameraMatrix);
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    
    glUniform1f(timeLocation, zMove); // sin(zMove));
    glUniform2f(resolutionLocation, self.frame.size.width, self.frame.size.height);
    
    aglBegin(GL_QUADS);
    
    aglTexCoord2f(0.0f, 1.0f);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(-1.0f, -1.0f, 0.0f);
    
    aglTexCoord2f(0.0f, 0.0f);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(-1.0f, 1.0f, 0.0f);
    
    aglTexCoord2f(1.0f, 0.0f);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(1.0f, 1.0f, 0.0f);
    
    aglTexCoord2f(1.0f, 1.0f);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(1.0f, -1.0f, 0.0f);
    
    aglEnd();
    
    glFlush();
}


- (void)dealloc
{
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    
    [super dealloc];
}

@end
