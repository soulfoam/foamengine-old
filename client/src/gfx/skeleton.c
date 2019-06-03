#include <string.h>
#include <core/ext/logger.h>
#include "skeleton.h"
#include "../engine.h"

// optimize this after we get it working to not recalculate no changes
static void
bone_lerp(Skeleton *skel, Bone *bone, u32 cur_frame, u32 next_frame, mat4 dst)
{
	assert(skel && bone && bone->cur_bone_anim);

	const BoneAnimData *bone_anim = bone->cur_bone_anim;

	f32 lerp_time = skel->anim.time;

	vec3 trl = {0, 0, 0};
	glm_vec3_lerp(bone_anim->translations[cur_frame],
			bone_anim->translations[next_frame],
			lerp_time,
			trl);
			
	versor rot = {0, 0, 0, 1};
	glm_quat_slerp(bone_anim->rotations[cur_frame],
			bone_anim->rotations[next_frame],
			lerp_time,
			rot);

	vec3 scl = {0, 0, 0};
	glm_vec3_lerp(bone_anim->scales[cur_frame],
			bone_anim->scales[next_frame],
			lerp_time,
			scl);

	mat4 result = GLM_MAT4_IDENTITY_INIT;
	
	glm_translate(result, trl);
	glm_quat_rotate(result, rot, result);
	glm_scale(result, scl);	
	
	glm_mat4_copy(result, dst);

}

static void
calc_bones(Skeleton *skel, u32 next_frame)
{
	u32 bone_count = skel->bone_count;

	for (u32 i = 0; i < bone_count; i++)
	{
		Bone *bone = &skel->bones[i];

		mat4 bone_mat;
		bone_lerp(skel, bone, skel->anim.cur_frame, next_frame, bone_mat);
	
		if (bone->parent_bone)
		{
			glm_mat4_mul(bone->parent_bone->render_matrix, bone_mat, bone->render_matrix);
		}
		else
		{
			glm_mat4_copy(bone_mat, bone->render_matrix);
		}
	}

	for (u32 i = 0; i < bone_count; i++)
	{
		Bone *bone = &skel->bones[i];
		glm_mat4_mul(bone->render_matrix, *bone->offset_matrix, bone->render_matrix);
	}

}

static void
reset_anim(Skeleton *skel)
{
	skel->anim.playing       = false;
	skel->anim.played_once   = false;
	skel->anim.loop          = false;
	skel->anim.time          = 0;
	skel->anim.cur_frame     = 0;
	skel->anim.cur_anim_data = NULL;
	skel->anim.duration      = 0;
	skel->anim.tick_rate     = 0;
}

Skeleton*
skeleton_make(SkeletonData *skel_data)
{
	Skeleton *skel   = malloc(sizeof(*skel));
	skel->bones      = malloc(sizeof(Bone) * skel_data->bone_count);
	skel->bone_count = skel_data->bone_count;
	skel->skel_data  = skel_data;
	reset_anim(skel);

	for (u32 i = 0; i < skel_data->bone_count; i++)
	{
		BoneData *bone_data = &skel_data->bones[i];
		Bone *bone = &skel->bones[i];

		bone->name          = bone_data->name;
		bone->parent_name   = bone_data->parent_name;
		bone->parent_bone   = NULL;
		bone->meshes        = VECTOR_EMPTY;
		bone->cur_bone_anim = NULL;

		glm_mat4_identity(bone->render_matrix);
		bone->offset_matrix = &bone_data->offset_matrix;
	}
	
	for (u32 i = 1; i < skel->bone_count; i++)
	{
		Bone *bone = &skel->bones[i];
		bone->parent_bone = skeleton_get_bone(skel, bone->parent_name);
	}

	return skel;
}

void
skeleton_destroy(Skeleton *skel)
{
	free(skel->bones);	
	free(skel);
}

void
skeleton_update(Skeleton *skel)
{
	if (skel->anim.playing)
	{
		f32 time_per_frame  = 1.0f / skel->anim.tick_rate;
		skel->anim.time += engine_dt() / time_per_frame;

		while (skel->anim.time >= 1.0f)
		{
			skel->anim.cur_frame++;
			skel->anim.time--;
		}

		u32 next_frame = skel->anim.cur_frame+1; 
		if (next_frame >= skel->anim.duration)
		{
			if (skel->anim.loop) next_frame = 0;
			else next_frame = skel->anim.duration-1;
		}

		if (skel->anim.cur_frame >= skel->anim.duration)
		{
			if (skel->anim.loop)
			{
				skel->anim.cur_frame = 0;
				next_frame = 1;
			}
			else
			{
				skel->anim.cur_frame = skel->anim.duration-1;
				skel->anim.playing = false;
			}
			
			skel->anim.played_once = true;
		}
	
		calc_bones(skel, next_frame);
	}
}

Bone*
skeleton_get_bone(const Skeleton *skel, const char *bone_name)
{
	for (u32 i = 0; i < skel->bone_count; i++)
	{
		if (strcmp(skel->bones[i].name, bone_name) == 0) return &skel->bones[i];
	}

	return NULL;
}

void
skeleton_play_anim(Skeleton *skel, const char *name, bool loop)
{
	reset_anim(skel);

	skel->anim.loop    = loop;
	skel->anim.playing = true;
	
	SkeletonAnimData *anim   = skeleton_get_anim_data(skel->skel_data, name);
	skel->anim.cur_anim_data = anim;
	skel->anim.duration      = anim->duration;
	skel->anim.tick_rate     = anim->ticks_per_sec;

	for (u32 i = 0; i < skel->bone_count; i++)
	{
		Bone *bone = &skel->bones[i];		
		bone->cur_bone_anim = skeleton_get_bone_anim_data(anim, bone->name);	
	}
}

void
skeleton_restart_anim(Skeleton *skel)
{
	skel->anim.cur_frame = 0;
	skel->anim.time		 = 0;
}

void
bone_add_mesh(Bone *bone, Mesh *mesh)
{
	vector_push(bone->meshes, mesh);
}

void
bone_remove_mesh(Bone *bone, Mesh *mesh)
{
	vector_remove_eq(bone->meshes, mesh);
}
