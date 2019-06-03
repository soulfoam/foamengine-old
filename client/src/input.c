#include <core/ext/sugar.h>
#include <string.h>
#include <cglm/cglm.h>
#include "input.h"
#include "gfx/gfx.h"

#define clamp(v, min, max) (((v) > (max)) ? (max) : (((v) < (min)) ? (min) : (v)))

/*===============*/
/* Internal vars */
/*===============*/
static vec2 mouse_last = {0, 0};
static vec2 mouse_offset = {0, 0};
static vec2 mouse_scroll_offset = {0, 0};
static bool first_mouse_move = true;

/*============================*/
/* Internal methods and funcs */
/*============================*/
static void 
input_mouse_move(GLFWwindow *window, f64 xpos, f64 ypos)
{
	if (first_mouse_move)
	{
		mouse_last[0] = xpos;
		mouse_last[1] = ypos;
		first_mouse_move = false;
	}

    mouse_offset[0] = (f32)(xpos - mouse_last[0]);
    mouse_offset[1] = (f32)(mouse_last[1] - ypos);	

    mouse_last[0] = (f32)xpos;
    mouse_last[1] = (f32)ypos;


}

static void 
input_mouse_scroll(GLFWwindow *window, f64 xoffset, f64 yoffset)
{
	yoffset = clamp(yoffset, -1, 1);
#ifdef __EMSCRIPTEN__
	/* GLFW3 seems to have a bug with WebGL where the mouse Y is inverted */ 
	yoffset *= -1;
#endif

	mouse_scroll_offset[0] = xoffset;
	mouse_scroll_offset[1] = yoffset;

}

static void
input_mouse_button(GLFWwindow *window, s32 button, s32 action, s32 mods)
{

}

static void
input_key(GLFWwindow *window, s32 key, s32 scancode, s32 action, s32 mode)
{

}

/*==========================*/
/* Public methods and funcs */
/*==========================*/
void
input_module_init(void)
{
	GLFWwindow *window = gfx_get_window()->window;

    glfwSetCursorPosCallback(window, input_mouse_move);
    glfwSetScrollCallback(window, input_mouse_scroll);
    glfwSetKeyCallback(window, input_key);
	glfwSetMouseButtonCallback(window, input_mouse_button);

}

void
input_module_cleanup(void)
{

}

void
input_lock_mouse(bool lock)
{
	GLFWwindow *window = gfx_get_window()->window;

	if (lock) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	
}

void 
input_get_mouse_pos(vec2 pos)
{
	memcpy(pos, mouse_last, sizeof(vec2));
}

void
input_get_mouse_offset(vec2 offset)
{
	memcpy(offset, mouse_offset, sizeof(vec2));
}

void
input_get_mouse_scroll_offset(vec2 offset)
{
	memcpy(offset, mouse_scroll_offset, sizeof(vec2));
}

