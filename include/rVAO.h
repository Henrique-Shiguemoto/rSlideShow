#ifndef R_VAO_H
#define R_VAO_H

#include <glad/glad.h>
#include "rVBO.h"

void vao_create(unsigned int* vao_id);
void vao_define_vbo_layout(unsigned int* vbo_id, unsigned int layout_index, int size_in_components, unsigned int stride, unsigned int offset_in_components);
void vao_bind(unsigned int* vao_id);
void vao_unbind();
void vao_delete(unsigned int* vao_id);

#endif // R_VAO_H