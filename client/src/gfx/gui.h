#pragma once

#include <gl.h>
#include <cglm/cglm.h>
#include "texture.h"

typedef struct
{
	u32 vao;
	u32 vbo;
	vec2 pos;
	vec2 rot;
	f32 rot_amount;
	vec2 scale;
	vec4 color;
	
	Texture tex;
} Gui;

Gui* gui_init(Texture tex, vec2 pos, vec2 scale);
void   gui_destroy(Gui *gui);

