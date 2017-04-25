#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

schema_t* cli_graph_import_schema_find_by_id(int s_id);

void
cli_graph_import_tuples(char* fl, int* pos, schema_t s, tuple_t tuple)
{
	//collect field name
	char n[ATTR_NAME_MAXLEN];
	memset(n, 0, ATTR_NAME_MAXLEN);
	nextarg(fl, pos, ITEM_SEP, n);

	//collect field value
	char v[BUFSIZE];
	memset(v, 0, BUFSIZE);
	nextarg(fl, pos, ITEM_SEP, v);

	while(strlen(n) > 0 && strlen(v) > 0)
	{
		printf("%s - %s: ", n, v);

		base_types_t bt = schema_find_type_by_name(s, n);

		tuple_set(tuple, n, v);

		printf("%d\n", bt);

		//collect field name
		memset(n, 0, ATTR_NAME_MAXLEN);
		nextarg(fl, pos, ITEM_SEP, n);

		//collect field value
		memset(v, 0, BUFSIZE);
		nextarg(fl, pos, ITEM_SEP, v);
	}
}

void
cli_graph_import_tuples_vertex(char* fl, int* pos, int s_id, vertex_t v)
{
	schema_t* s = cli_graph_import_schema_find_by_id(s_id);

	v->tuple = (tuple_t)malloc(sizeof(struct tuple));

	tuple_init(v->tuple, *s);

	cli_graph_import_tuples(fl, pos, *s, v->tuple);
}

void
cli_graph_import_tuples_edge(char* fl, int* pos, int s_id, edge_t e)
{
	schema_t* s = cli_graph_import_schema_find_by_id(s_id);

	e->tuple = (tuple_t)malloc(sizeof(struct tuple));

	tuple_init(e->tuple, *s);

	//cli_graph_import_tuples(fl, pos, *s, e->tuple);
}
