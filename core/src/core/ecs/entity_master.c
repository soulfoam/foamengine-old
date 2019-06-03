#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ext/logger.h"
#include "entity_master.h"
#include "system.h"

EntityMaster*
entity_master_make(void)
{
	EntityMaster *master = malloc(sizeof(*master));
    master->indexes = stack_make(MAX_ENTITIES);

	for (size_t i = MAX_ENTITIES; i --> 0;)
	{	
        stack_push(master->indexes, i);

        master->versions[i] = 0;
		for (u32 j = 0; j < COMPONENT_COUNT; j++)
        {
            master->components[i][j] = 0;
        }
        master->component_masks[i] = 0;
	}
    
	master->pool[COMPONENT_TRANSFORM] = component_pool_make(sizeof(CTransform), CTRANSFORM_POOL_SIZE);
	master->pool[COMPONENT_VELOCITY]  = component_pool_make(sizeof(CVelocity), CVELOCITY_POOL_SIZE);


	return master;
}

void
entity_master_destroy(EntityMaster *master)
{
	for (u32 i = 0; i < COMPONENT_COUNT; i++)
	{
		component_pool_destroy(&master->pool[i]);
	}

    stack_destroy(master->indexes);

	free(master);
}

void
entity_master_run_systems(EntityMaster *master)
{
	system_movement(master);
}


Entity
entity_make(EntityMaster *master)
{
    u32 index = stack_pop(master->indexes);
    u32 ver   = master->versions[index]; 

    return E_ID(index, ver);
}

void
entity_destroy(EntityMaster *master, Entity e)
{	
    u32 index = E_INDEX(e);

    master->versions[index]++;
	for (u32 i = 0; i < COMPONENT_COUNT; i++)
	{
		entity_remove_component(master, e, i);
	}
	master->component_masks[index] = 0;

    stack_push(master->indexes, index);

}

bool
entity_valid(EntityMaster *master, Entity e)
{
    return master->versions[E_INDEX(e)] == E_VER(e);
}

void
entity_add_component(EntityMaster *master, Entity e, ComponentType type, void *component_data)
{
    u32 index = E_INDEX(e);

	if (MASK(type) & master->component_masks[index])
	{
		log_warn("Component %d already exists on Entity %d", type, E_INDEX(e));
		return;
	}

	ComponentPool *pool = &master->pool[type];
    u32 c_index = stack_pop(pool->indexes);
	memcpy(&pool->data[c_index], component_data, pool->size);
	master->components[index][type] = c_index;
    master->component_masks[index] |= MASK(type);
}

void
entity_remove_component(EntityMaster *master, Entity e, ComponentType type)
{
    u32 index = E_INDEX(e);

	if (!(MASK(type) & master->component_masks[index]))
	{
		log_warn("Component %d doesn't exist on Entity %d", type, index);
		return;
	}

	ComponentPool *pool = &master->pool[type];
    stack_push(pool->indexes, master->components[index][type]);
    master->component_masks[index] &= ~MASK(type);
}

void*
entity_get_component(EntityMaster *master, Entity e, ComponentType type)
{	
    u32 index = E_INDEX(e);

    if (!(MASK(type) & master->component_masks[index]))
	{
		log_warn("Trying to get non existing Component %d on Entity %d", type, index);
		return NULL;
	}

	return &master->pool[type].data[master->components[index][type]];
}

void
entity_print(EntityMaster *master, Entity e)
{
    u32 index = E_INDEX(e);

	printf("---- Entity ----\nIndex: %d\nVersion: %d\nMask: ", E_INDEX(e), master->versions[index]);

    printf("%lu\n", master->component_masks[index]);

	for (u32 i = 0; i < COMPONENT_COUNT; i++)
	{
		printf("Component Type: %d (Index: %d)\n", i, master->components[index][i]);
	}

	printf("----------------\n");

}

