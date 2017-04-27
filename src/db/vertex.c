#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void
vertex_init(vertex_t v)
{
	assert(v != NULL);
	memset(v, 0, sizeof(struct vertex));
}

void
vertex_copy(vertex_t in, vertex_t out)
{
	out->id = in->id;
	out->tuple = (tuple_t)malloc(sizeof(struct tuple));

	schema_t s = (schema_t)malloc(sizeof(struct schema));
	memset(s, 0, sizeof(struct schema));
	schema_copy(in->tuple->s, s);
	tuple_init(out->tuple, s);

	out->next = in->next;
	out->prev = in->prev;
}

void
vertex_set_id(vertex_t v, vertexid_t id)
{
	assert(v != NULL);

	v->id = id;
}

void
vertex_print(vertex_t v)
{
	assert(v != NULL);

	//printf("%04llx", v->id);
	printf("%llu", v->id);
}
