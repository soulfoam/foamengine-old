#include <cglm/cglm.h>
#include "light.h"
#include "shader/shader_module.h"

static const char *S_LIGHT_POS   = "light_pos";
static const char *S_LIGHT_COLOR = "light_color";

Light* 
light_make(vec3 pos, vec3 color)
{
	Light *light = malloc(sizeof(*light));
	glm_vec3_copy(pos, light->pos);
	glm_vec3_copy(color, light->color);

	return light;
}

void
light_destroy(Light *light)
{
	free(light);
}

void
light_render(Light *light)
{
	GLuint active_shader = shader_module_get_active_shader();

	shader_set_vec3(active_shader, S_LIGHT_POS, light->pos);
	shader_set_vec3(active_shader, S_LIGHT_COLOR, light->color);
}



