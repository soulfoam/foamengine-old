#pragma once

#include <cglm/cglm.h>
#include "../ext/sugar.h"
#include "../ext/stack.h"

typedef struct
{
	void *data;
	u32 count;
	u32 size;

    Stack *indexes;
	
} ComponentPool;

ComponentPool component_pool_make(u32 size, u32 count);
void		  component_pool_destroy(ComponentPool *pool);

