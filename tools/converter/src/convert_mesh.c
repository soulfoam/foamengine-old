#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <float.h>
#include <core/ext/binary_mapper_ext.h>
#include <core/data/mesh_bundle.h>
#include <core/data/skeleton_data.h>
#include "convert_mesh.h"
#include "util.h"

#define FORMAT_EXT "mb"

static const struct aiScene *scene = NULL;
static f_type input_file_type = OBJ;

static vector(MeshData) meshes = VECTOR_EMPTY;

static void
process_mesh(struct aiMesh *am, MeshData *mesh)
{

	mesh->vert_count = am->mNumVertices;
	mesh->indice_count = am->mNumFaces * 3;
	mesh->verts = malloc(sizeof(Vertex) * mesh->vert_count);
	mesh->indices = malloc(sizeof(u32) * mesh->indice_count);

	for (u32 i = 0; i < am->mNumVertices; i++)
	{
		Vertex *v = &mesh->verts[i];

		v->pos[0] = am->mVertices[i].x;
		v->pos[1] = am->mVertices[i].y;
		v->pos[2] = am->mVertices[i].z;

		if (am->mTextureCoords[0])
		{
			v->uv[0] = am->mTextureCoords[0][i].x;
			v->uv[1] = am->mTextureCoords[0][i].y;
		}
		else
		{
			v->uv[0] = 0.0f;
			v->uv[1] = 0.0f;
		}

		v->norm[0] = am->mNormals[i].x;
		v->norm[1] = am->mNormals[i].y;
		v->norm[2] = am->mNormals[i].z;

	}
	
	u32 top = 0;
	for (u32 i = 0; i < am->mNumFaces; i++)
	{
		struct aiFace f = am->mFaces[i];

		for (u32 j = 0; j < f.mNumIndices; j++) mesh->indices[top++] = f.mIndices[j];
	}

	vector_push(meshes, *mesh);

}

static void
cleanup(void)
{
	vector_free(meshes);
}

static void
export_bundle(const char *path)
{	
	BinaryMapper *mapper = binary_mapper_make(1024*1024*20);

	MeshBundle *mb = malloc(sizeof(*mb));
	mb->path       = NULL;
	mb->version    = MDL_VER;
	mb->mesh_count = vector_count(meshes);
	mb->meshes     = malloc(sizeof(MeshData) * mb->mesh_count);

	for (u32 i = 0; i < mb->mesh_count; i++)
	{
		memcpy(&mb->meshes[i], &meshes[i], sizeof(MeshData));
	}	

	bm_map_mesh_bundle(mapper, mb);

	char final_path[2048];
	memset(final_path, '\0', 2048);
	sprintf(final_path, "%s.%s", path, FORMAT_EXT);

	if (binary_mapper_save(mapper, final_path))
	{
		printf("\n--- %s (%lu bytes) successfully exported.\n\n", final_path, mapper->offset);
	}
	else
	{
		printf("\n--- %s (%lu bytes) FAILED to export!\n\n", final_path, mapper->offset);
	}
	
	binary_mapper_destroy(mapper);

	mesh_bundle_destroy(mb);
}

void 
convert_mesh(
		const char *input_path, 
		char **mesh_names, u32 mesh_count, 
		const char *output_path)
{	
	set_input_file_type(input_path, &input_file_type);

	scene = aiImportFile(input_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		printf("Failed to load file %s\n%s\n", input_path, aiGetErrorString());
		return;
	}

	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *am = scene->mMeshes[i];

		MeshData mesh;
		memset(mesh.name, '\0', MAX_NAME_LEN);

		strcpy(mesh.name, am->mName.data);
		if (input_file_type == OBJ)
		{
			char *n_name = strrchr(mesh.name, '_');
			if (n_name) *n_name = '\0';
		}

		if (mesh_names != NULL)
		{
			for (u32 mc = 0; mc < mesh_count; mc++)
			{
				if (strcmp(mesh.name, mesh_names[mc]) == 0) process_mesh(am, &mesh);
			}
		}
		else
		{
			process_mesh(am, &mesh);
		}

	}

	printf("Mesh count: %u\n", vector_count(meshes));

	for (u32 i = 0; i < vector_count(meshes); i++)
	{
		printf("--- Name: %s \nVertex Count: %u\nIndice Count: %u\n", 
				meshes[i].name, 
				meshes[i].vert_count, meshes[i].indice_count);	

		//for (u32 v = 0; v < vector_count(meshes[i].verts); v++)
		//{
		//	vertex *vert = &meshes[i].verts[v];
		//	printf("pos: %f %f %f\nuv: %f %f\nnorm: %f %f %f\n\n",
		//	vert->pos[0], vert->pos[1], vert->pos[2],
		//	vert->uv[0], vert->uv[1],
		//	vert->norm[0], vert->norm[1], vert->norm[2]);
		//}
	}

	export_bundle(output_path);

	cleanup();
}

