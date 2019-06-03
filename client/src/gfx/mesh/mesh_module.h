#pragma once

#include "../camera.h"
#include "mesh.h"

void mesh_module_init(void);
void mesh_module_cleanup(void);
void mesh_module_add_mesh(Mesh *mesh);
void mesh_module_add_meshes(Mesh **mesh, u32 count);
void mesh_module_remove_mesh(Mesh *mesh);
void mesh_module_remove_meshes(Mesh **mesh, u32 count);
void mesh_module_render(Camera *cam);

