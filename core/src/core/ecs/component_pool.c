#include <assert.h>
#include <string.h>
#include "component_pool.h"

ComponentPool
component_pool_make(u32 size, u32 count)
{
	ComponentPool pool;
	pool.data        = malloc(size * count);
	pool.count       = count;
	pool.size        = size;
    pool.indexes     = stack_make(count);

	for (u32 i = count; i --> 0; )
    {
        stack_push(pool.indexes, i);
    }

	return pool;
}

void
component_pool_destroy(ComponentPool *pool)
{
	free(pool->data);
    stack_destroy(pool->indexes);
}


