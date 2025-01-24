#include "rVAO.h"

void vao_create(unsigned int* vao_id){
	glGenVertexArrays(1, vao_id);
}

void vao_define_vbo_layout(unsigned int* vbo_id, unsigned int layout_index, int size_in_components, unsigned int stride, unsigned int offset_in_components){
	vbo_bind(vbo_id);
	glVertexAttribPointer(layout_index, size_in_components, GL_FLOAT, GL_FALSE, stride, (void*)(offset_in_components * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(layout_index);
	vbo_unbind(vbo_id);
}

void vao_bind(unsigned int* vao_id){
	glBindVertexArray(*vao_id);
}

void vao_unbind(){
	glBindVertexArray(0);
}

void vao_delete(unsigned int* vao_id){
	glDeleteVertexArrays(1, vao_id);
}
