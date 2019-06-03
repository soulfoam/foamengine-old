#ifdef _WIN32
#include <windows.h>
#define sleep(n) Sleep(n)
#else
#include <unistd.h>
#define sleep(n) usleep((n) * 1000)
#endif

#include <gl.h>
#include <string.h>       
#include <cglm/cglm.h>    
#include <stdio.h>        
#include <core/ext/logger.h>
#include "engine.h"
#include "gfx/camera.h"
#include "gfx/gfx.h"      
#include "gfx/model.h"      
#include "gfx/mesh/mesh_module.h"      
#include "gfx/terrain_module.h"
#include "gfx/model_module.h"
#include "gfx/shader/shader.h"
#include "gfx/gui.h"      
#include "input.h"      

/*======================*/
/* Internal engine vars */
/*======================*/
static bool quit = false;

static f32 tick_rate   = 1.0f / 60.0f;
static f32 last_time   = 0;
static f32 accumulator = 0;
static f32 delta_time  = 0;
static f32 render_time = 0;
static u32 fps		   = 0;

static f32 display_timer  = 0;
static u32 display_ticks  = 0;
static u32 display_frames = 0;

static tick_callback tick_cb;
static render_callback render_cb;

/*============================*/
/* Internal methods and funcs */
/*============================*/
static void
tick(bool fixed_tick)
{
	if (tick_cb) tick_cb(fixed_tick);
}

static void
render(void)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	if (render_cb) render_cb();

	glfwSwapBuffers(gfx_get_window()->window);

}

static void
events(void)
{
	glfwPollEvents();
}

static void
loop(void)
{	
	f32 now          = (f32)glfwGetTime();
	delta_time       = now - last_time;
	last_time        = now;
	accumulator     += delta_time;
	bool fixed_tick  = false;
	
	events();
	
	while (accumulator >= tick_rate)
	{
		tick(true);
		accumulator -= tick_rate;
		fixed_tick   = true;
		display_ticks++;
	}

	if (!fixed_tick)
	{
		tick(false);
	}
	
	render_time = accumulator / tick_rate;

	render();

	display_frames++;

	if (glfwGetTime() - display_timer > 1.0)
	{
		display_timer++;
		printf("FPS: %u ticks: %u\n", display_frames, display_ticks);
		fps = display_frames;
		display_frames = 0;
		display_ticks = 0;
	}

}

/*==========================*/
/* Public methods and funcs */
/*==========================*/
void
engine_init(const char *title, vec2 window_size)
{	
	
	logger_set_cleanup_callback(engine_cleanup);

	//pretty much now just need to write
	//model_module and have it update
	//dont even think it needs a renderer
	//we will need to multiply all meshes in a model
	//by the model matrix so its included
	gfx_module_init(title, window_size);
	shader_module_init();
	mesh_module_init();
	model_module_init();
	terrain_module_init();
	input_module_init();
	

	last_time = glfwGetTime();
	display_timer = last_time;
}

void
engine_cleanup(void)
{
	gfx_module_cleanup();
	shader_module_cleanup();
	mesh_module_cleanup();
	model_module_cleanup();
	terrain_module_cleanup();
	input_module_cleanup();

	glfwTerminate();

	exit(0);
}

void
engine_start(void)
{
	if (!tick_cb) log_warn("No tick callback has been set."
			"Set one with engine_tick_callback_set(bool fixed_update)");

	if (!render_cb) log_warn("No render callback has been set."
			"Set one with engine_render_callback_set()");

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(loop, 0, 1);
#else
	while (!quit)
	{	
		loop();
		sleep(1);
	}
#endif
}

void
engine_set_tick_callback(tick_callback cb)
{
	tick_cb = cb;
}

void
engine_set_render_callback(render_callback cb)
{
	render_cb = cb;
}

void
engine_set_tick_rate(f32 rate)
{
	tick_rate = (1.0f / rate);
}

f32
engine_dt(void)
{
	return delta_time;
}

f32
engine_rt(void)
{
	return render_time;
}

u32
engine_fps(void)
{
	return fps;
}

f32
engine_time(void)
{
	return (f32)glfwGetTime();
}


