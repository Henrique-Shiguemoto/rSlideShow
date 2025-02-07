#include "rVBO.h"

void vbo_create(unsigned int* vbo_id, float* data, unsigned int size_in_bytes, int dynamic_buffer){
	glGenBuffers(1, vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
	glBufferData(GL_ARRAY_BUFFER, size_in_bytes, data, dynamic_buffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void vbo_set(unsigned int* vbo_id, float* data, unsigned int size_in_bytes){
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_bytes, data);
}

void vbo_bind(unsigned int* vbo_id){
	glBindBuffer(GL_ARRAY_BUFFER, *vbo_id);
}

void vbo_unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(unsigned int* vbo_id){
	glDeleteBuffers(1, vbo_id);
}