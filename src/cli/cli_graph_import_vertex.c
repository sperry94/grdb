#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void cli_graph_import_tuples(char* fl, int* pos, int s_id, tuple_t tuple);

void
cli_graph_import_vertex(char* fl, int* pos, graph_t r_g)
{
	//collect id
	char id[BUFSIZE];
	memset(id, 0 , BUFSIZE);
	nextarg(fl, pos, DEF_SEP, id);

	//collect schema id
	char s_id[BUFSIZE];
	memset(s_id, 0, BUFSIZE);
	nextarg(fl, pos, DEF_SEP, s_id);



	vertex_t vertex = (vertex_t)malloc(sizeof(struct vertex));
	vertex_init(vertex);
	vertex->id = strtoll(id, NULL, 10);
	vertex->tuple = (tuple_t)malloc(sizeof(struct tuple));

	//collect tuple values if schema is specified
	if(strcmp(s_id, "N") != 0)
		cli_graph_import_tuples(fl, pos, strtol(s_id, NULL, 10), vertex->tuple);

	graph_insert_vertex(r_g, vertex);
}
