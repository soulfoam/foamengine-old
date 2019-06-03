#include <cglm/cglm.h>
#include <core/ext/logger.h>
#include <string.h>
#include "../shader/shader.h"
#include "mesh.h"
#include "mesh_module.h"
#include "../shader/shader_module.h"

#define VERTS_IN_BYTES(a)   (a * sizeof(Vertex))
#define INDICES_IN_BYTES(a) (a * sizeof(u32))

static const char *S_MODEL      = "model";
static const char *S_MESH_COLOR = "mesh_color";

Mesh*
mesh_make(MeshData *mesh_data)
{
	Mesh *mesh            = malloc(sizeof(*mesh));
	strcpy(mesh->name, mesh_data->name);
	mesh->vert_count      = mesh_data->vert_count;
	mesh->indice_count    = mesh_data->indice_count;
	mesh->instance.count  = 0;
	mesh->visible         = true;
	mesh->tex             = NULL;
	mesh->tex_index       = 0;
	mesh->shader_type     = SHADER_DEFAULT;
	mesh->render_flags    = 0;
	mesh->render_flags   |= RENDER_NORMAL;

	mesh_set_color(mesh, (vec4){1, 1, 1, 1});

	glm_vec3_zero(mesh->pos);
	glm_quat_identity(mesh->rot);
	glm_vec3_one(mesh->scale);
	glm_mat4_identity(mesh->matrix);

	glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    glGenBuffers(1, &mesh->instance.pos_buffer);

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(
			GL_ARRAY_BUFFER, 
			VERTS_IN_BYTES(mesh_data->vert_count), 
			mesh_data->verts, 
			GL_STATIC_DRAW);

	u32 slot = 0;
	glVertexAttribPointer(slot++, 3, GL_FLOAT, GL_FALSE, 
			sizeof(Vertex), (void*)0);
	glVertexAttribPointer(slot++, 2, GL_FLOAT, GL_FALSE, 
			sizeof(Vertex), (void*)(3 * sizeof(f32)));
	glVertexAttribPointer(slot++, 3, GL_FLOAT, GL_FALSE, 
			sizeof(Vertex), (void*)(5 * sizeof(f32)));		

	for (u32 i = 0; i < slot; i++) glEnableVertexAttribArray(i);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			INDICES_IN_BYTES(mesh_data->indice_count), 
			mesh_data->indices, 
			GL_STATIC_DRAW);

	glBindVertexArray(0);

	return mesh;
}

void
mesh_destroy(Mesh *mesh)
{
	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);
	free(mesh);
}

static void
bind_render_flags(Mesh *mesh)
{
	if (mesh->render_flags & RENDER_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

static void
unbind_render_flags(Mesh *mesh)
{	
	if (mesh->render_flags & RENDER_TRANSPARENT)
	{
		glDisable(GL_BLEND);
	}
}

void
mesh_render(Mesh *mesh)
{
	assert(mesh);

	if (!mesh->visible) return;
	
	Shader active_shader = shader_module_get_active_shader();
	shader_set_mat4(active_shader, S_MODEL, mesh->matrix);
	shader_set_vec4(active_shader, S_MESH_COLOR, mesh->color);

	bind_render_flags(mesh);

	glBindVertexArray(mesh->vao);

//	texture_bind(&mdl->tex.tex);

	if (mesh->instance.count > 0)
	{
		glDrawElementsInstanced(GL_TRIANGLES, 
			mesh->indice_count, 
			GL_UNSIGNED_INT, 0,
			mesh->instance.count);
	}
	else
	{
//		log_debug("%u", vector_count(mesh->data.indices));
		glDrawElements(GL_TRIANGLES, 
			mesh->indice_count, 
			GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, vector_count(mesh->data.verts));
	}

//	texture_unbind(&mdl->tex.tex);

	glBindVertexArray(0);

	unbind_render_flags(mesh);
}


void
mesh_build_instance_buffer(Mesh *mesh, u32 count, mat4 *positions)
{
	mesh->instance.count = count;	

	glBindBuffer(GL_ARRAY_BUFFER, mesh->instance.pos_buffer);
	glBufferData(GL_ARRAY_BUFFER, 
			mesh->instance.count * sizeof(mat4), 
			positions, 
			GL_STATIC_DRAW);

	glBindVertexArray(mesh->vao);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(sizeof(vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(2 * sizeof(vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(3 * sizeof(vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	mesh->shader_type = SHADER_DEFAULT_INSTANCING;

}

void
mesh_clear_instance_buffer(Mesh *mesh)
{
	//vector_free(mesh->instance.positions);
}

void
mesh_set_color(Mesh *mesh, vec4 color)
{
	assert(mesh);

	glm_vec4_copy(color, mesh->color);	
}

void
mesh_get_texture_offset(Mesh *mesh, vec2 dst)
{
	s32 column = mesh->tex_index % mesh->tex->num_of_rows;
	dst[0] = (f32)(column / mesh->tex->num_of_rows);

	s32 row = mesh->tex_index / mesh->tex->num_of_rows;
	dst[1] = (f32)(row / mesh->tex->num_of_rows);
}

