#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <core/ext/sugar.h>
#include <core/ext/vector.h>
#include <core/ext/logger.h>
#include <core/data/skeleton_data.h>
#include <core/ext/binary_mapper_ext.h>
#include <float.h>
#include "convert_skeleton.h"

#define FORMAT_EXT "skel"

static const struct aiScene *scene = NULL;
static u32 input_file_type         = FBX;

static BoneData *bones         = NULL;
static u32 bone_count          = 0;
static u32 bone_tree_count     = 0;

static SkeletonAnimData *anims = NULL;
static u32 anim_count          = 0;

static void
cleanup(void)
{	
	aiReleaseImport(scene);
}

static struct aiNode*
get_node(struct aiNode *node, const char *name)
{
	if (strcmp(name, node->mName.data) == 0) return node;
	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		struct aiNode *n = get_node(node->mChildren[i], name);
		if (n) return n;
	}

	return NULL;
}

static struct aiBone*
get_bone(const char *name)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *m = scene->mMeshes[i];
		for (u32 j = 0; j < m->mNumBones; j++)
		{
			struct aiBone *b = m->mBones[j];
			if (strcmp(name, b->mName.data) == 0) return b;
		}
	}

	return NULL;
}

static struct aiNode*
get_root_bone(void)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *m = scene->mMeshes[i];
		for (u32 j = 0; j < m->mNumBones; j++)
		{
			struct aiBone *b = m->mBones[j];
			struct aiNode *node = get_node(scene->mRootNode, b->mName.data);
			struct aiBone *parent_bone = get_bone(node->mParent->mName.data);
			if (!parent_bone) return node;
		}
	}
	
	return NULL;
}

static void
gather_bone_count(struct aiNode *bone_node)
{
	struct aiBone *ai_bone = get_bone(bone_node->mName.data);
	if (ai_bone) bone_count++;

	for (u32 i = 0; i < bone_node->mNumChildren; i++)
	{	
		struct aiNode *c = bone_node->mChildren[i];
		
		gather_bone_count(c);
	}
}

static void
build_bone_tree(struct aiNode *bone_node)
{	

	BoneData *b = &bones[bone_tree_count++];
	strcpy(b->name, bone_node->mName.data);
	strcpy(b->parent_name, bone_node->mParent->mName.data);

	struct aiBone *ai_bone = get_bone(bone_node->mName.data);
	
	if (!ai_bone)
	{
		printf("Bone %s was unable to be found on any of the meshes."
				"Make sure the bone has vertexes attached to it or else the bone won't"
				"be imported into the list of meshes correctly.\n", 
				bone_node->mName.data);
		cleanup();
		exit(1);
	}

	aiMat4_to_mat4(&ai_bone->mOffsetMatrix, b->offset_matrix);

	for (u32 i = 0; i < bone_node->mNumChildren; i++)
	{	
		struct aiNode *c = bone_node->mChildren[i];
		
		build_bone_tree(c);
	}
}

void
export_skeleton(const char *path)
{

	BinaryMapper *mapper = binary_mapper_make(1024*1024*20);

	SkeletonData *skel_data = malloc(sizeof(*skel_data));
	skel_data->path = NULL;
	skel_data->bone_count = bone_count;
	skel_data->anim_count = anim_count;
	skel_data->bones = bones;
	skel_data->anims = anims;

	bm_map_skeleton_data(mapper, skel_data);

	char final_path[2048];
	memset(final_path, '\0', 2048);
	sprintf(final_path, "%s.%s", path, FORMAT_EXT);

	if (binary_mapper_save(mapper, final_path))
	{
		printf("\n--- %s (%lu bytes) successfully exported.\n\n", final_path, mapper->offset);
	}
	else
	{
		printf("\n--- %s (%lu bytes) FAILED to exported.\n\n", final_path, mapper->offset);
	}

	skeleton_data_destroy(skel_data);

}

void
convert_skeleton(const char *input_path, const char *output_path)
{
	set_input_file_type(input_path, &input_file_type);
	
	scene = aiImportFile(
			input_path, 
			aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene)
	{
		printf("Failed to load file %s\n%s\n", input_path, aiGetErrorString());
		return;
	}

	anim_count = scene->mNumAnimations;
	anims = malloc(sizeof(SkeletonAnimData) * anim_count);

	struct aiNode *root_bone = get_root_bone();
	gather_bone_count(root_bone);
	bones = malloc(sizeof(BoneData) * bone_count);
	build_bone_tree(root_bone);

	for (u32 i = 0; i < scene->mNumAnimations; i++)
	{
		struct aiAnimation *a = scene->mAnimations[i];
		
		SkeletonAnimData *anim = &anims[i];
		anim->duration = a->mDuration;
		anim->ticks_per_sec = a->mTicksPerSecond;

		if (anim->ticks_per_sec <= 0) anim->ticks_per_sec = 24;

		//strip and correct names
		if (input_file_type == FBX)
		{
			char *n_name = strchr(a->mName.data, '|')+1;
			if (n_name) strcpy(anim->name, n_name);
		}

		anim->bone_anim_count = a->mNumChannels-1;
		anim->bone_anims      = malloc(sizeof(BoneAnimData) * anim->bone_anim_count);

		//j = 1 because armature is the root animation, dont think we want it?
		for (u32 j = 1; j < a->mNumChannels; j++)
		{
			struct aiNodeAnim *node_anim = a->mChannels[j];

			BoneAnimData *bone_anim = &anim->bone_anims[j-1];

			memset(bone_anim->name, '\0', MAX_NAME_LEN);
			strncpy(bone_anim->name, node_anim->mNodeName.data, MAX_NAME_LEN);

			bone_anim->translation_count = node_anim->mNumPositionKeys;
			bone_anim->rotation_count    = node_anim->mNumRotationKeys;
			bone_anim->scale_count       = node_anim->mNumScalingKeys;

			bone_anim->translations = malloc(sizeof(vec3) * bone_anim->translation_count);
			bone_anim->rotations    = malloc(sizeof(versor) * bone_anim->rotation_count);
			bone_anim->scales       = malloc(sizeof(vec3) * bone_anim->scale_count);

			for (u32 p = 0; p < node_anim->mNumPositionKeys; p++)
			{
				f32 x = node_anim->mPositionKeys[p].mValue.x;
				f32 y = node_anim->mPositionKeys[p].mValue.y;
				f32 z = node_anim->mPositionKeys[p].mValue.z;

				bone_anim->translations[p][0] = x;
				bone_anim->translations[p][1] = y;
				bone_anim->translations[p][2] = z;
			}

			for (u32 r = 0; r < node_anim->mNumRotationKeys; r++)
			{
				f32 x = node_anim->mRotationKeys[r].mValue.x;
				f32 y = node_anim->mRotationKeys[r].mValue.y;
				f32 z = node_anim->mRotationKeys[r].mValue.z;
				f32 w = node_anim->mRotationKeys[r].mValue.w;
				
				bone_anim->rotations[r][0] = x;
				bone_anim->rotations[r][1] = y;
				bone_anim->rotations[r][2] = z;
				bone_anim->rotations[r][3] = w;

			}

			for (u32 s = 0; s < node_anim->mNumScalingKeys; s++)
			{
				f32 x = node_anim->mScalingKeys[s].mValue.x;
				f32 y = node_anim->mScalingKeys[s].mValue.y;
				f32 z = node_anim->mScalingKeys[s].mValue.z;		

				bone_anim->scales[s][0] = x;
				bone_anim->scales[s][1] = y;
				bone_anim->scales[s][2] = z;
			}
		}
	}
	

	printf("\nBone List:\n");

	for (u32 i = 0; i < bone_count; i++)
	{
		printf("%s (parent: %s)\n", bones[i].name, bones[i].parent_name);
		//mat4_print(bones[i].offset_matrix);
	}
	
	printf("\n\nAnimation List:\n");
	for (u32 i = 0; i < anim_count; i++) 
	{
		SkeletonAnimData *a = &anims[i];	
		printf("%s (frames %u) (duration: %.1f) (ticks_per_sec: %.1f)\n", 
				a->name, a->bone_anim_count,
				a->duration, a->ticks_per_sec);

	//	for (u32 b = 0; b < a->bone_anim_count; b++)
	//	{
	//		BoneAnimData *bone_anim = &a->bone_anims[b];
	//		printf("---- %s ----\n", bone_anim->name);

	//		u32 keyframe_count = bone_anim->translation_count;
	//		for (u32 k = 0; k < keyframe_count; k++)
	//		{
	//			vec3 t = {0, 0, 0}; 
	//			versor r = {0, 0, 0, 0};
	//			vec3 s = {0, 0, 0};

	//			glm_vec3_copy(bone_anim->translations[k], t);
	//			glm_vec4_copy(bone_anim->rotations[k], r);
	//			glm_vec3_copy(bone_anim->scales[k], s);

	//			printf("(%u) Translation: %f %f %f\n"
	//				   "(%u) Rotation: %f %f %f %f\n"
	//				   "(%u) Scale: %f %f %f\n",
	//					k, t[0], t[1], t[2], k, r[0], r[1], r[2], r[3], k, s[0], s[1], s[2]);
	//		}
	//	}
	}

	export_skeleton(output_path);

	cleanup();

}


