#pragma once

#include "binary_mapper.h"
#include "../data/mesh_bundle.h"
#include "../data/skeleton_data.h"

void bm_map_mesh_bundle(BinaryMapper *mapper, MeshBundle *mb);
void bm_map_skeleton_data(BinaryMapper *mapper, SkeletonData *skel_data);

