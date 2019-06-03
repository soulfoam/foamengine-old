#include <string.h>
#include "../ext/sugar.h"
#include "../ext/binary_mapper_ext.h"
#include "../ext/logger.h"
#include "skeleton_data.h"

static const u32 MODEL_VER       = 1;
static const char *SK_FORMAT_EXT = "skel";

/*===========================*/
/* Private methods and funcs */
/*===========================*/
SkeletonData*
skeleton_data_make(const char *path)
{
	char f_path[4096];
	memset(f_path, '\0', 4096);
	sprintf(f_path, "%s.%s", path, SK_FORMAT_EXT);
	
	SkeletonData *skel_data = malloc(sizeof(*skel_data));
	skel_data->path         = strdup(path);

	BinaryMapper *mapper = binary_mapper_load(f_path);
	if (mapper == NULL)
	{
		log_fatal("mesh_bundle: (%s) doesn't exist / couldn't be read from the disk!", f_path);
		return NULL;
	}

	bm_map_skeleton_data(mapper, skel_data);

	binary_mapper_destroy(mapper);

	return skel_data;
}

void
skeleton_data_destroy(SkeletonData *skel_data)
{
	for (u32 i = 0; i < skel_data->anim_count; i++)
	{
		SkeletonAnimData *anim = &skel_data->anims[i];

		for (u32 j = 0; j < anim->bone_anim_count; j++)
		{
			BoneAnimData *ba = &anim->bone_anims[j];
			free(ba->translations);
			free(ba->rotations);
			free(ba->scales);
		}

		free(anim->bone_anims);
	}
	
	free(skel_data->anims);
	free(skel_data->bones);
	free(skel_data->path);
	free(skel_data);
}

BoneData*
skeleton_get_bone_data(const SkeletonData *skel, const char *bone_str)
{
	for (u32 i = 0; i < skel->bone_count; i++)
	{
		if (strcmp(skel->bones[i].name, bone_str) == 0) return &skel->bones[i];
	}

	return NULL;
}

SkeletonAnimData*
skeleton_get_anim_data(const SkeletonData *skel_data, const char *anim_str)
{
	for (u32 i = 0; i < skel_data->anim_count; i++)
	{
		if (strcmp(skel_data->anims[i].name, anim_str) == 0) return &skel_data->anims[i];
	}

	return NULL;
}

BoneAnimData*
skeleton_get_bone_anim_data(const SkeletonAnimData *anim, const char *name)
{
	for (u32 i = 0; i < anim->bone_anim_count; i++)
	{
		if (strcmp(anim->bone_anims[i].name, name) == 0) return &anim->bone_anims[i];
	}

	return NULL;
}

