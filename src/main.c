#include <stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "glad/glad.h"

#define WINDOW_TITLE "rSlideShow"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window* g_window = NULL;
SDL_GLContext g_context = NULL;
int g_should_quit = 0;

int main(void){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Error SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	g_window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(!g_window){
		printf("Error SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}

	// I'm currently using RenderDoc for graphics debugging and it is important that the code is in this order to avoid errors in RenderDoc, 
	// 		SDL_Init => SDL_CreateWindow => SDL_GL_SetAttribute => SDL_GL_CreateContext.
	// If it's not in this order, RenderDoc will show an error (at least on my machine) along the lines of "Context not created by CreateContextAttribs. Captures Disabled."
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	g_context = SDL_GL_CreateContext(g_window);
	if(!g_context){
		printf("Error SDL_GL_CreateContext(): %s\n", SDL_GetError());
		return -1;
	}

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		printf("Error gladLoadGLLoader(): Couldn't initialize GLAD and load OpenGL function pointers\n");
		return -1;
	}

	while(!g_should_quit){
		// handling events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_QUIT: {
					g_should_quit = 1;
					break;
				}
				default: {
					break;
				}
			}
		}

		// simulate 

		// render
		glClearColor(0.001f, 0.02f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		SDL_GL_SwapWindow(g_window);
	}

	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
	return 0;
}
