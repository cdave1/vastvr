#include <SDL2/SDL.h>
#include "core/application/Client.h"

int main(int argc, char ** argv) {
    
    hd::application::Client *client = new hd::application::Client();
    // variables
    
    bool quit = false;
    SDL_Event event;
    int x = 288;
    int y = 208;
    
    // init SDL
    
    
    SDL_Window *window;
    SDL_GLContext maincontext;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    window = SDL_CreateWindow("SDL2 Keyboard/Mouse events",
                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
   maincontext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);
    // handle events
    
    while (!quit)
    {
        SDL_WaitEvent(&event);
        
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
        
        client->Update();
        client->Render();
        SDL_GL_SwapWindow(window);
    }
    
    // cleanup SDL
    client->WillShutdown();
	client->Shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}