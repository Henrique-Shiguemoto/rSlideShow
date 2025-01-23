#ifndef R_SHADER_H
#define R_SHADER_H

#include <glad/glad.h>

#include "rMathlib.h"
#include "rLogger.h"
#include "rInout.h"

unsigned int shader_create(const char* vertex_shader_path, const char* fragment_shader_path);
void shader_use(unsigned int shader_id);
void shader_delete(unsigned int shader_id);
void shader_set_bool_uniform(unsigned int shader_id, const char* uniform_name, int value);
void shader_set_int_uniform(unsigned int shader_id, const char* uniform_name, int value);
void shader_set_float_uniform(unsigned int shader_id, const char* uniform_name, float value);
void shader_set_vec2_uniform(unsigned int shader_id, const char* uniform_name, rm_v2f value);
void shader_set_vec3_uniform(unsigned int shader_id, const char* uniform_name, rm_v3f value);
void shader_set_vec4_uniform(unsigned int shader_id, const char* uniform_name, rm_v4f value);
void shader_set_mat2_uniform(unsigned int shader_id, const char* uniform_name, rm_mat2f value);
void shader_set_mat3_uniform(unsigned int shader_id, const char* uniform_name, rm_mat3f value);
void shader_set_mat4_uniform(unsigned int shader_id, const char* uniform_name, rm_mat4f value);

#endif // R_SHADER_H