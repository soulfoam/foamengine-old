#pragma once

#include <gl.h>
#include <cglm/cglm.h>
#include <core/ext/sugar.h>
#include <core/data/mesh_bundle.h>
#include "../texture.h"
#include "../shader/shader_module.h"

typedef struct
{
	u32            count;
	GLuint         pos_buffer;
} MeshInstance;

typedef enum
{
	RENDER_NORMAL      = 1 << 0,
	RENDER_OUTLINE     = 1 << 1,
	RENDER_TRANSPARENT = 1 << 2,


	RENDER_FLAG_COUNT
} MeshRenderFlag;

typedef struct
{
	char name[MAX_NAME_LEN];
	u32 vert_count;
	u32 indice_count;

	MeshInstance instance;
	
	vec3   pos;
	versor rot;
	vec3   scale;
	mat4   matrix;

	vec4    color;
	Texture *tex;
	u32     tex_index;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	ShaderType  shader_type;
	MeshRenderFlag render_flags;

	bool visible;

} Mesh;

Mesh*	 mesh_make(MeshData *mesh_data);
void     mesh_destroy(Mesh *mesh);
void	 mesh_build_instance_buffer(Mesh *mesh, u32 count, mat4 *positions);
void	 mesh_clear_instance_buffer(Mesh *mesh);
void     mesh_render(Mesh *mesh);
void	 mesh_set_color(Mesh *mesh, vec4 color);

