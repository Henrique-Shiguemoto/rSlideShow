#include "main.h"

int g_app_running = 1;

#define WINDOW_TITLE "rSlideShow"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SLIDE_COUNT 2
rSlide g_slides[SLIDE_COUNT] = {0};

// this probably can be created by another program inside a folder and then the visualizer just reads the files inside it
const char* filepaths[] = {
	"test_slides/slide1.rslide",
	"test_slides/slide2.rslide"
};
int g_filepaths_size = (sizeof(filepaths) / sizeof(filepaths[0]));

unsigned long int g_performance_frequency = 0;

int g_slide_index = 0;

int main(void){
	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	if(!init_app(&window, WINDOW_WIDTH, WINDOW_HEIGHT, &context)){
		RLOGGER_ERROR("%s", "Something went wrong with the initialization...");
		return 1;
	}

	rLogger_init(RLOG_TERMINAL_MODE);
	
	RLOGGER_INFO("sizeof(rText): %lu bytes", sizeof(rText));
	RLOGGER_INFO("sizeof(rImage): %lu bytes", sizeof(rImage));
	RLOGGER_INFO("Slides count: %i slides", SLIDE_COUNT);
	RLOGGER_INFO("Filepath count: %i filepaths", g_filepaths_size);

	parse_rslide_files(g_slides, SLIDE_COUNT, filepaths);

	// while(g_app_running){
	// 	handle_input();

	// 	double start_ticks = SDL_GetPerformanceCounter();
	// 	render_graphics(&window);
		
	// 	double end_ticks = SDL_GetPerformanceCounter();
	// 	double delta_ticks = end_ticks - start_ticks;
	// 	double delta_time_us = (delta_ticks * 1000000) / g_performance_frequency;
	// 	double delta_time_ms = delta_time_us / 1000;
	// 	double raw_fps = 1000.0 / delta_time_ms;
		
	// 	if (delta_time_ms < APP_DESIRED_FRAME_TIME_IN_MS) {
	// 		double time_to_sleep = APP_DESIRED_FRAME_TIME_IN_MS - delta_time_ms;
	// 		SDL_Delay(time_to_sleep);
	// 	}else{
	// 		RLOGGER_WARN("Frame time (%lf) longer than desired (%lf)", delta_time_ms, APP_DESIRED_FRAME_TIME_IN_MS);
	// 	}

	// 	double virtual_end_ticks = SDL_GetPerformanceCounter();
	// 	double virtual_delta_ticks = virtual_end_ticks - start_ticks;
	// 	double virtual_delta_time_us = (virtual_delta_ticks * 1000000) / g_performance_frequency;
	// 	double virtual_delta_time_ms = virtual_delta_time_us / 1000;
	// 	double virtual_fps = 1000.0 / virtual_delta_time_ms;
	// 	RLOGGER_INFO("Delta Time (ms): %lf (%lf FPS RAW) - Virtual Delta Time (ms): %lf (%lf FPS VIRTUAL)", delta_time_ms, raw_fps, virtual_delta_time_ms, virtual_fps);
	// }

	free_rslides(g_slides, SLIDE_COUNT);
	quit_app(&window, &context);
	return 0;
}

int init_app(SDL_Window** window, int width, int height, SDL_GLContext* gl_context){
	rLogger_init(RLOG_TERMINAL_MODE);

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		RLOGGER_ERROR("SDL_Init(): %s", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Initialized SDL2");

	if(TTF_Init() < 0){
		RLOGGER_ERROR("TTF_Init(): %s", TTF_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Initialized TTF");

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if(!(*window)){
		RLOGGER_ERROR("SDL_CreateWindow(): %s", SDL_GetError());
		return 0;
	}
	RLOGGER_INFO("%s", "Created SDL_Window");

	g_performance_frequency = SDL_GetPerformanceFrequency();

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
	static char g_left_arrow_was_down = 0;
	static char g_right_arrow_was_down = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);
		g_app_running = !keyboardState[SDL_SCANCODE_ESCAPE];
		char g_left_arrow_is_down = keyboardState[SDL_SCANCODE_LEFT];
		char g_right_arrow_is_down = keyboardState[SDL_SCANCODE_RIGHT];

		if(g_left_arrow_is_down && !g_left_arrow_was_down){
			if(--g_slide_index < 0) { g_slide_index = 0; }
		}else if(g_right_arrow_is_down && !g_right_arrow_was_down){
			if(++g_slide_index >= SLIDE_COUNT) { g_slide_index = SLIDE_COUNT - 1; }
		}

		if(event.type == SDL_QUIT) { g_app_running = 0; }

		g_left_arrow_was_down = g_left_arrow_is_down;
		g_right_arrow_was_down = g_right_arrow_is_down;
	}
}

void render_graphics(SDL_Window** window){
	R_ASSERT(*window);
	
	glClearColor(COLOR_HEX_TO_FLOATS(g_slides[g_slide_index].background_color));
	glClear(GL_COLOR_BUFFER_BIT);

	// TODO(Rick): render the contents of the current indexed rslide as just a normal for loop for images then for text
	
	SDL_GL_SwapWindow(*window);
}

void quit_app(SDL_Window** window, SDL_GLContext* gl_context) {
	R_ASSERT(*window);
	R_ASSERT(*gl_context);

	TTF_Quit();

	SDL_GL_DeleteContext(*gl_context);
	SDL_DestroyWindow(*window);
	SDL_Quit();
	rLogger_quit();
}

int parse_rslide_files(rSlide* slides, int slide_count, const char** filepaths) {
	R_ASSERT(SLIDE_COUNT == g_filepaths_size);
	R_ASSERT(slides);
	R_ASSERT(filepaths);

	for (int i = 0; i < slide_count; ++i) {
		slides[i] = rslide_create(filepaths[i]);
		if(slides[i].image_array.length == 0 && slides[i].text_array.length == 0 && slides[i].background_color == 0){
			return 0;
		}
	}

	return 1;
}

void free_rslides(rSlide* slides, int slide_count) {
	for (int i = 0; i < slide_count; ++i) {
		rslide_delete(&slides[i]);
	}
}
