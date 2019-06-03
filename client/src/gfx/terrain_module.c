#include <core/ext/vector.h>
#include <core/ext/logger.h>
#include "terrain.h"
#include "terrain_module.h"
#include "light.h"

static const char *S_PROJECTION = "projection";
static const char *S_VIEW = "view";

static vector(terrain_t*) terrains = VECTOR_EMPTY;
static Light *sun_light = NULL;

void
terrain_module_init(void)
{
	terrain_t *terrain = terrain_make((vec3){0, 0, 0}, 800, 128);
	sun_light = light_make((vec3){0, 0, 10}, (vec3){1, 1, 1});
	vector_push(terrains, terrain);
}

void
terrain_module_cleanup(void)
{
	vector_free_all(terrains, terrain_destroy);
	light_destroy(sun_light);
}

static void
render_terrain(terrain_t *terrain, Camera *cam)
{
	mat4 projection;
	camera_get_projection(cam, projection);

	mat4 view;
	camera_get_view(cam, view);

	Shader shader = shader_module_get_shader(terrain->mesh->shader_type);	

	shader_use(shader);
	shader_set_mat4(shader, S_PROJECTION, projection);
	shader_set_mat4(shader, S_VIEW, view);

	mesh_render(terrain->mesh);
}

void
terrain_module_render(Camera *cam)
{
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	light_render(sun_light);

	for (u32 i = 0; i < vector_count(terrains); i++)
	{
		terrain_t *terrain = terrains[i];
		render_terrain(terrain, cam);	
	}
}

