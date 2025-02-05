#ifndef R_GLOBAL_H
#define R_GLOBAL_H

typedef struct rGlobal {
	const char* app_name;
	float window_width;
	float window_height;
	int slide_count;
	const int console_text_buffer_max_size;
} rGlobal;

extern rGlobal global_state;

#endif // R_GLOBAL_H