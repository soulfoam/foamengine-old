#pragma once

#include <core/ext/sugar.h>

typedef struct
{

} Game;


void	 game_init(void);
void     game_cleanup(void);
void     game_tick(bool fixed_tick);
void     game_render(void);

