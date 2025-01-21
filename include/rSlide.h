#ifndef R_SLIDE_H
#define R_SLIDE_H

#include "rInout.h"
#include "rStrlib.h"
#include "rDarray.h"
#include "rLogger.h"

typedef struct rImage {
	void* pixel_data;
	float x;
	float y;
	int width;
	int height;
	unsigned int vao_id;
	unsigned int vbo_id;
} rImage;

typedef struct rText {
	const char* text;
	float x;
	float y;
	int font_size;
	unsigned int color;
	unsigned int vao_id;
	unsigned int vbo_id;
} rText;

typedef struct rSlide {
	rdarray image_array;
	rdarray text_array;
	unsigned int background_color;
} rSlide;

rSlide rslide_create(const char* filepath);
void rslide_delete(rSlide* slide);

rText rtext_create(const char* text, float x, float y, int font_size, unsigned int color);
void rtext_delete(rText* text);

rImage rimage_create(const char* filepath, float x, float y, int width, int height);
void rimage_delete(rImage* image);

#endif // R_SLIDE_H 