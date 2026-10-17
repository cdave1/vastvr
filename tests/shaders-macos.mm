// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import <AppKit/AppKit.h>
#include <OpenGL/gl3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
static void check(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr,"FAIL: %s\n",msg); std::exit(1); } }
static GLuint compile(GLenum kind,const std::string& text,const char* name) {
 GLuint shader=glCreateShader(kind);const char* data=text.c_str();glShaderSource(shader,1,&data,nullptr);glCompileShader(shader);
 GLint success;glGetShaderiv(shader,GL_COMPILE_STATUS,&success);if(!success){char log[8192];glGetShaderInfoLog(shader,sizeof(log),nullptr,log);std::fprintf(stderr,"%s: %s\n",name,log);}check(success,name);return shader;
}
static std::string read(const char* path) {std::ifstream file(path);check(bool(file),path);std::ostringstream s;s<<file.rdbuf();return s.str();}
static GLuint program(const char* fragment) {
 // Matching varyings for focused compilation and rendering of the changed fragments.
 const char* vertex="#version 330\nout vec2 texCoord; out vec4 color; out vec4 position; out vec3 normal; out vec3 vertexPosition; out vec3 worldViewProjectionPosition; out float viewSpaceDistance; void main(){vec2 p=vec2((gl_VertexID<<1)&2,gl_VertexID&2); texCoord=p; gl_Position=vec4(p*2.0-1.0,0,1);color=vec4(1);position=vec4(0);normal=vec3(0,0,1);vertexPosition=vec3(0);worldViewProjectionPosition=vec3(0);viewSpaceDistance=0;}";
 GLuint v=compile(GL_VERTEX_SHADER,vertex,"test vertex");GLuint f=compile(GL_FRAGMENT_SHADER,read(fragment),fragment);GLuint p=glCreateProgram();glAttachShader(p,v);glAttachShader(p,f);glLinkProgram(p);GLint ok;glGetProgramiv(p,GL_LINK_STATUS,&ok);if(!ok){char log[8192];glGetProgramInfoLog(p,sizeof(log),nullptr,log);std::fprintf(stderr,"%s: %s\n",fragment,log);}check(ok,fragment);glDeleteShader(v);glDeleteShader(f);return p;
}
int main(){@autoreleasepool {
 NSOpenGLPixelFormatAttribute attrs[]={NSOpenGLPFAOpenGLProfile,NSOpenGLProfileVersion4_1Core,NSOpenGLPFAColorSize,24,NSOpenGLPFADepthSize,24,0};
 NSOpenGLPixelFormat* format=[[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];check(format!=nil,"OpenGL pixel format");NSOpenGLContext* context=[[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];[context makeCurrentContext];check(glGetString(GL_VERSION)!=nullptr,"OpenGL context");
 const char* root="VastApps/resources/shaders/gl3/";
 for(const char* name:{"galaxy.frag","huePicker.frag","postBlur.frag","filterShadow.frag","ambientObscurance.frag","shadowMapToEye.frag","blinnPhongGeometry.frag","blinnPhongTexture.frag"}){std::string path=std::string(root)+name;GLuint f=compile(GL_FRAGMENT_SHADER,read(path.c_str()),path.c_str());glDeleteShader(f);}
 GLuint target,fb,vao;glGenTextures(1,&target);glBindTexture(GL_TEXTURE_2D,target);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,1,12,0,GL_RGBA,GL_FLOAT,nullptr);glGenFramebuffers(1,&fb);glBindFramebuffer(GL_FRAMEBUFFER,fb);glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,target,0);check(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE,"float framebuffer");glGenVertexArrays(1,&vao);glBindVertexArray(vao);glViewport(0,0,1,12);
 GLuint hue=program("VastApps/resources/shaders/gl3/huePicker.frag");glUseProgram(hue);glUniform1f(glGetUniformLocation(hue,"Opacity"),0.75);glDrawArrays(GL_TRIANGLES,0,3);float pixels[48];glReadPixels(0,0,1,12,GL_RGBA,GL_FLOAT,pixels);
 for(int row=0;row<12;++row){double h=(row+0.5)*0.5;int sector=int(h);float f=float(h-sector);float expected[6][3]={{1,f,0},{1-f,1,0},{0,1,f},{0,1-f,1},{f,0,1},{1,0,1-f}};for(int channel=0;channel<3;++channel)check(std::fabs(pixels[row*4+channel]-expected[sector][channel])<0.0001,"hue sector interpolation");check(std::fabs(pixels[row*4+3]-0.75)<0.0001,"hue opacity");}
 GLuint input;glGenTextures(1,&input);glBindTexture(GL_TEXTURE_2D,input);float sample[]={0.2,0.4,0.6,1};glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,1,1,0,GL_RGBA,GL_FLOAT,sample);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
 GLuint blur=program("VastApps/resources/shaders/gl3/postBlur.frag");glUseProgram(blur);glUniform1i(glGetUniformLocation(blur,"iShadowTexture"),0);glUniform2f(glGetUniformLocation(blur,"uTextureSize"),1,1);
 for(int width:{0,1,4,16,100}){glUniform1i(glGetUniformLocation(blur,"uBlurSize"),width);glDrawArrays(GL_TRIANGLES,0,3);glReadPixels(0,0,1,1,GL_RGBA,GL_FLOAT,pixels);for(int c=0;c<4;++c)check(std::isfinite(pixels[c])&&std::fabs(pixels[c]-sample[c])<0.0001,"box blur preserves constant colors at clamped sizes");}
 check(glGetError()==GL_NO_ERROR,"OpenGL verification errors");std::puts("Eight changed GL3 fragments compiled; hue interpolation and blur boundaries passed GPU rendering checks.");
}}
