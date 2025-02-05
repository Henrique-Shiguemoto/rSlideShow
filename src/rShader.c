#include "rShader.h"

unsigned int shader_create(const char* vertex_shader_path, const char* fragment_shader_path){
	R_ASSERT(vertex_shader_path != NULL && fragment_shader_path != NULL);
	
	rio_file* vertex_shader_file = rio_open_file(vertex_shader_path, RIO_READ_MODE);
	rio_file* fragment_shader_file = rio_open_file(fragment_shader_path, RIO_READ_MODE);

	if(!vertex_shader_file) {
		RLOGGER_ERROR("Couldn't load file: %s", vertex_shader_path);
		return 0;
	}

	if(!fragment_shader_file) {
		RLOGGER_ERROR("Couldn't load file: %s", fragment_shader_path);
		return 0;
	}

	unsigned int vertexShaderID, fragmentShaderID;
	int success;
	char infoLog[512];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, (char const * const *)&(vertex_shader_file->contents), NULL);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to compile vertex shader (%s): %s", vertex_shader_path, infoLog);
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (char const * const *)&(fragment_shader_file->contents), NULL);
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to compile fragment shader (%s): %s", fragment_shader_path, infoLog);
	}

	unsigned int shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertexShaderID);
	glAttachShader(shader_program_id, fragmentShaderID);
	glLinkProgram(shader_program_id);
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shader_program_id, 512, NULL, infoLog);
		RLOGGER_WARN("Failed to link shader program: %s", infoLog);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return shader_program_id;
}

void shader_use(unsigned int shader_id) { 
	glUseProgram(shader_id); 
}

void shader_delete(unsigned int shader_id) { 
	glDeleteProgram(shader_id); 
}

void shader_set_bool_uniform(unsigned int shader_id, const char* uniform_name, int value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform1i(uniform_location, value); 
}

void shader_set_int_uniform(unsigned int shader_id, const char* uniform_name, int value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform1i(uniform_location, value); 
}

void shader_set_float_uniform(unsigned int shader_id, const char* uniform_name, float value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform1f(uniform_location, value); 
}

void shader_set_vec2_uniform(unsigned int shader_id, const char* uniform_name, rm_v2f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform2fv(uniform_location, 1, (GLfloat*)&value); 
}

void shader_set_vec3_uniform(unsigned int shader_id, const char* uniform_name, rm_v3f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform3fv(uniform_location, 1, (GLfloat*)&value); 
}

void shader_set_vec4_uniform(unsigned int shader_id, const char* uniform_name, rm_v4f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniform4fv(uniform_location, 1, (GLfloat*)&value); 
}

void shader_set_mat2_uniform(unsigned int shader_id, const char* uniform_name, rm_mat2f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniformMatrix2fv(uniform_location, 1, GL_FALSE, &(value.elem[0])); 
}

void shader_set_mat3_uniform(unsigned int shader_id, const char* uniform_name, rm_mat3f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniformMatrix3fv(uniform_location, 1, GL_FALSE, &(value.elem[0])); 
}

void shader_set_mat4_uniform(unsigned int shader_id, const char* uniform_name, rm_mat4f value) { 
	int uniform_location = glGetUniformLocation(shader_id, uniform_name);
	if(uniform_location < 0){
		RLOGGER_WARN("Uniform with name \"%s\" not found", uniform_name);
		return;
	}
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &(value.elem[0])); 
}
