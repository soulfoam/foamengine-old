#pragma once

#include <string.h>
#include <core/ext/sugar.h>
#include <core/ext/vector.h>

typedef struct
{
	char **srcs;
	u32 count;
} ShaderSrc;

typedef struct
{
	ShaderSrc *vertex_src;
	ShaderSrc *fragment_src;
} ShaderSrcSet;

ShaderSrcSet shader_src_set_make(const char *vertex_path, const char *fragment_path);
void         shader_src_set_destroy(ShaderSrcSet *src);
void         shader_src_add_define(ShaderSrc *src, const char *define);
void         shader_src_print(ShaderSrc *src);
