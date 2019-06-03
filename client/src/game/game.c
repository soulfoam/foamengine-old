#include <stdio.h>
#include <core/ext/logger.h>
#include <core/ext/binary_mapper_ext.h>
#include "game.h"
#include "../gfx/shader/shader.h"
#include "../gfx/shader/shader_module.h"
#include "../gfx/model.h"
#include "../gfx/camera.h"
#include "../gfx/light.h"
#include "../gfx/mesh/mesh_module.h"
#include "../gfx/model_module.h"
#include "../gfx/terrain.h"
#include "../gfx/terrain_module.h"
#include "../engine.h"

#define TEST_AMT 1

static Model *mdl       = NULL;
static Model *axes      = NULL;
static Model *cube      = NULL;
static Model *terrain   = NULL;
static Camera *cam      = NULL;

static void
mouse_move(f64 x, f64 y)
{

}

void
game_init(void)
{

	MeshBundle *debug_bundle = mesh_bundle_make("res/models/debug");
	MeshBundle *guy_meshes = mesh_bundle_make("res/models/guy");
	SkeletonData *guy_skel_data = skeleton_data_make("res/models/guy");

	const char *bone_list[] = 
	{
		"bone_pelvis",
		"bone_torso",
		"bone_head",
		"bone_upper_arm.l",
		"bone_lower_arm.l",
		"bone_hand.l",
		"bone_upper_arm.r",
		"bone_lower_arm.r",
		"bone_hand.r",
		"bone_upper_leg.l",
		"bone_lower_leg.l",
		"bone_foot.l",
		"bone_upper_leg.r",
		"bone_lower_leg.r",
		"bone_foot.r",
	};

	const char *mesh_list[] = 
	{
		"pelvis",
		"torso",
		"head",
		"upper_arm.l",
		"lower_arm.l",
		"hand.l",
		"upper_arm.r",
		"lower_arm.r",
		"hand.r",
		"upper_leg.l",
		"lower_leg.l",
		"foot.l",
		"upper_leg.r",
		"lower_leg.r",
		"foot.r",
	};
	
	versor ang;
	glm_quat_identity(ang);

	mdl = model_make((vec3){0, 0, 0}, ang);	
	model_add_mesh_bundle(mdl, guy_meshes);
	model_set_skeleton(mdl, skeleton_make(guy_skel_data));
	
	for (u32 j = 0; j < sizeof(bone_list)/sizeof(bone_list[0]); j++)
	{
		Bone *bone = skeleton_get_bone(mdl->skeleton, bone_list[j]);
		bone_add_mesh(bone, model_get_mesh(mdl, mesh_list[j]));
	}

	skeleton_play_anim(mdl->skeleton, "wave", true);

	model_module_add_model(mdl);

	mat4 positions[2] = {GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT};	
	glm_translate(positions[0], (vec3){-3, 0, 0});
	glm_translate(positions[1], (vec3){5, 0, 2});

	Mesh *cube_mesh = mesh_make(mesh_bundle_get_mesh_data(debug_bundle, "cube"));
	cube = model_make((vec3){0, 0, 1}, ang);
	model_add_mesh(cube, cube_mesh);
	mesh_build_instance_buffer(cube_mesh, 2, positions);
	model_module_add_model(cube);

	axes = model_make((vec3){0, 0, 0}, ang);
	model_add_mesh(axes, mesh_make(mesh_bundle_get_mesh_data(debug_bundle, "coords")));
	model_module_add_model(axes);

	//terrain = model_make((vec3){0, 0, 0}, ang);
	//model_add_mesh(terrain, mesh_make(mesh_bundle_get_mesh_data(debug_bundle, "terrain")));
	//model_module_add_model(terrain);
	
	cam = camera_make((vec3){0, -12, 2});

	//skeleton_data_destroy(guy_skel_data);
	//mesh_bundle_destroy(guy_meshes);
	//mesh_bundle_destroy(debug_bundle);
    //we dont destroy them because the model needs them and will cause segfault

}

void
game_cleanup(void)
{

}

void
game_tick(bool fixed_tick)
{	
	camera_update(cam);
	model_module_update();
}

void
game_render(void)
{
//	terrain_module_render(cam);
	mesh_module_render(cam);	
}

