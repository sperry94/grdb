#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_import_edge(char* fl, int* pos, graph_t r_g, s_list_t s_list);

void cli_import_schema(char* fl, int* pos, s_list_t s_list);
void cli_nf_schema_print_list(s_list_t s_list);

void cli_import_vertex(char* fl, int* pos, graph_t r_g, s_list_t s_list);

void graph_normalize(graph_t in, graph_t out);

void schema_remove_union(schema_t s1, schema_t s2, schema_t u_s);

void
cli_import_line(char* fl, graph_t r_g, s_list_t s_list)
{
	char t[BUFSIZE];
	memset(t, 0, BUFSIZE);

	int pos = 0;
	nextarg(fl, &pos, DEF_SEP, t);

	if(strcmp(t, "S") == 0)
		cli_import_schema(fl, &pos, s_list);
	else if(strcmp(t, "V") == 0)
		cli_import_vertex(fl, &pos, r_g, s_list);
	else if(strcmp(t, "E") == 0)
		cli_import_edge(fl, &pos, r_g, s_list);
	else
	{
		printf("Unexpected type\n");
	}
}

void
cli_import(char* cmdline, int* pos)
{
	char fn[BUFSIZE];
	memset(fn, 0, BUFSIZE);
	nextarg(cmdline, pos, DEF_SEP, fn);

	char* ext = strstr(fn, ".grdb");
  if(ext == NULL || strlen(ext) != 5) {
      printf("Import file must have extension .grdb\n");
      return;
  }

	FILE* fd;
	if((fd = fopen(fn, "r")) == NULL)
	{
		perror("Error opening file");
		return;
	}

	graph_t r_g = (graph_t)malloc(sizeof(struct graph));
	memset(r_g, 0, sizeof(struct graph));

	s_list_t s_list =  (s_list_t)malloc(sizeof(struct s_list));
	memset(s_list, 0, sizeof(struct s_list));

	char fl[BUFSIZE];
	memset(fl, 0, BUFSIZE);

	while(fgets(fl, BUFSIZE, fd) != NULL)
	{
		cli_import_line(fl, r_g, s_list);
		memset(fl, 0, BUFSIZE);
	}

	if(ferror(fd) != 0)
		perror("Error reading file");
	else if(feof(fd) == 0)
		printf("Error reading file: Did not read entire file.");

	graph_print(r_g, 1);
	printf("\n");

	for(vertex_t v=r_g->v; v != NULL; v=v->next)
	{
		vertex_t ns = graph_find_neighbor_ids(r_g, v->id);

		for(vertex_t n=ns; n != NULL; n=n->next)
		{
			schema_t n_s_u = (schema_t)malloc(sizeof(struct schema));
			memset(n_s_u, 0, sizeof(struct schema));

			schema_remove_union(v->tuple->s, n->tuple->s, n_s_u);

			schema_print(v->tuple->s);
			printf("\n");
			schema_print(n->tuple->s);
			printf("\n");
			schema_print(n_s_u);
			printf("\n");
			// handle the neighbors
		}

		//deep free running graph, schema list, neighbor id list
	}
}
