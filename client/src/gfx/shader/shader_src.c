#include <core/ext/sugar.h>
#include <gl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <core/ext/logger.h>
#include <core/ext/util.h>
#include "shader.h"
#include "shader_src.h"

const char *GL_VER[] =
{
	"#version 330 core\n",
	"#version 300 es\nprecision mediump float;\nprecision mediump sampler2DArray;\n"
};

static char*
load_src(const char *path)
{
	FILE *file = fopen(path, "rb");

	if (!file) 
	{
		printf("Failed to load shader src: %s\n", path);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	rewind(file);
	char *data = calloc(length + 1, sizeof(char));
	fread(data, 1, length, file);
	fclose(file);

	data[length] = '\0';

	return data;
}

static ShaderSrc*
shader_src_make(const char *src_path)
{
	char *src = load_src(src_path);

	char *token = strtok(src, "\n");

	char file_names[100][128];
	u32 file_names_index = 0;

	while (token != NULL)
	{
		char *line = token;	
		char *inc = strstr(line, "#include");
		if (inc)
		{	
			char file_name[128];
			strcpy(file_name, strchr(inc, '"')+1);
			char *n_name = strrchr(file_name, '"');
			if (n_name) *n_name = '\0';
				
			strcpy(file_names[file_names_index++], file_name);
		}
		token = strtok(NULL, "\n");
	}
	
	ShaderSrc *shader_src = malloc(sizeof(*shader_src));
	shader_src->srcs = malloc(sizeof(char*) * file_names_index+2); // +2 cause orig src + ver

	free(src);

	char *new_src = load_src(src_path);

	for (u32 i = 0; i < file_names_index; i++)
	{
		char *file_name = file_names[i];

		const char *shader_path = "res/shaders/";
		char file_name_f[512];
		sprintf(file_name_f, "%s%s", shader_path, file_name);
		shader_src->srcs[i+1] = load_src(file_name_f);
		
		char line_name[512];
		sprintf(line_name, "#include \"%s\"", file_name);
		
		new_src = str_remove(new_src, line_name);
	}
	
	char *ver = NULL;
	u32 gl_index = 0;

#ifdef __EMSCRIPTEN__	
	gl_index = 1;
#endif

	ver = malloc(strlen(GL_VER[gl_index]+1));
	strcpy(ver, GL_VER[gl_index]);

	shader_src->srcs[0] = ver;
	shader_src->srcs[file_names_index+1] = new_src;
	shader_src->count = file_names_index+2;

	return shader_src;
}

static void         
shader_src_destroy(ShaderSrc *src)
{
	for (u32 i = 0; i < src->count; i++)
	{
		free(src->srcs[i]);
	}

	free(src->srcs);
	free(src);
}


ShaderSrcSet
shader_src_set_make(const char *vertex_path, const char *fragment_path)
{
	ShaderSrcSet set = 
	{
		.vertex_src = shader_src_make(vertex_path),
		.fragment_src = shader_src_make(fragment_path),
	};

	return set;
}

void         
shader_src_set_destroy(ShaderSrcSet *set)
{
	shader_src_destroy(set->vertex_src);
	shader_src_destroy(set->fragment_src);
}

void
shader_src_add_define(ShaderSrc *src, const char *define)
{
	char *new_src = malloc((strlen(define)+1) + (strlen(src->srcs[0])+1));
	sprintf(new_src, "%s\n#define %s\n", src->srcs[0], define);
	free(src->srcs[0]);
	src->srcs[0] = new_src;
}

void
shader_src_print(ShaderSrc *src)
{
	for (u32 i = 0; i < src->count; i++)
	{
		printf("%s\n", src->srcs[i]);
	}
}
