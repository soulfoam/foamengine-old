#pragma once

#include <cglm/cglm.h>
#include <core/ext/sugar.h>
#include <core/ext/vector.h>
#include <core/data/skeleton_data.h>
#include "mesh/mesh.h"

typedef struct
Bone
{	
	const char *name;	
	const char *parent_name;	

	struct Bone *parent_bone;

	mat4 *offset_matrix;

	mat4 render_matrix;

	vector(Mesh*) meshes;

	const BoneAnimData *cur_bone_anim;

} Bone;

typedef struct
{
	bool playing;
	bool played_once;
	bool loop;
	f32  time;
	u32  cur_frame;
	f32  duration;
	f32  tick_rate;

	const SkeletonAnimData *cur_anim_data;
} SkeletonAnim;

typedef struct
{

	Bone *bones;
	u32	bone_count;
	SkeletonData *skel_data;

	SkeletonAnim anim;
	
} Skeleton;

Skeleton* skeleton_make(SkeletonData *skel_data);
void      skeleton_destroy(Skeleton *skel);
void      skeleton_update(Skeleton *skel);
Bone*     skeleton_get_bone(const Skeleton *skel, const char *bone_name);
void      skeleton_play_anim(Skeleton *skel, const char *name, bool loop);
void      skeleton_restart_anim(Skeleton *skel);
void      bone_add_mesh(Bone *bone, Mesh *mesh);
void      bone_remove_mesh(Bone *bone, Mesh *mesh);

