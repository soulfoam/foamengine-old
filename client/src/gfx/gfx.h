#pragma once

#include <gl.h>
#include <cglm/cglm.h>

typedef struct
{
	char title[1024];
	vec2 size;
	bool fullscreen;
	GLFWwindow *window;
} Window;

void       gfx_module_init(const char *title, vec2 window_size);
void       gfx_module_cleanup(void);
Window*  gfx_get_window(void);
void       gfx_set_window_size(vec2 size);
void       gfx_set_Windowitle(const char *title);
void       gfx_set_fullscreen(bool fullscreen);

