#include "system.h"
#include "entity_master.h"
#include "../ext/logger.h"

#define SYSTEM_MASK_MOVEMENT (MASK(COMPONENT_TRANSFORM) | MASK(COMPONENT_VELOCITY))
void 
system_movement(EntityMaster *master)
{
	//for (u32 i = 0; i < id_pool_max_id(master->id_pool); i++)
	//{
	//	EntityData *ed = &master->entities[i];
	//	Entity e = E_ID(i, ed->version);

	//	if (!(ed->component_mask & SYSTEM_MASK_MOVEMENT)) continue;

	//	CTransform *transform = entity_get_component(master, e, COMPONENT_TRANSFORM);
	//	CVelocity *velocity   = entity_get_component(master, e, COMPONENT_VELOCITY);

	//	glm_vec3_print(transform->pos, stdout);
	//	
	//	entity_print(master, e);
	//}
}

