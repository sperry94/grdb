#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_import(char *cmdline, int *pos)
{
	char filename[BUFSIZE];

	memset(filename, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", filename);

	printf("filename=%s\n", filename);

	FILE* fd;
	if((fd = fopen(filename, "r")) == NULL)
	{
		printf("Error opening file.");
		return;
	}

	char fileline[BUFSIZE];

	memset(fileline, 0, BUFSIZE);
	fgets(fileline, BUFSIZE, fd);

	printf("fileline=%s", fileline);

	// read a graph file
	// create new graphs with vertices and edges
	// add schemas

	/* create new graph code
	graph_t g;
	vertex_t v;

	// Create first vertex in graph
	v = (vertex_t) malloc(sizeof(struct vertex));
	assert (v != NULL);
	vertex_init(v);
	v->id = 1;

	// Create new graph
	g = (graph_t) malloc(sizeof(struct graph));
	assert (g != NULL);
	graph_init(g);
	graph_insert_vertex(g, v);

	if (graphs == NULL)
		current = g;
	cli_graphs_insert(g);
	*/
}
