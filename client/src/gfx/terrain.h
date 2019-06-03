#pragma once

#include <core/ext/sugar.h>
#include <cglm/cglm.h>
#include "model.h"

typedef struct
{
	Mesh *mesh;
	vec3 pos;
	f32 size;
	u32 vertex_count;	
} terrain_t;

terrain_t* terrain_make(vec3 pos, f32 size, u32 vertex_count);
void	   terrain_destroy(terrain_t *terrain);
