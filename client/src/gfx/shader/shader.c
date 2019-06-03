#include <core/ext/sugar.h>
#include <gl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <core/ext/logger.h>
#include <core/ext/util.h>
#include "shader.h"
#include "shader_module.h"

/*============================*/
/* Internal methods and funcs */
/*============================*/
static void
err_check(GLuint id, const char *type)
{
	GLint success;
	GLchar log[1024];
	if(strcmp(type, "PROGRAM") != 0)
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(id, 1024, NULL, log);
			printf("Shader compile error (%s): \n%s\n", type, log);
		}
	}
	else
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(id, 1024, NULL, log);
			printf("Shader linking error (%s): \n%s\n", type, log);
		}
	}
}

/*==========================*/
/* Public methods and funcs */
/*==========================*/
Shader
shader_make(ShaderSrcSet *src_set)
{
	GLuint v_id = 0;
	GLuint f_id = 0;		

	v_id = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(v_id, src_set->vertex_src->count, (const char**)src_set->vertex_src->srcs, NULL);
	glCompileShader(v_id);
	err_check(v_id, "VERTEX");

	f_id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(f_id, src_set->fragment_src->count,(const char**)src_set->fragment_src->srcs, NULL);
	glCompileShader(f_id);
	err_check(f_id, "FRAGMENT");

	GLuint id = glCreateProgram();
	glAttachShader(id, v_id);
	glAttachShader(id, f_id);

	glLinkProgram(id);
	err_check(id, "PROGRAM");

	glDeleteShader(v_id);
	glDeleteShader(f_id);

	return id;
}

void
shader_destroy(Shader shader)
{
	glDeleteProgram(shader);
}

void
shader_use(Shader shader)
{
	glUseProgram(shader);
	shader_module_set_active_shader(shader);
}

void 
shader_set_bool(Shader shader, const char *name, bool value)
{
	glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void 
shader_set_int(Shader shader, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name), value);
}

void 
shader_set_float(Shader shader, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(shader, name), value);
}

void 
shader_set_vec2(Shader shader, const char *name, vec2 value)
{
	glUniform2fv(glGetUniformLocation(shader, name), 1, value);
}

void 
shader_set_vec3(Shader shader, const char *name, vec3 value)
{
	glUniform3fv(glGetUniformLocation(shader, name), 1, value);
}

void 
shader_set_vec4(Shader shader, const char *name, vec4 value)
{
	glUniform4fv(glGetUniformLocation(shader, name), 1, value);
}

void 
shader_set_mat4(Shader shader, const char *name, mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, value[0]);
}



