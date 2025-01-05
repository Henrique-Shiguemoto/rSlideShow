#include "main.h"

int g_should_quit = 0;

#define WINDOW_TITLE "rSlideShow"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SLIDE_COUNT 1
rSlide g_slides[SLIDE_COUNT] = {0};

int main(void){
	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	if(!init_everything(&window, WINDOW_WIDTH, WINDOW_HEIGHT, &context)){
		RLOGGER_ERROR("%s", "Something went wrong with the initialization...");
		return 1;
	}

	// TODO(Rick): Call rslide_create for all test_slides
	// TODO(Rick): Fill g_slides with parsed rSlide structs
	// TODO(Rick): After g_slides is filled correctly, figure out a way to render it

	while(!g_should_quit){
		handle_input();
		render_graphics(&window);
	}

	quit_everything(&window, &context);
	return 0;
}

int init_everything(SDL_Window** window, int width, int height, SDL_GLContext* gl_context){
	rLogger_init(RLOG_TERMINAL_MODE);

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		RLOGGER_ERROR("SDL_Init(): %s", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Initialized SDL2");

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(!(*window)){
		RLOGGER_ERROR("SDL_CreateWindow(): %s", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Created SDL_Window");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	*gl_context = SDL_GL_CreateContext(*window);
	if(!(*gl_context)){
		RLOGGER_ERROR("SDL_GL_CreateContext(): %s", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Created OpenGL context");

	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
		RLOGGER_ERROR("gladLoadGLLoader() Couldn't initialize GLAD and load OpenGL function pointers", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Initialized GLAD and loaded OpenGL functions");

	return 1;
}

void handle_input(){
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
}

void render_graphics(SDL_Window** window){
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	SDL_GL_SwapWindow(*window);
}

void quit_everything(SDL_Window** window, SDL_GLContext* gl_context) {
	SDL_GL_DeleteContext(*gl_context);
	SDL_DestroyWindow(*window);
	SDL_Quit();
	rLogger_quit();
}