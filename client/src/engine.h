#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include <core/ext/sugar.h>

typedef void (*tick_callback)(bool fixed_tick);
typedef void (*render_callback)(void);

void engine_init(const char *title, vec2 window_size);
void engine_cleanup(void);
void engine_start(void);
void engine_set_tick_callback(tick_callback cb);
void engine_set_render_callback(render_callback cb);
void engine_set_tick_rate(f32 tick_rate);
f32  engine_dt(void);
f32  engine_rt(void);
u32  engine_fps(void);
f32  engine_time(void);


