#include <core/ext/vector.h>
#include "shader_module.h"

#define SHADER_COUNT 3

static Shader active_shader = 0;
static Shader shaders[SHADER_COUNT] = {0};

void
shader_module_init(void)
{	
	ShaderSrcSet def_set = shader_src_set_make("res/shaders/def.vs", "res/shaders/def.fs");
	ShaderSrcSet def_set_instance = shader_src_set_make("res/shaders/def.vs", "res/shaders/def.fs");
	ShaderSrcSet terrain_set = shader_src_set_make("res/shaders/terrain.vs", "res/shaders/terrain.fs");

	shader_src_add_define(def_set_instance.vertex_src, "INSTANCING");

	Shader def = shader_make(&def_set);
	Shader def_instancing = shader_make(&def_set_instance);
	Shader terrain = shader_make(&terrain_set);

	shaders[SHADER_DEFAULT] = def;
	shaders[SHADER_DEFAULT_INSTANCING] = def_instancing;
	shaders[ShaderERRAIN] = terrain;

	shader_src_set_destroy(&def_set);
	shader_src_set_destroy(&def_set_instance);
	shader_src_set_destroy(&terrain_set);

	shader_use(def);
}

void
shader_module_cleanup(void)
{
	for (u32 i = 0; i < SHADER_COUNT; i++)
	{
		shader_destroy(shaders[i]);
	}
}

Shader
shader_module_get_shader(ShaderType type)
{
	return shaders[type];	
}

void
shader_module_set_active_shader(Shader shader)
{
	active_shader = shader;
}

Shader
shader_module_get_active_shader(void)
{
	return active_shader;
}

