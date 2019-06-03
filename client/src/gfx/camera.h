#pragma once

#include <core/ext/sugar.h>
#include <cglm/cglm.h>
#include "model.h"

typedef struct 
{
	mat4 matrix;

	f32 fov;
	f32 near_clip;
	f32 far_clip;

	struct
	{
		Model *targ;
		f32 targ_dist;
		f32 targ_ang;

		vec3 pos;
		vec3 prev_pos;

		vec3 rot;
		vec3 prev_rot;
	};

} Camera;

Camera* camera_make(vec3 pos);
void    camera_destroy(Camera *cam);
void    camera_update(Camera *cam);
void    camera_get_projection(Camera *cam, mat4 dst);
void    camera_get_view(Camera *cam, mat4 dst);
