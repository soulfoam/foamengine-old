#pragma once

#include <cglm/cglm.h>
#include "../ext/sugar.h"
#include "component.h"
#include "component_pool.h"
#include "../ext/stack.h"


#define E_ID(index, ver) (((u64)ver << 32) | index)
#define E_INDEX(id) ((u32)id)
#define E_VER(id) ((u32)(id >> 32))

#define MAX_ENTITIES 3

#define CTRANSFORM_POOL_SIZE 1000000
#define CVELOCITY_POOL_SIZE  200

typedef u64 Entity;

typedef struct
EntityMaster
{	
    Stack *indexes;
    u32   versions[MAX_ENTITIES];
    u32   components[MAX_ENTITIES][COMPONENT_COUNT];
    u64   component_masks[MAX_ENTITIES];

	ComponentPool pool[COMPONENT_COUNT];

} EntityMaster;

EntityMaster* entity_master_make(void);
void          entity_master_destroy(EntityMaster *master);
void          entity_master_run_systems(EntityMaster *master);
Entity        entity_make(EntityMaster *master);
void          entity_destroy(EntityMaster *master, Entity e);
bool          entity_valid(EntityMaster *master, Entity e);
void          entity_add_component(EntityMaster *master, Entity e, ComponentType type, void *component_data);
void          entity_remove_component(EntityMaster *master, Entity e, ComponentType type);
void*         entity_get_component(EntityMaster *master, Entity e, ComponentType type);
u64           entity_get_mask(EntityMaster *master, Entity e);
u32           entity_get_version(EntityMaster *master, Entity e);
void          entity_print(EntityMaster *master, Entity e);
