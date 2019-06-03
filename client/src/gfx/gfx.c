#include <core/ext/sugar.h>
#include <core/ext/logger.h>
#include <stdio.h>
#include <string.h>
#include <gl.h>
#include <stb_image.h>
#include "gfx.h"

#define RES_WIDTH 1280
#define RES_HEIGHT 720

static Window win = {{0}, {RES_WIDTH, RES_HEIGHT}, false, NULL};

/*=============================*/
/* Internal methods and funcs  */
/*=============================*/
static void
gfx_window_resize(GLFWwindow *glfw_win, s32 width, s32 height)
{
	win.size[0] = (f32)width; 
	win.size[1] = (f32)height;
	glViewport(0, 0, width, height);
}

/*===========================*/
/* Private methods and funcs */
/*===========================*/
void 
gfx_module_init(const char *title, vec2 window_size)
{

	glfwInit();

#ifdef __EMSCRIPTEN__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	win.window = glfwCreateWindow((s32)window_size[0], (s32)window_size[1], title, NULL, NULL);
    if (win.window == NULL)
    {
		printf("Failed to create GLFW window :(\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(win.window);
    glfwSetFramebufferSizeCallback(win.window, gfx_window_resize);

#ifndef __EMSCRIPTEN__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		printf("Failed to init GLAD\n");
        return;
    }

	glfwSwapInterval(0); 
#endif

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	gfx_set_Windowitle(title);
	gfx_set_window_size(window_size);

	printf("Vendor:          %s\n", glGetString(GL_VENDOR));
	printf("Renderer:        %s\n", glGetString(GL_RENDERER));
	printf("Version OpenGL:  %s\n", glGetString(GL_VERSION));
	printf("Version GLSL:    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

}

void
gfx_module_cleanup(void)
{
	glfwDestroyWindow(win.window); 
}

Window*
gfx_get_window(void)
{
	return &win;
}

void
gfx_set_window_size(vec2 size)
{
	memcpy(win.size, size, sizeof(vec2));
	glfwSetWindowSize(win.window, (s32)size[0], (s32)size[1]);
}

void
gfx_set_Windowitle(const char *title)
{
	strcpy(win.title, title);
	glfwSetWindowTitle(win.window, title);
}

void
gfx_set_fullscreen(bool fullscreen)
{
	if (!win.fullscreen && fullscreen)
	{
		win.window = glfwCreateWindow(
			(s32)win.size[0], (s32)win.size[1], win.title, 
			glfwGetPrimaryMonitor(), NULL);
	}

	if (win.fullscreen && !fullscreen)
	{
		win.window = glfwCreateWindow(
			(s32)win.size[0], (s32)win.size[1], win.title, 
			NULL, NULL);
	}

	win.fullscreen = fullscreen;
}


