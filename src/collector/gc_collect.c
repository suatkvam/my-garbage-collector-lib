#include "internal_collector.h"

/*
	*run garbage collection cycle
	*performs mark-and-sweep to free unreachable allocations,
	*updates last_collect_count for auto-collection tracking
*/

void gc_collect(t_gc_context *contex)
{
	if(!contex)
		return ;
	gc_mark(contex);
	gc_sweep(contex);
	contex->last_collect_count = contex->allocation_count;
}
