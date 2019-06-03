#pragma once

#include "../ext/sugar.h"
#include <cglm/cglm.h>

#define MAX_NAME_LEN 64

typedef struct
{
	char name[MAX_NAME_LEN];

	vec3 *translations;
	u32 translation_count;
	versor *rotations;
	u32 rotation_count;
	vec3 *scales;
	u32 scale_count;
	
} BoneAnimData;

typedef struct
{

	char name[MAX_NAME_LEN];
	f32 duration;
	f32 ticks_per_sec;
	BoneAnimData *bone_anims;
	u32 bone_anim_count;

} SkeletonAnimData;

typedef struct
{	
	char name[MAX_NAME_LEN];	
	char parent_name[MAX_NAME_LEN];	

	mat4 offset_matrix;

} BoneData;

typedef struct
{
	char *path;
	BoneData *bones;
	u32 bone_count;
	SkeletonAnimData *anims;
	u32 anim_count;

} SkeletonData;

SkeletonData*     skeleton_data_make(const char *path);
void              skeleton_data_destroy(SkeletonData *skel_data);
BoneData*         skeleton_get_bone_data(const SkeletonData *skel_data, const char *bone_str);
SkeletonAnimData* skeleton_get_anim_data(const SkeletonData *skel_data, const char *anim_str);
BoneAnimData*     skeleton_get_bone_anim_data(const SkeletonAnimData *anim, const char *name);


