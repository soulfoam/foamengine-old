#include <core/ext/sugar.h>
#include <stdio.h>
#include <string.h>
#include <stb_image.h>
#include <cglm/cglm.h>
#include "gui.h"
#include "mesh/mesh_preset.h"

//need gui module for these
//
Gui*
gui_init(Texture tex, vec2 pos, vec2 scale)
{	
	Gui *gui = malloc(sizeof(*gui));
	
	return gui;
}

void 
gui_destroy(Gui *gui)
{
	glDeleteVertexArrays(1, &gui->vao);
	glDeleteBuffers(1, &gui->vbo);
}

