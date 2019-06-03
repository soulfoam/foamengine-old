#include "binary_mapper_ext.h"
#include "logger.h"

static const u32 MODEL_VER = 1;

static void
bm_map_mesh_data(BinaryMapper *mapper, MeshData *md)
{
	bm_map_char(mapper, md->name, MAX_NAME_LEN);

	bm_map_u32(mapper, &md->vert_count);
	bm_map_u32(mapper, &md->indice_count);

	if (mapper->mode == BINARY_MAPPER_READ)
	{
		md->verts = malloc(sizeof(Vertex) * md->vert_count);
		md->indices = malloc(sizeof(u32) * md->indice_count);
	}

	for (u32 i = 0; i < md->vert_count; i++)
	{
		Vertex *v = &md->verts[i];
		bm_map_vec3(mapper, v->pos);
		bm_map_vec2(mapper, v->uv);
		bm_map_vec3(mapper, v->norm);
	}

	for (u32 i = 0; i < md->indice_count; i++)
	{
		u32 *indice = &md->indices[i];
		bm_map_u32(mapper, indice);
	}		
}

void
bm_map_mesh_bundle(BinaryMapper *mapper, MeshBundle *mb)
{
	bm_map_u32(mapper, &mb->version);
	bm_map_u32(mapper, &mb->mesh_count);	

	if (mapper->mode == BINARY_MAPPER_READ)
	{
		mb->meshes = malloc(sizeof(MeshData) * mb->mesh_count);
	}
	for (u32 i = 0; i < mb->mesh_count; i++)
	{
		bm_map_mesh_data(mapper, &mb->meshes[i]);
	}
}


static void
bm_map_bone_data(BinaryMapper *mapper, BoneData *bone_data)
{	
	bm_map_char(mapper, bone_data->name, MAX_NAME_LEN);
	bm_map_char(mapper, bone_data->parent_name, MAX_NAME_LEN);
	bm_map_mat4(mapper, bone_data->offset_matrix);
}

static void
bm_map_bone_anim_data(BinaryMapper *mapper, BoneAnimData *anim_data)
{	
	bm_map_char(mapper, anim_data->name, MAX_NAME_LEN);

	bm_map_u32(mapper, &anim_data->translation_count);
	bm_map_u32(mapper, &anim_data->rotation_count);
	bm_map_u32(mapper, &anim_data->scale_count);

	if (mapper->mode == BINARY_MAPPER_READ)
	{
		anim_data->translations = malloc(sizeof(vec3) * anim_data->translation_count);
		anim_data->rotations = malloc(sizeof(versor) * anim_data->rotation_count);
		anim_data->scales = malloc(sizeof(vec3) * anim_data->scale_count);
	}
	
	for (u32 i = 0; i < anim_data->translation_count; i++)
	{
		bm_map_vec3(mapper, anim_data->translations[i]);
	}
	for (u32 i = 0; i < anim_data->rotation_count; i++)
	{
		bm_map_vec4(mapper, anim_data->rotations[i]);
	}
	for (u32 i = 0; i < anim_data->scale_count; i++)
	{
		bm_map_vec3(mapper, anim_data->scales[i]);
	}
}

static void
bm_map_skeleton_anim_data(BinaryMapper *mapper, SkeletonAnimData *anim_data)
{	
	bm_map_char(mapper, anim_data->name, MAX_NAME_LEN);
	bm_map_f32(mapper, &anim_data->duration);
	bm_map_f32(mapper, &anim_data->ticks_per_sec);
	bm_map_u32(mapper, &anim_data->bone_anim_count);

	if (mapper->mode == BINARY_MAPPER_READ)
	{
		anim_data->bone_anims = malloc(sizeof(BoneAnimData) * anim_data->bone_anim_count);
	}

	for (u32 i = 0; i < anim_data->bone_anim_count; i++)
	{
		bm_map_bone_anim_data(mapper, &anim_data->bone_anims[i]);
	}
}

void
bm_map_skeleton_data(BinaryMapper *mapper, SkeletonData *skel_data)
{
	bm_map_u32(mapper, &skel_data->bone_count);
	bm_map_u32(mapper, &skel_data->anim_count);

	if (mapper->mode == BINARY_MAPPER_READ)
	{
		skel_data->bones = malloc(sizeof(BoneData) * skel_data->bone_count);
		skel_data->anims = malloc(sizeof(SkeletonAnimData) * skel_data->anim_count);
	}

	for (u32 i = 0; i < skel_data->bone_count; i++)
	{
		bm_map_bone_data(mapper, &skel_data->bones[i]);
	}
	for (u32 i = 0; i < skel_data->anim_count; i++)
	{
		bm_map_skeleton_anim_data(mapper, &skel_data->anims[i]);
	}
}

