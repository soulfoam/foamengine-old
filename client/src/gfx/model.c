#include <core/ext/io.h>
#include <core/ext/sugar.h>
#include <core/ext/logger.h>
#include <core/data/skeleton_data.h>
#include <core/data/mesh_bundle.h>
#include "model.h"
#include "shader/shader.h"
#include "shader/shader_module.h"
#include "mesh/mesh.h"
#include "../engine.h"

static const f32 BASE_MODEL_SCALE = 1;
static const char *S_MODEL        = "model";

/*============================*/
/* Internal methods and funcs */
/*============================*/
static void
calc_matrix(Model *mdl, mat4 dest)
{
	mat4 model = GLM_MAT4_IDENTITY_INIT;
	glm_translate(model, mdl->pos);
	glm_quat_rotate(model, mdl->rot, model);
	glm_scale(model, mdl->scale);	
	glm_mat4_copy(model, dest);
}

/*==========================*/
/* Public methods and funcs */
/*==========================*/
Model*
model_make(vec3 pos, versor rot)
{
	Model *mdl = malloc(sizeof(*mdl));

	glm_vec3_copy(pos, mdl->pos);
	glm_quat_copy(rot, mdl->rot);
	vec3 scale = {BASE_MODEL_SCALE, BASE_MODEL_SCALE, BASE_MODEL_SCALE};
	glm_vec3_copy(scale, mdl->scale);

	mdl->meshes    = VECTOR_EMPTY;
	mdl->skeleton  = NULL;

	//alloc texture

	return mdl;
}

void
model_destroy(Model *mdl)
{	
	model_remove_all_meshes(mdl);
	if (mdl->skeleton) skeleton_destroy(mdl->skeleton);

	vector_free(mdl->meshes);
	
	free(mdl);
}

void
model_update(Model *mdl)
{
	if (mdl->skeleton)
	{
		skeleton_update(mdl->skeleton);

		for (u32 i = 0; i < mdl->skeleton->bone_count; i++)
		{	
			Bone *bone = &mdl->skeleton->bones[i];
			u32 mesh_bone_count = vector_count(bone->meshes);
			if (mesh_bone_count <= 0) continue;

			//shader_set_bool(active_shader, S_HAS_BONE, true);

			for (u32 bm = 0; bm < mesh_bone_count; bm++)
			{	
				glm_mat4_copy(bone->render_matrix, bone->meshes[bm]->matrix);
			}
		}
	}
}

void
model_render(Model *mdl)
{	
	GLuint active_shader = shader_module_get_active_shader();

	mat4 model;
	calc_matrix(mdl, model);
	shader_set_mat4(active_shader, S_MODEL, model);

}

void
model_pos_set(Model *mdl, vec3 pos)
{	
	glm_vec3_copy(pos, mdl->pos);
}

void
model_add_mesh(Model *mdl, Mesh *mesh)
{
	vector_push(mdl->meshes, mesh);
}

void
model_remove_mesh(Model *mdl, Mesh *mesh)
{
	vector_remove_eq(mdl->meshes, mesh);
	mesh_destroy(mesh);
}

void
model_remove_all_meshes(Model *mdl)
{	
	for (u32 i = 0; i < vector_count(mdl->meshes); i++)
	{
		Mesh *mesh = mdl->meshes[i];
		mesh_destroy(mesh);
	}

	vector_free(mdl->meshes);
}

void
model_add_mesh_bundle(Model *mdl, const MeshBundle *bundle)
{	
	for (u32 i = 0; i < bundle->mesh_count; i++)
	{
		MeshData *mesh_data = &bundle->meshes[i];
		model_add_mesh(mdl, mesh_make(mesh_data));
	}
}

void
model_remove_mesh_bundle(Model *mdl, const MeshBundle *bundle)
{	
	for (u32 i = 0; i < vector_count(mdl->meshes); i++)
	{
		Mesh *mesh = mdl->meshes[i];
		MeshData *data = mesh_bundle_get_mesh_data(bundle, mesh->name);
		if (data) model_remove_mesh(mdl, mesh); 
	}
}

Mesh*
model_get_mesh(Model *mdl, const char *mesh_str)
{
	for (u32 i = 0; i < vector_count(mdl->meshes); i++)
	{
		if (strcmp(mesh_str, mdl->meshes[i]->name) == 0) return mdl->meshes[i];
	}

	return NULL;
}

void	 
model_set_skeleton(Model *mdl, Skeleton *skel)
{
	mdl->skeleton = skel;
}

void
model_print(Model *mdl)
{
	printf("\n--- Model: ---\n");
	printf("Pos: %f %f %f\nRot: %f %f %f %f\nScale: %f %f %f\n",
			mdl->pos[0], mdl->pos[1], mdl->pos[2],
			mdl->rot[0], mdl->rot[1], mdl->rot[2], mdl->rot[3],
			mdl->scale[0], mdl->scale[1], mdl->scale[2]);

	u32 mesh_count = vector_count(mdl->meshes);

	printf("\n--- Meshes: %u ---\n", mesh_count);

	for (u32 i = 0; i < mesh_count; i++)
	{
		Mesh *mesh = mdl->meshes[i];

		printf("(%s) Verts: %u Indices: %u \nPos: %f %f %f\nRot: %f %f %f %f\nScale: %f %f %f\n",
			mesh->name, mesh->vert_count, mesh->indice_count,
			mesh->pos[0], mesh->pos[1], mesh->pos[2],
			mesh->rot[0], mesh->rot[1], mesh->rot[2], mesh->rot[3],
			mesh->scale[0], mesh->scale[1], mesh->scale[2]);
	}

	if (mdl->skeleton)
	{
		u32 bone_count = mdl->skeleton->bone_count;
		u32 anim_count = mdl->skeleton->skel_data->anim_count;

		log_debug("mdl->skeleton: (%s) bones: (%u) anims: (%u)", 
				mdl->skeleton->skel_data->path, bone_count, anim_count);

		for (u32 i = 0; i < bone_count; i++)
		{
			Bone *bone = &mdl->skeleton->bones[i];

			log_debug("\n\nBone: (%s) Parent: (%s)(%p) Offset Matrix:", 
					bone->name, bone->parent_name, bone->parent_bone);
			//mat4_print(*bone->offset_matrix);
		}

	}
}

