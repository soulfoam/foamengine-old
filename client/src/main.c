#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "engine.h"
#include "game/game.h"

int
main(int argc, char **argv)
{
	engine_init("3ps shooter", (vec2){1280, 720});
	engine_set_tick_callback(game_tick);
	engine_set_render_callback(game_render);
	engine_set_tick_rate(25);	

	game_init();
	engine_start();
	game_cleanup();
	engine_cleanup();
}

