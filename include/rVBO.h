#ifndef R_VBO_H
#define R_VBO_H

#include <glad/glad.h>

void vbo_create(unsigned int* vbo_id, float* data, unsigned int size_in_bytes, int dynamic_buffer);
void vbo_set(unsigned int* vbo_id, float* data, unsigned int size_in_bytes);
void vbo_bind(unsigned int* vbo_id);
void vbo_unbind();
void vbo_delete(unsigned int* vbo_id);

#endif // R_VBO_H