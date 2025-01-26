
#include "main.h"

int g_app_running = 1;

rSlide g_slides[2] = {0};

// this probably can be created by another program inside a folder and then the visualizer just reads the files inside it
const char* filepaths[] = {
	"test_slides/slide1.rslide",
	"test_slides/slide2.rslide"
};
unsigned int g_filepaths_size = (sizeof(filepaths) / sizeof(filepaths[0]));
unsigned long int g_performance_frequency = 0;
unsigned int g_slide_index = 0;
unsigned int g_shader_id = 0;

int main(void){
	rLogger_init(RLOG_FILE_MODE);
	
	RLOGGER_INFO("sizeof(rText): %lu bytes", sizeof(rText));
	RLOGGER_INFO("sizeof(rImage): %lu bytes", sizeof(rImage));
	RLOGGER_INFO("Slides count: %i slides", global_state.slide_count);
	RLOGGER_INFO("Filepath count: %i filepaths", g_filepaths_size);

	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	if(!init_app(&window, global_state.window_width, global_state.window_height, &context)){
		RLOGGER_ERROR("%s", "Something went wrong with the initialization...");
		return 1;
	}

	g_shader_id = shader_create("shaders/ui.vs", "shaders/ui.fs");
	if(g_shader_id == 0) {
		RLOGGER_ERROR("%s", "Couldn't create shader");
		g_app_running = 0;
	}
	shader_use(g_shader_id);
	
	while(g_app_running){
		handle_input(window);

		double start_ticks = SDL_GetPerformanceCounter();
		render_graphics(&window);
		
		double end_ticks = SDL_GetPerformanceCounter();
		double delta_ticks = end_ticks - start_ticks;
		double delta_time_us = (delta_ticks * 1000000) / g_performance_frequency;
		double delta_time_ms = delta_time_us / 1000;
		double raw_fps = 1000.0 / delta_time_ms;
		
		if (delta_time_ms < APP_DESIRED_FRAME_TIME_IN_MS) {
			double time_to_sleep = APP_DESIRED_FRAME_TIME_IN_MS - delta_time_ms;
			SDL_Delay(time_to_sleep);
		}else{
			RLOGGER_WARN("Frame time (%lf) longer than desired (%lf)", delta_time_ms, APP_DESIRED_FRAME_TIME_IN_MS);
		}

		double virtual_end_ticks = SDL_GetPerformanceCounter();
		double virtual_delta_ticks = virtual_end_ticks - start_ticks;
		double virtual_delta_time_us = (virtual_delta_ticks * 1000000) / g_performance_frequency;
		double virtual_delta_time_ms = virtual_delta_time_us / 1000;
		double virtual_fps = 1000.0 / virtual_delta_time_ms;
		RLOGGER_INFO("Delta Time (ms): %lf (%lf FPS RAW) - Virtual Delta Time (ms): %lf (%lf FPS VIRTUAL)", delta_time_ms, raw_fps, virtual_delta_time_ms, virtual_fps);
	}

	quit_app(&window, &context);
	return 0;
}

int init_app(SDL_Window** window, int width, int height, SDL_GLContext* gl_context){
	rLogger_init(RLOG_FILE_MODE);

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

	*window = SDL_CreateWindow(global_state.app_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, global_state.window_width, global_state.window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	RLOGGER_INFO("Vendor:   %s", glGetString(GL_VENDOR));
	RLOGGER_INFO("Renderer: %s", glGetString(GL_RENDERER));
	RLOGGER_INFO("Version:  %s", glGetString(GL_VERSION));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int parse_result = parse_rslide_files(g_slides, global_state.slide_count, filepaths);
	if(!parse_result){
		RLOGGER_ERROR("%s", "parse_rslide_files() has failed");
		return 0;
	}
	RLOGGER_INFO("%s", "Parsed .rslides");

	return 1;
}

void handle_input(SDL_Window* window){
	static char g_left_arrow_was_down = 0;
	static char g_right_arrow_was_down = 0;
	static char f_key_was_down = 0;
	static int window_is_fullscreen = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		const unsigned char* keyboardState = SDL_GetKeyboardState(NULL);

		// Handling quitting app by pressing ESC button
		g_app_running = !keyboardState[SDL_SCANCODE_ESCAPE];

		// input for transitioning slides
		char g_left_arrow_is_down = keyboardState[SDL_SCANCODE_LEFT];
		char g_right_arrow_is_down = keyboardState[SDL_SCANCODE_RIGHT];
		if(g_left_arrow_is_down && !g_left_arrow_was_down){
			if(--g_slide_index == 0) { g_slide_index = 0; }
		}else if(g_right_arrow_is_down && !g_right_arrow_was_down){
			if(++g_slide_index >= global_state.slide_count) { g_slide_index = global_state.slide_count - 1; }
		}
		g_left_arrow_was_down = g_left_arrow_is_down;
		g_right_arrow_was_down = g_right_arrow_is_down;

		// quitting app
		if(event.type == SDL_QUIT) { g_app_running = 0; }

		// handling fullscreen toggle by pressing the F key
		char f_key_is_down = keyboardState[SDL_SCANCODE_F];
		if(f_key_is_down && !f_key_was_down){
			window_is_fullscreen = !window_is_fullscreen;
			int fullscreen_toggle_result = SDL_SetWindowFullscreen(window, window_is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
			if(fullscreen_toggle_result < 0){
				RLOGGER_ERROR("Failed to toggle fullscreen mode with SDL_SetWindowFullscreen(): %s", SDL_GetError());
			}
		}
		f_key_was_down = f_key_is_down;

		// handling window resize
		if(event.type == SDL_WINDOWEVENT) { 
			if(event.window.event == SDL_WINDOWEVENT_RESIZED){
				int w, h;
				SDL_GetWindowSize(window, &w, &h);
				global_state.window_width = (unsigned int)w;
				global_state.window_height = (unsigned int)h;
				glViewport(0, 0, global_state.window_width, global_state.window_height);
			}
		}
	}
}

void render_graphics(SDL_Window** window){
	R_ASSERT(*window);

	rSlide slide = g_slides[g_slide_index];
	
	glClearColor(COLOR_HEX_TO_FLOATS(slide.background_color));
	glClear(GL_COLOR_BUFFER_BIT);

	// render images
	for(int i = 0; i < slide.image_array.length; i++){
		rImage* image_to_render = (rImage*)rdarray_at(&slide.image_array, i);
		render_image_as_quad(image_to_render);
	}

	// render texts
	for(int i = 0; i < slide.text_array.length; i++){
		rText* text_to_render = (rText*)rdarray_at(&slide.text_array, i);
		render_text_as_quad(text_to_render);
	}
	
	SDL_GL_SwapWindow(*window);
}

void quit_app(SDL_Window** window, SDL_GLContext* gl_context) {
	R_ASSERT(*window);
	R_ASSERT(*gl_context);

	free_rslides(g_slides, global_state.slide_count);
	RLOGGER_INFO("%s", "Deleted Slides");

	shader_delete(g_shader_id);
	RLOGGER_INFO("%s", "Deleted Shader");

	TTF_Quit();
	RLOGGER_INFO("%s", "Quit SDL_TTF");

	SDL_GL_DeleteContext(*gl_context);
	RLOGGER_INFO("%s", "Deleted OpenGL Context");

	SDL_DestroyWindow(*window);
	RLOGGER_INFO("%s", "Deleted OpenGL Window");

	SDL_Quit();
	RLOGGER_INFO("%s", "Quit SDL");

	rLogger_quit();
}

int parse_rslide_files(rSlide* slides, int slide_count, const char** filepaths) {
	R_ASSERT(global_state.slide_count == g_filepaths_size);
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

void render_image_as_quad(rImage* image){
	// can apply transformations here
	texture_bind(&image->texture_id);
	vao_bind(&image->vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	vao_unbind();
	texture_unbind();
}

void render_text_as_quad(rText* text){
	// can apply transformations here
	texture_bind(&text->texture_id);
	vao_bind(&text->vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	vao_unbind();
	texture_unbind();
}
