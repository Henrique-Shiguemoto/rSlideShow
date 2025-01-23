#include "rVBO.h"

void vbo_create(unsigned int* vbo_id, float* data, unsigned int size_in_bytes){
	glGenBuffers(1, vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
	glBufferData(GL_ARRAY_BUFFER, size_in_bytes, data, GL_STATIC_DRAW);
}

void vbo_bind(unsigned int* vbo_id){
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
}

// I know vbo_id isn't even used here, it's ok
void vbo_unbind(unsigned int* vbo_id){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(unsigned int* vbo_id){
	glDeleteBuffers(1, vbo_id);
}