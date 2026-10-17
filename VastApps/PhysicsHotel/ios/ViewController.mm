//
//  ViewController.m
//  test
//
//  Created by David Petrie on 15/04/13.
//  Copyright (c) 2013 David Petrie. SPDX-License-Identifier: MIT
//

#import "ViewController.h"

@interface ViewController ()
{
}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)dealloc
{
    [_context release];
    [_effect release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [view  setMultipleTouchEnabled:YES];
    
    self.preferredFramesPerSecond = 60;
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}



- (void) setClient:(hd::application::Client *)client
{
    m_client = client;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
            interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

}

#pragma mark - GLKView and GLKViewController delegate methods
static BOOL clientInitted = NO;
- (void)update
{
    if (clientInitted == NO)
    {
        m_client->Init();
        clientInitted = YES;
    }
    
    m_client->Update();
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{

    
    if (clientInitted)
    {
        
        m_client->Render();
    }
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!m_client) return;
 
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    int i = 0;
    for (UITouch *touch in [touches allObjects])
    {
        Vec3 location;
        float height = hd::Device::GetDisplayScreenHeight();
        location.x = [touch locationInView:touch.view].x;
        location.y = height - [touch locationInView:touch.view].y;
    
        eventQueue->OnPointerSelect(event.timestamp, i++, location);
    }
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!m_client) return;
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
 
    int i = 0;
    for (UITouch *touch in [touches allObjects])
    {
        Vec3 prevLocation, location, delta;
        float height = hd::Device::GetDisplayScreenHeight() ;
        prevLocation.x = [touch previousLocationInView:touch.view].x;
        prevLocation.y = height - [touch previousLocationInView:touch.view].y;
        location.x = [touch locationInView:touch.view].x;
        location.y = height - [touch locationInView:touch.view].y;
        
        delta.x = location.x - prevLocation.x;
        delta.y = location.y - prevLocation.y;
        delta.z = 0.0f;
    
        eventQueue->OnPointerDrag(event.timestamp, i++, location, delta);
    }
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    int i = 0;
    for (UITouch *touch in [touches allObjects])
    {
        Vec3 location;
        float height = hd::Device::GetDisplayScreenHeight();
        location.x = [touch locationInView:touch.view].x;
        location.y = height - [touch locationInView:touch.view].y;
        
        eventQueue->OnPointerRelease(event.timestamp, i++, location);
    }
}


@end
