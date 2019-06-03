#pragma once

#include <cglm/cglm.h>
#include "../ext/sugar.h"

#define MAX_NAME_LEN 64

typedef struct
{
	vec3 pos;
	vec2 uv;
	vec3 norm;
} Vertex;

typedef struct
{
	char name[MAX_NAME_LEN];

	Vertex *verts;
	u32 vert_count;
	u32 *indices;
	u32 indice_count;

	bool cached;

} MeshData;

typedef struct
{
	char *path;
	u32 version;
	MeshData *meshes;
	u32 mesh_count;
} MeshBundle;

MeshBundle* mesh_bundle_make(const char *path);
void        mesh_bundle_destroy(MeshBundle *set);
MeshData*   mesh_bundle_get_mesh_data(const MeshBundle *bundle, const char *mesh_str);
void        mesh_bundle_print(const MeshBundle *bundle);
