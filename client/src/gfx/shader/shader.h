#pragma once

#include <cglm/cglm.h>
#include <core/ext/sugar.h>
#include "shader_src.h"

typedef u32 Shader;

typedef enum
{
	SHADER_DEFAULT,
	SHADER_DEFAULT_INSTANCING,
	ShaderERRAIN
} ShaderType;

Shader shader_make(ShaderSrcSet *src_set);
void     shader_destroy(Shader shader);
void     shader_use(Shader shader);
void     shader_set_bool(Shader shader, const char *name, bool value);
void     shader_set_int(Shader shader, const char *name, int value);
void     shader_set_f32(Shader shader, const char *name, f32 value);
void     shader_set_vec2(Shader shader, const char *name, vec2 value);
void     shader_set_vec3(Shader shader, const char *name, vec3 value);
void     shader_set_vec4(Shader shader, const char *name, vec4 value);
void     shader_set_mat4(Shader shader, const char *name, mat4 value);
