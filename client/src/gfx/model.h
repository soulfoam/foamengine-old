#pragma once

#include <cglm/cglm.h>
#include <core/ext/sugar.h>
#include <core/ext/vector.h>
#include <core/data/skeleton_data.h>
#include "mesh/mesh.h"
#include "skeleton.h"

typedef struct
{	
	vec3 pos;
	versor rot;
	vec3 scale;
	
	vector(Mesh*) meshes;
	u32 mesh_count;
	Skeleton *skeleton;

} Model;

Model* model_make(vec3 pos, versor rot);
void   model_destroy(Model *mdl);
void   model_update(Model *mdl);
void   model_render(Model *mdl);
void   model_pos_set(Model *mdl, vec3 pos);
void   model_set_skeleton(Model *mdl, Skeleton *skel);
void   model_add_mesh(Model *mdl, Mesh *mesh);
void   model_remove_mesh(Model *mdl, Mesh *mesh);
void   model_remove_all_meshes(Model *mdl);
void   model_add_mesh_bundle(Model *mdl, const MeshBundle *bundle);
void   model_remove_mesh_bundle(Model *mdl, const MeshBundle *bundle);
Mesh*  model_get_mesh(Model *mdl, const char *mesh_str);
void   model_print(Model *mdl);
