#pragma once

#include <gl.h>
#include <cglm/cglm.h>
#include "shader/shader.h"

typedef struct
{
	vec3 pos;
	vec3 color;
} Light;

Light* light_make(vec3 pos, vec3 color);
void   light_destroy(Light *light);
void   light_render(Light *light);
