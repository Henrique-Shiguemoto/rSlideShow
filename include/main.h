#ifndef MAIN_H
#define MAIN_H

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <glad/glad.h>

#include "rLogger.h"
#include "rDarray.h"
#include "rShader.h"
#include "rSlide.h"
#include "rTexture.h"
#include "rStrlib.h"

#define UNDEFINEED_GLBUFFER_VALUE 0xB01DFACE
#define APP_DESIRED_FRAME_TIME_IN_MS (double)16.67
#define COLOR_HEX_TO_FLOATS(_color) (float)(((_color) >> 24) & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 16) & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 8)  & 0x000000FF) / 255.0f, \
									(float)(((_color) >> 0)  & 0x000000FF) / 255.0f

int init_app(SDL_Window** window, int width, int height, SDL_GLContext* gl_context);
void handle_input(SDL_Window* window);
void render_graphics(SDL_Window** window);
void render_entity(rEntity* entity);
void render_console();
void render_image_as_quad(rImage* image);
void render_text_as_quad(rText* text);
void quit_app(SDL_Window** window, SDL_GLContext* gl_context);

int parse_rslide_files(rSlide* slides, int slide_count, const char** filepaths);
void free_rslides(rSlide* slides, int slide_count);

#endif // MAIN_H