#include <gl.h>
#include <core/ext/logger.h>
#include <string.h>
#include "terrain.h"

//static const f32 SIZE         = 800;
//static const u32 VERTEX_COUNT = 128;

static f32 x = 0;
static f32 y = 0;

terrain_t*
terrain_make(vec3 pos, f32 size, u32 vertex_count)
{
	terrain_t *terrain = malloc(sizeof(*terrain));

	glm_vec3_copy(pos, terrain->pos);

	vector(Vertex) verts = VECTOR_EMPTY;
//	for (u32 i = 0; i < vertex_count; i++)
//	{
//		for (u32 j = 0; j < vertex_count; j++)
//		{
//			Vertex v;
//			v.pos[0]  = (f32)j / ((f32)vertex_count - 1) * size;
//			v.pos[1]  = (f32)i / ((f32)vertex_count - 1) * size;
//			v.pos[2]  = 0;
//			v.norm[0] = 0;
//			v.norm[1] = 0;
//			v.norm[2] = 1;
//			v.uv[0]   = (f32)j / ((f32)vertex_count - 1);
//			v.uv[1]   = (f32)i / ((f32)vertex_count - 1);
//			vector_push(verts, v);
//		}
//	}
	
	vector(u32) indices = VECTOR_EMPTY;
//	for (u32 gy = 0; gy < vertex_count - 1; gy++)
//	{
//		for (u32 gx = 0; gx < vertex_count - 1; gx++)
//		{
//			u32 top_left     = (gy * vertex_count) + gx;
//			u32 top_right    = top_left + 1;
//			u32 bottom_left  = ((gy + 1) * vertex_count) + gx;
//			u32 bottom_right = bottom_left + 1;
//			vector_push(indices, top_left);
//			vector_push(indices, bottom_left);
//			vector_push(indices, top_right);
//			vector_push(indices, top_right);
//			vector_push(indices, bottom_left);
//			vector_push(indices, bottom_right);
//		}
//	}
	
	for (u32 y = 0; y < 200; y++)
	{
		for (u32 x = 0; x < 200; x++)
		{			
			Vertex v;
			v.pos[0]  = x;
			v.pos[1]  = y;
			v.pos[2]  = 0;
			v.norm[0] = 0;
			v.norm[1] = 0;
			v.norm[2] = 1;
			v.uv[0]   = x;
			v.uv[1]   = y;
			vector_push(verts, v);
		}
	}
	for (u32 i = 0; i < 200 * 200; i++)
	{
		vector_push(indices, i);
	}

//	MeshData d = mesh_data_make("terrain", verts, indices, false);

	//MeshBundle *debug_bundle = mesh_bundle_make("res/models/debug");
	//MeshData *data = mesh_bundle_get_mesh_data(debug_bundle, "terrain");
//	terrain->mesh = mesh_make(&d);
//	terrain->mesh->shader_type = ShaderERRAIN;
//
//	vector_free(verts);
//	vector_free(indices);

	return terrain;
}

void
terrain_destroy(terrain_t *terrain)
{
	mesh_destroy(terrain->mesh);

	free(terrain);
}


