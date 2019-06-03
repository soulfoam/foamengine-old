#pragma once

#include <gl.h>
#include <cglm/cglm.h>
#include <core/ext/sugar.h>

typedef struct
{
	char *path;

	GLuint id;
	GLuint unit;
	vec2 size;

	u32 num_of_rows;
} Texture;

Texture* texture_make(const char *path);
Texture* texture_buffer_make(const u8 *buffer, u32 buffer_len);
Texture* texture_atlas_make(const char *path, u32 num_of_rows);
void texture_destroy(Texture *tex);
void texture_bind(const Texture *tex);
void texture_unbind(const Texture *tex);


