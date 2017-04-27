#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void
edge_init(edge_t e)
{
	assert(e != NULL);
	memset(e, 0, sizeof(struct edge));
}

void
edge_copy(edge_t in, edge_t out)
{
	out->id1 = in->id1;
	out->id2 = in->id2;
	out->tuple = (tuple_t)malloc(sizeof(struct tuple));

	schema_t s = (schema_t)malloc(sizeof(struct schema));
	memset(s, 0, sizeof(struct schema));
	schema_copy(in->tuple->s, s);
	tuple_init(out->tuple, s);

	out->next = in->next;
	out->prev = in->prev;
}

void
edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2)
{
	assert(e != NULL);

	e->id1 = id1;
	e->id2 = id2;
}

void
edge_print(edge_t e)
{
	assert(e != NULL);

	//printf("(%04llx,%04llx)", e->id1, e->id2);
	printf("(%llu,%llu)", e->id1, e->id2);
}
