#ifndef R_SLIDE_H
#define R_SLIDE_H

typedef struct rImage {
	void* pixel_data;
	int x;
	int y;
	int width;
	int height;
} rImage;

typedef struct rText {
	const char* text;
	int x;
	int y;
	int font_size;
	unsigned int color;
} rText;

typedef struct rSlide {
	rImage* image_array; 		// dynamic array
	rText* text_array; 			// dynamic array
	unsigned int background_color;
} rSlide;

// TODO(Rick): Implement these functions

rSlide* rslide_create(const char* filepath);
void rslide_delete(rSlide* slide);
rText* rtext_create(const char* text, int x, int y, int font_size, unsigned int color);
void rtext_delete(rText* text);
rImage* rimage_create(const char* filepath, int x, int y, int width, int height);
void rimage_delete(rImage* image);

#endif // R_SLIDE_H 