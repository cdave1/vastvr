#import "AppView.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import <OpenGL/glu.h>

using namespace hd;


@implementation AppView
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;




#define BUFFER_OFFSET(i) ((void*)(i))





//Vertex, tex0
//
//SIZE : 4+4+4 +4+4 = 4*6 = 20 bytes
//It's better to make it multiple of 32
//32-20 = 12 bytes (of garbage should be added)
//12/4 = 3 floats should be added
struct TVertex_VT
{
    float   x, y, z;
    float   s0, t0;
    float   padding[3];
};

//Vertex, normal, tex0
//
//SIZE : 4+4+4 +4+4+4 +4+4 = 4*8 = 32 bytes
struct TVertex_VNT
{
    float   x, y, z;
    float   nx, ny, nz;
    float   s0, t0;
};

//Vertex, color
//
//SIZE : 4+4+4 +4 = 4*4 = 16 bytes
//It's better to make it multiple of 32
//32-16 = 16 bytes (of garbage should be added)
//16/4 = 4 floats should be added
struct TVertex_VC
{
    float   x, y, z;
    unsigned int    color;
    float   padding[4];
};




//Globals

//A quad
GLushort    pindex_quad[6];
TVertex_VC  pvertex_quad[4];

//A triangle
GLushort        pindex_triangle[3];
TVertex_VNT     pvertex_triangle[3];

//1 VAO for the quad
//1 VAO for the triangle
GLuint VAOID[2];
//1 IBO for the quad (Index Buffer Object)
//1 IBO for the triangle
GLuint IBOID[2];
//1 IBO for the quad (Vertex Buffer Object)
//1 IBO for the triangle
GLuint VBOID[2];

//1 shader for the quad
//1 shader for the triangle
GLuint  ShaderProgram[2];
GLuint  VertexShader[2];
GLuint  FragmentShader[2];

int ProjectionModelviewMatrix_Loc[2];       //The location of ProjectionModelviewMatrix in the shaders

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
std::string loadFile(const char *fname)
{
    std::ifstream file(fname);
    if(!file.is_open())
    {
        cout << "Unable to open file " << fname << endl;
        exit(1);
    }

    std::stringstream fileData;
    fileData << file.rdbuf();
    file.close();

    return fileData.str();
}


// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0)
    {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        cout << "InfoLog : " << endl << infoLog << endl;
        delete [] infoLog;
    }
}


void InitGLStates()
{
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);/*
    glReadBuffer(GL_BACK);
    glDrawBuffer(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(TRUE);
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFFFFFFFF);
    glStencilFunc(GL_EQUAL, 0x00000000, 0x00000001);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);*/
    glClearColor(1.0, 0.0, 0.0, 0.0);/*
    glClearDepth(1.0);
    glClearStencil(0);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);
    glActiveTexture(GL_TEXTURE0);*/
}



int LoadShader(const char *pfilePath_vs, const char *pfilePath_fs, bool bindTexCoord0, bool bindNormal, bool bindColor, GLuint &shaderProgram, GLuint &vertexShader, GLuint &fragmentShader)
{
    shaderProgram=0;
    vertexShader=0;
    fragmentShader=0;

    // load shaders & get length of each
    int vlen;
    int flen;
    std::string vertexShaderString = loadFile(pfilePath_vs);
    std::string fragmentShaderString = loadFile(pfilePath_fs);
    vlen = vertexShaderString.length();
    flen = fragmentShaderString.length();

    if(vertexShaderString.empty())
    {
        return -1;
    }

    if(fragmentShaderString.empty())
    {
        return -1;
    }

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertexShaderCStr = vertexShaderString.c_str();
    const char *fragmentShaderCStr = fragmentShaderString.c_str();
    glShaderSource(vertexShader, 1, (const GLchar **)&vertexShaderCStr, &vlen);
    glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentShaderCStr, &flen);

    GLint compiled;

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if(compiled==FALSE)
    {
        cout << "Vertex shader not compiled." << endl;
        printShaderInfoLog(vertexShader);

        glDeleteShader(vertexShader);
        vertexShader=0;
        glDeleteShader(fragmentShader);
        fragmentShader=0;

        return -1;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if(compiled==FALSE)
    {
        cout << "Fragment shader not compiled." << endl;
        printShaderInfoLog(fragmentShader);

        glDeleteShader(vertexShader);
        vertexShader=0;
        glDeleteShader(fragmentShader);
        fragmentShader=0;

        return -1;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //glBindAttribLocation(shaderProgram, 0, "InVertex");

   // if(bindTexCoord0)
   //     glBindAttribLocation(shaderProgram, 1, "InTexCoord0");

    //if(bindNormal)
    //    glBindAttribLocation(shaderProgram, 2, "InNormal");

    //if(bindColor)
     //   glBindAttribLocation(shaderProgram, 3, "InColor");

    glLinkProgram(shaderProgram);

    GLint IsLinked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==FALSE)
    {
        cout << "Failed to link shader." << endl;

        GLint maxLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        if(maxLength>0)
        {
            char *pLinkInfoLog = new char[maxLength];
            glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, pLinkInfoLog);
            cout << pLinkInfoLog << endl;
            delete [] pLinkInfoLog;
        }

        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
        glDeleteShader(vertexShader);
        vertexShader=0;
        glDeleteShader(fragmentShader);
        fragmentShader=0;
        glDeleteProgram(shaderProgram);
        shaderProgram=0;

        return -1;
    }

    return 1;       //Success
}

void CreateGeometry()
{
    //A quad
    pvertex_quad[0].x=-0.8f;
    pvertex_quad[0].y=-0.5f;
    pvertex_quad[0].z=0;
    pvertex_quad[0].color=0xFFFFFFFF;

    pvertex_quad[1].x=0.0f;
    pvertex_quad[1].y=-0.5f;
    pvertex_quad[1].z=0;
    pvertex_quad[1].color=0xFFFF0000;

    pvertex_quad[2].x=-0.8f;
    pvertex_quad[2].y=0.5f;
    pvertex_quad[2].z=0;
    pvertex_quad[2].color=0xFF00FF00;

    pvertex_quad[3].x=0.0f;
    pvertex_quad[3].y=0.5f;
    pvertex_quad[3].z=0;
    pvertex_quad[3].color=0xFF0000FF;

    pindex_quad[0]=0;
    pindex_quad[1]=1;
    pindex_quad[2]=2;
    pindex_quad[3]=2;
    pindex_quad[4]=1;
    pindex_quad[5]=3;

    //The triangle
    pvertex_triangle[0].x=0.0f;
    pvertex_triangle[0].y=0.5f;
    pvertex_triangle[0].z=-1.0f;
    pvertex_triangle[0].nx=0.0f;
    pvertex_triangle[0].ny=0.0f;
    pvertex_triangle[0].nz=1.0f;
    pvertex_triangle[0].s0=0.0f;
    pvertex_triangle[0].t0=0.0f;

    pvertex_triangle[1].x=0.3f;
    pvertex_triangle[1].y=-0.5f;
    pvertex_triangle[1].z=-1.0f;
    pvertex_triangle[1].nx=0.0f;
    pvertex_triangle[1].ny=0.0f;
    pvertex_triangle[1].nz=1.0f;
    pvertex_triangle[1].s0=1.0f;
    pvertex_triangle[1].t0=0.0f;

    pvertex_triangle[2].x=0.8f;
    pvertex_triangle[2].y=0.5f;
    pvertex_triangle[2].z=-1.0f;
    pvertex_triangle[2].nx=0.0f;
    pvertex_triangle[2].ny=0.0f;
    pvertex_triangle[2].nz=1.0f;
    pvertex_triangle[2].s0=0.5f;
    pvertex_triangle[2].t0=1.0f;

    pindex_triangle[0]=0;
    pindex_triangle[1]=1;
    pindex_triangle[2]=2;


    //Create the IBO for the quad
    //16 bit indices
    glGenBuffers(1, &IBOID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), pindex_quad, GL_STATIC_DRAW);

    GLenum error=glGetError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    error=glGetError();

    //Create VBO for the quad
    glGenBuffers(1, &VBOID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(TVertex_VC), pvertex_quad, GL_STATIC_DRAW);

    error=glGetError();

    //Just testing
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    //VAO for the quad *********************
    glGenVertexArrays(1, &VAOID[0]);
   // glBindVertexArray(VAOID[0]);


    //Bind the IBO for the VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);

    //Just testing
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);







    //Create the IBO for the triangle
    //16 bit indices
    //We could have actually made one big IBO for both the quad and triangle.
    glGenBuffers(1, &IBOID[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLushort), pindex_triangle, GL_STATIC_DRAW);

    error=glGetError();

    //Create VBO for the triangle
    glGenBuffers(1, &VBOID[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOID[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(TVertex_VNT), pvertex_triangle, GL_STATIC_DRAW);

    //Just testing
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    error=glGetError();

    //Second VAO setup *******************
    //This is for the triangle
    glGenVertexArrays(1, &VAOID[1]);
    //glBindVertexArray(VAOID[1]);


    
    //Bind the IBO for the VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[1]);
    
    //Just testing
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void attr() {
    glBindVertexArray(VAOID[0]);

    //Bind the VBO and setup pointers for the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[0]);
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex_VC), BUFFER_OFFSET(0));
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TVertex_VC), BUFFER_OFFSET(sizeof(float)*3));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




    glBindVertexArray(VAOID[1]);
    //Bind the VBO and setup pointers for the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBOID[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID[1]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex_VNT), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex_VNT), BUFFER_OFFSET(sizeof(float)*3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex_VNT), BUFFER_OFFSET(sizeof(float)*6));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void display()
{
    float projectionModelviewMatrix[16];

    //Just set it to identity matrix
    memset(projectionModelviewMatrix, 0, sizeof(float)*16);
    projectionModelviewMatrix[0]=1.0;
    projectionModelviewMatrix[5]=1.0;
    projectionModelviewMatrix[10]=1.0;
    projectionModelviewMatrix[15]=1.0;

    //Clear all the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    attr();
    //Bind the shader that we want to use
    glUseProgram(ShaderProgram[0]);
    //Setup all uniforms for your shader
    glUniformMatrix4fv(ProjectionModelviewMatrix_Loc[0], 1, FALSE, projectionModelviewMatrix);
    //Bind the VAO
    glBindVertexArray(VAOID[0]);
    //At this point, we would bind textures but we aren't using textures in this example

    //Draw command
    //The first to last vertex is 0 to 3
    //6 indices will be used to render the 2 triangles. This make our quad.
    //The last parameter is the start address in the IBO => zero
    glDrawRangeElements(GL_TRIANGLES, 0, 3, 6, GL_UNSIGNED_SHORT, NULL);

    //Bind the shader that we want to use
    glUseProgram(ShaderProgram[1]);
    //Setup all uniforms for your shader
    glUniformMatrix4fv(ProjectionModelviewMatrix_Loc[1], 1, FALSE, projectionModelviewMatrix);
    //Bind the VAO
    glBindVertexArray(VAOID[1]);
    //At this point, we would bind textures but we aren't using textures in this example

    //Draw command
    //The first to last vertex is 0 to 3
    //3 indices will be used to render 1 triangle.
    //The last parameter is the start address in the IBO => zero
    glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, NULL);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ExitFunction(int value)
{
    cout<<"Exit called."<<endl;

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    glDeleteBuffers(1, &IBOID[0]);
    glDeleteBuffers(1, &IBOID[1]);
    glDeleteBuffers(1, &VBOID[0]);
    glDeleteBuffers(1, &IBOID[1]);
    glDeleteVertexArrays(1, &VAOID[0]);
    glDeleteVertexArrays(1, &VAOID[1]);

    glDetachShader(ShaderProgram[0], VertexShader[0]);
    glDetachShader(ShaderProgram[0], FragmentShader[0]);
    glDeleteShader(VertexShader[0]);
    glDeleteShader(FragmentShader[0]);
    glDeleteProgram(ShaderProgram[0]);

    glDetachShader(ShaderProgram[1], VertexShader[1]);
    glDetachShader(ShaderProgram[1], FragmentShader[1]);
    glDeleteShader(VertexShader[1]);
    glDeleteShader(FragmentShader[1]);
    glDeleteProgram(ShaderProgram[1]);
}
- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format {
    self = [super initWithFrame:frameRect pixelFormat:format];

    return self;
}


- (void)prepareOpenGL {

    // this sets swap interval for double buffering
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    [[self window] setAcceptsMouseMovedEvents:YES];

    timer = [NSTimer timerWithTimeInterval:0.001
                                    target:self
                                  selector:@selector(timerEvent:)
                                  userInfo:nil
                                   repeats:YES];

    updateTimer = [NSTimer timerWithTimeInterval:0.001
                                          target:self
                                        selector:@selector(updateTimerEvent:)
                                        userInfo:nil
                                         repeats:YES];

    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
    [[NSRunLoop mainRunLoop] addTimer:updateTimer forMode:NSDefaultRunLoopMode];

    InitGLStates();

    std::string vert1 = std::string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/testShader1.vert";
    std::string frag1 = std::string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/testShader1.frag";
    std::string vert2 = std::string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/testShader2.vert";
    std::string frag2 = std::string([[[NSBundle mainBundle] resourcePath] UTF8String]) + "/testShader2.frag";

    if(LoadShader(vert1.c_str(), frag1.c_str(), false, false, true, ShaderProgram[0], VertexShader[0], FragmentShader[0])==-1)
    {
        exit(1);
    }
    else
    {
        ProjectionModelviewMatrix_Loc[0]=glGetUniformLocation(ShaderProgram[0], "ProjectionModelviewMatrix");
    }

    if(LoadShader(vert2.c_str(), frag2.c_str(), true, true, false, ShaderProgram[1], VertexShader[1], FragmentShader[1])==-1)
    {
        exit(1);
    }
    else
    {
        ProjectionModelviewMatrix_Loc[1]=glGetUniformLocation(ShaderProgram[1], "ProjectionModelviewMatrix");
    }

    CreateGeometry();
}


- (void)timerEvent:(NSTimer *)t {
    //[self setNeedsDisplay:YES];
    [renderProxy update];
    [self render];
}


- (void) updateTimerEvent:(NSTimer *)t {
    //[self setNeedsDisplay:YES];
}


- (BOOL)acceptsFirstResponder
{
    return NO;
}
- (BOOL)becomeFirstResponder
{
    return YES;
}
///-(BOOL)acceptsFirstResponder { return YES; }


- (void) setRenderProxy:(id)proxy {
    renderProxy = proxy;
}


- (void) setClient:(hd::application::Client *)client {
    m_client = client;
}


- (void) render {
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
    backingPixelHeight = (GLsizei)(backingBounds.size.height);

    // Set viewport
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);

    display();

    [[self openGLContext] flushBuffer];
}


static bool r = false;
- (void) drawRect: (NSRect) bounds {
    [self render];
}


- (void)keyDown:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnKeySelect(0, unicodeKey);
}


- (void)keyUp:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnKeyRelease(0, unicodeKey);
}


- (void)mouseDown:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
	if (event.type == NSLeftMouseDown) {
        eventQueue->OnPointerSelect(0, location);
	} else if (event.type == NSRightMouseDown) {
        eventQueue->OnPointerSelect(1, location);
	}
}


- (void)mouseUp:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(0, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(1, location);
    }
}


- (void) mouseEntered:(NSEvent *)theEvent {
    
}


- (void) mouseExited:(NSEvent *)theEvent {
    
}


- (void)mouseMoved:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);

    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);

    if (event.type == NSMouseMoved) {
        eventQueue->OnPointerMove(0, location, delta);
    }
    
    
    /*
	if (theEvent.type == NSMouseMoved)
	{
        NSLog(@"Mouse moved");
        
		mouseValues.previousLocation.Set(mouseValues.currentLocation.x,
                                         mouseValues.currentLocation.y);
        
		mouseValues.currentLocation.Set(theEvent.locationInWindow.x,
                                        theEvent.locationInWindow.y);
		
		if (mouseValues.leftButton.clickState == clickstate_down)
		{
			mouseValues.leftButton.clickState = clickstate_moving;
		}
		
		if (mouseValues.rightButton.clickState == clickstate_down)
		{
			mouseValues.rightButton.clickState = clickstate_moving;
		}
	}*/
}


- (void)mouseDragged:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);
    
    if (event.type == NSLeftMouseDragged) {
        eventQueue->OnPointerDrag(0, location, delta);
    } else if (event.type == NSRightMouseDragged) {
        eventQueue->OnPointerDrag(1, location, delta);
    }
}


- (void)scrollWheel:(NSEvent *)theEvent {
	if (theEvent.type == NSScrollWheel) {
        NSLog(@"Scroll Wheel");
        
        //mouseValues.scrollWheel.clickState = clickstate_moving;
		//mouseValues.scrollWheel.xClickDown = [theEvent deltaX];
		//mouseValues.scrollWheel.yClickDown = [theEvent deltaY];
		
		//printf("%3.2f, %3.2f\n", [theEvent deltaX], [theEvent deltaY]);
	}
}


/*
 * Cleanup
 */
- (void) dealloc {
	[super dealloc];
}
@end
