#include "../ext/io.h"
#include "../ext/logger.h"
#include "../ext/binary_mapper_ext.h"
#include "mesh_bundle.h"

const u32 MODEL_VER       = 1;
const char *MB_FORMAT_EXT = "mb";

static void
mesh_data_destroy(MeshData *mesh_data)
{
	free(mesh_data->verts);
	free(mesh_data->indices);
}

MeshBundle*
mesh_bundle_make(const char *path)
{
	char f_path[4096];
	memset(f_path, '\0', 4096);
	sprintf(f_path, "%s.%s", path, MB_FORMAT_EXT);
	
	MeshBundle *bundle = malloc(sizeof(*bundle));
	bundle->path       = strdup(path);

	BinaryMapper *mapper = binary_mapper_load(f_path);
	if (mapper == NULL)
	{
		log_fatal("mesh_bundle: (%s) doesn't exist / couldn't be read from the disk!", f_path);
		return NULL;
	}

	bm_map_mesh_bundle(mapper, bundle);

	if (bundle->version != MODEL_VER)
	{
		log_fatal("mesh_bundle_make: (%s) is version %u and engine version is %u", 
				path, bundle->version, MODEL_VER);
		return NULL;
	} 
	
	binary_mapper_destroy(mapper);

	return bundle;
}

void
mesh_bundle_destroy(MeshBundle *bundle)
{
	for (u32 i = 0; i < bundle->mesh_count; i++) mesh_data_destroy(&bundle->meshes[i]);
	free(bundle->path);
	free(bundle);
}

MeshData*
mesh_bundle_get_mesh_data(const MeshBundle *bundle, const char *mesh_str)
{
	for (u32 i = 0; i < bundle->mesh_count; i++)
	{
		if (strcmp(mesh_str, bundle->meshes[i].name) == 0) return &bundle->meshes[i];
	}
	
	return NULL;
}

void
mesh_bundle_print(const MeshBundle *bundle)
{
	u32 mesh_count = bundle->mesh_count;
	log_debug("Mesh set: (%s) meshes (%u)", bundle->path, mesh_count);

	for (u32 i = 0; i < mesh_count; i++)
	{
		MeshData *data = &bundle->meshes[i];

		log_debug("Mesh: (%s) ... Vertex count: (%u) ... Indice count: (%u)", 
			data->name, data->vert_count, data->indice_count);
	}
}

