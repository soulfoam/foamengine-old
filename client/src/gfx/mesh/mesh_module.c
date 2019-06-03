#include <core/ext/logger.h>
#include "../gfx.h"
#include "mesh_module.h"
#include "mesh.h"
#include "../shader/shader_module.h"
#include "../light.h"

static const char *S_PROJECTION = "projection";
static const char *S_VIEW = "view";

static vector(Mesh*) meshes = VECTOR_EMPTY;
static Light *sun_light = NULL;

static s32
sort_meshes(const void *a, const void *b)
{
	const Mesh *ma = *(const Mesh**)a;
	const Mesh *mb = *(const Mesh**)b;

	return (ma->shader_type - mb->shader_type);
}

void
mesh_module_init(void)
{
	sun_light = light_make((vec3){0, 0, 10}, (vec3){1, 1, 1});
}

void
mesh_module_cleanup(void)
{
	vector_free(meshes);
	light_destroy(sun_light);
}

void
mesh_module_add_mesh(Mesh *mesh)
{
	vector_push(meshes, mesh);
	qsort(meshes, vector_count(meshes), sizeof(Mesh*), sort_meshes);
}

void
mesh_module_add_meshes(Mesh **mesh, u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		vector_push(meshes, mesh[i]);
	}
	qsort(meshes, vector_count(meshes), sizeof(Mesh*), sort_meshes);
}

void
mesh_module_remove_mesh(Mesh *mesh)
{
	vector_remove_eq(meshes, mesh);
	qsort(meshes, vector_count(meshes), sizeof(Mesh*), sort_meshes);
}

void
mesh_module_remove_meshes(Mesh **mesh, u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		vector_remove_eq(meshes, mesh[i]);
	}
	qsort(meshes, vector_count(meshes), sizeof(Mesh*), sort_meshes);
}

static void
render_mesh(Mesh *mesh, Camera *cam)
{
	mat4 projection;
	camera_get_projection(cam, projection);

	mat4 view;
	camera_get_view(cam, view);

	Shader shader = shader_module_get_shader(mesh->shader_type);	
	shader_use(shader);
	shader_set_mat4(shader, S_PROJECTION, projection);
	shader_set_mat4(shader, S_VIEW, view);

	mesh_render(mesh);
}

void
mesh_module_render(Camera *cam)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	light_render(sun_light);

	for (u32 i = 0; i < vector_count(meshes); i++)
	{
		Mesh *mesh = meshes[i];
		render_mesh(mesh, cam);	
	}

}

