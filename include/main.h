#ifndef MAIN_H
#define MAIN_H

#include "rSlide.h"
#include "rLogger.h"
#include "rDarray.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "SDL2/SDL_ttf.h"

#define APP_DESIRED_FRAME_TIME_IN_MS (double)16.67
#define COLOR_HEX_TO_FLOATS(_color) (float)(((_color) >> 24) & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 16) & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 8)  & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 0)  & 0x000000FF) / 255.0f

#include "glad/glad.h"

int init_app(SDL_Window** window, int width, int height, SDL_GLContext* gl_context);
void handle_input();
void render_graphics(SDL_Window** window);
void quit_app(SDL_Window** window, SDL_GLContext* gl_context);

int parse_rslide_files(rSlide* slides, int slide_count, const char** filepaths);
void free_rslides(rSlide* slides, int slide_count);

#endif // MAIN_H