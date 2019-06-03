#include "model_module.h"
#include "mesh/mesh_module.h"

static vector(Model*) models = VECTOR_EMPTY;

void 
model_module_init(void)
{

}

void 
model_module_cleanup(void)
{
	vector_free_all(models, model_destroy);
}

void
model_module_add_model(Model *model)
{
	vector_push(models, model);
	mesh_module_add_meshes(model->meshes, vector_count(model->meshes));
}

void
model_module_remove_model(Model *model)
{
	vector_remove_eq(models, model);
	mesh_module_remove_meshes(model->meshes, vector_count(model->meshes));
}

void
model_module_update(void)
{
	for (u32 i = 0; i < vector_count(models); i++)
	{
		model_update(models[i]);
	}
}
