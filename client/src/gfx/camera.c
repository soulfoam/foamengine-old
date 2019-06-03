#include <gl.h>
#include <core/ext/sugar.h>
#include <core/ext/logger.h>
#include <cglm/cglm.h>
#include "camera.h"
#include "gfx.h"
#include "shader/shader.h"
#include "shader/shader_module.h"

static const f32 FOV                = 60.0f;
static const f32 NEAR_CLIP          = 0.1f;
static const f32 FAR_CLIP           = 2000.0f;
static const f32 DISTANCE_FROM_TARG = 15.0f;

/*==============================*/
/* Public methods and functions */
/*==============================*/
Camera*
camera_make(vec3 pos)
{

	Camera *cam = malloc(sizeof(*cam));
	cam->fov        = FOV;
	cam->near_clip  = NEAR_CLIP;
	cam->far_clip   = FAR_CLIP;
	cam->targ		= NULL;
	cam->targ_dist  = DISTANCE_FROM_TARG;
	cam->targ_ang   = 0;

	glm_vec3_copy(pos, cam->pos);
	glm_vec3_copy(pos, cam->prev_pos);
	glm_vec3_zero(cam->rot);
	glm_vec3_zero(cam->prev_rot);

	glm_mat4_identity(cam->matrix);
	glm_translate(cam->matrix, pos);

	return cam;
}

void
camera_destroy(Camera *cam)
{
	free(cam);
}

void
camera_update(Camera *cam)
{

}

void
camera_render2d(Camera *cam)
{
	GLuint active_shader = shader_module_get_active_shader();

    glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Window *win = gfx_get_window();

	//shader_bool_set(active_shader, "gui_mode", true);
	mat4 proj = GLM_MAT4_IDENTITY_INIT;
	glm_ortho(0.0f, win->size[0], win->size[1], 0.0f, -1.0f, 1.0f, proj);
	//shader_set_mat4(active_shader, S_PROJECTION, proj);
}

void
camera_get_projection(Camera *cam, mat4 dst)
{
	Window *win = gfx_get_window();

	glm_mat4_identity(dst);
		
	glm_perspective(
			glm_rad(cam->fov), 
			win->size[0] / win->size[1], 
			cam->near_clip, 
			cam->far_clip, 
			dst);
}

void
camera_get_view(Camera *cam, mat4 dst)
{
	glm_mat4_identity(dst);

	mat4 view = 
	{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	glm_mat4_transpose(view);
	glm_mat4_mul(view, cam->matrix, dst);

	dst[3][0] = -dst[3][0];
	dst[3][1] = -dst[3][1];
	dst[3][2] = -dst[3][2];
}

