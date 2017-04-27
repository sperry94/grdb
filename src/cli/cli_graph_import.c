#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void cli_graph_import_edge(char* fl, int* pos, graph_t r_g);

void cli_graph_import_schema(char* fl, int* pos);
void cli_graph_import_schema_print_list(schema_list_t* s_list);

void cli_graph_import_vertex(char* fl, int* pos, graph_t r_g);

void graph_normalize(graph_t in, graph_t out);

void
cli_graph_import_line(char* fl, graph_t r_g)
{
	char t[BUFSIZE];
	memset(t, 0, BUFSIZE);

	int pos = 0;
	nextarg(fl, &pos, DEF_SEP, t);

	if(strcmp(t, "S") == 0)
		cli_graph_import_schema(fl, &pos);
	else if(strcmp(t, "V") == 0)
		cli_graph_import_vertex(fl, &pos, r_g);
	else if(strcmp(t, "E") == 0)
		cli_graph_import_edge(fl, &pos, r_g);
	else
	{
		printf("Unexpected type\n");
	}
}

void
cli_graph_import(char* cmdline, int* pos)
{
	char fn[BUFSIZE];

	memset(fn, 0, BUFSIZE);
	nextarg(cmdline, pos, DEF_SEP, fn);

	printf("%s:\n", fn);

	FILE* fd;
	if((fd = fopen(fn, "r")) == NULL)
	{
		perror("Error opening file");
		return;
	}

	graph_t r_g = (graph_t)malloc(sizeof(struct graph));
	memset(r_g, 0, sizeof(struct graph));

	char fl[BUFSIZE];
	memset(fl, 0, BUFSIZE);

	while(fgets(fl, BUFSIZE, fd) != NULL)
	{
		cli_graph_import_line(fl, r_g);
		memset(fl, 0, BUFSIZE);
	}

	if(ferror(fd) != 0)
		perror("Error reading file");
	else if(feof(fd) == 0)
		printf("Error reading file: Did not read entire file.");

	graph_print(r_g, 1);
	printf("\n");

	cli_graph_import_schema_print_list(s_list);

	graph_t norm = (graph_t)malloc(sizeof(struct graph));
	graph_init(norm);
	graph_normalize(r_g, norm);
}
