#include "iostream"
#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "GL/gl.h"

#define WND_WIDTH 800
#define WND_HEIGHT 600

int main()
{
    puts("bebra\n");
 
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* wnd = SDL_CreateWindow("launch", WND_WIDTH, WND_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_Renderer* ren = SDL_CreateRenderer(wnd, NULL);

    SDL_GLContext sdl_gl = SDL_GL_CreateContext(wnd);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0,WND_WIDTH,WND_HEIGHT);

    SDL_Event event;

    bool quit = false;
    
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_RenderClear(ren);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_RenderPresent(ren);

    }
    
    if(!SDL_GL_DestroyContext(sdl_gl))
    {
        std::cout << "Failed to destroy context. SDL says: \n" <<  SDL_GetError() << std::endl;
        return -1;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);

    puts("bebra2\n");
    return 0;
}