#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void cli_graph_import_tuples_edge(char* fl, int* pos, int s_id, edge_t e);

void
cli_graph_import_edge(char* fl, int* pos, graph_t* r_g)
{
	//collect id1
	char id1[BUFSIZE];
	memset(id1, 0 , BUFSIZE);
	nextarg(fl, pos, ITEM_SEP, id1);

	//collect id2
	char id2[BUFSIZE];
	memset(id2, 0 , BUFSIZE);
	nextarg(fl, pos, ITEM_SEP, id2);

	//collect schema id
	char s_id[BUFSIZE];
	memset(s_id, 0, BUFSIZE);
	nextarg(fl, pos, DEF_SEP, s_id);

	edge_t edge = (edge_t)malloc(sizeof(struct vertex));
	edge_init(edge);
	edge->id1 = strtoll(id1, NULL, 10);
	edge->id2 = strtoll(id2, NULL, 10);

	//collect tuple values if schema is specified
	if(strcmp(s_id, "N") != 0)
		cli_graph_import_tuples_edge(fl, pos, strtol(s_id, NULL, 10), edge);

	graph_insert_edge(*r_g, edge);
}
