#include <gl.h>
#include <string.h>
#include <stdlib.h>
#include <stb_image.h>
#include <core/ext/sugar.h>
#include "texture.h"

static GLuint tex_unit = 0;

/*============================*/
/* Internal methods and funcs */
/*============================*/
static Texture*
texture_core_make(const char *path, const u8 *buffer, u32 buffer_len)
{
	Texture *tex = malloc(sizeof(*tex));	
	tex->path = strdup(path);
	tex->unit = tex_unit;
	tex->num_of_rows = 1;

	s32 width = 0; s32 height = 0; s32 comp = 0;
	
	u8 *img = NULL;
	
	if (path == NULL)
	{
		img = stbi_load_from_memory(
			buffer, buffer_len, &width, &height, 
			&comp, STBI_rgb_alpha);
	}
	else
	{
		img = stbi_load(
			path, &width, &height, 
			&comp, STBI_rgb_alpha);
	}

	tex->size[0] = width;
	tex->size[1] = height;

	glGenTextures(1, &tex->id);

	texture_bind(tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	texture_unbind(tex);

	tex_unit++;

	stbi_image_free(img);

	return tex;
}

Texture*
texture_make(const char *path)
{
	return texture_core_make(path, NULL, 0);
}

Texture*
texture_buffer_make(const u8 *buffer, u32 buffer_len)
{
	return texture_core_make(NULL, buffer, buffer_len);
}

Texture* 
texture_atlas_make(const char *path, u32 num_of_rows)
{
	Texture *tex = texture_make(path);
	tex->num_of_rows = num_of_rows;
	return tex;
}

void 
texture_destroy(Texture *tex)
{
	glDeleteTextures(1, &tex->id);
	free(tex->path);
	free(tex);
}

void
texture_bind(const Texture *tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->id);
}

void
texture_unbind(const Texture *tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


