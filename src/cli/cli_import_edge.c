#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_import_tuples(char* fl, int* pos, tuple_t tuple, s_list_t s_list, int s_id);

void
cli_import_edge(char* fl, int* pos, graph_t r_g, s_list_t s_list)
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

	edge_t edge = (edge_t)malloc(sizeof(struct edge));
	edge_init(edge);
	edge->id1 = strtoll(id1, NULL, 10);
	edge->id2 = strtoll(id2, NULL, 10);
	edge->tuple = (tuple_t)malloc(sizeof(struct tuple));

	//collect tuple values if schema is specified
	if(strcmp(s_id, "N") != 0)
		cli_import_tuples(fl, pos, edge->tuple, s_list, strtol(s_id, NULL, 10));

	graph_insert_edge(r_g, edge);
}
