#ifndef MAIN_H
#define MAIN_H

#include "r_slide.h"
#include "r_logger.h"
#include "r_darray.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define APP_DESIRED_FRAME_TIME_IN_MS (double)16.67

#define r_assert(x) do { if(!(x)) { RLOGGER_ERROR("%s assertion failed! (%s:%i)", (#x), __FILE__, __LINE__); *(char*)0 = 0; } } while(0)

#include "glad/glad.h"

int init_app(SDL_Window** window, int width, int height, SDL_GLContext* gl_context);
void handle_input();
void render_graphics(SDL_Window** window);
void quit_app(SDL_Window** window, SDL_GLContext* gl_context);
int parse_rslide_files(rSlide* slides, int slide_count, const char** filepaths);
void free_rslides(rSlide* slides, int slide_count);

#endif // MAIN_H