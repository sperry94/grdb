#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_export_edges(graph_t g, e_s_map_t edg_maps, FILE* out);
void cli_export_schemas(schema_list_t* s_list, FILE* out);
void cli_export_vertices(graph_t g, v_s_map_t vtx_maps, FILE* out);

v_s_map_t cli_export_collect_vtx_schemas(graph_t g, schema_list_t* s_list, int* s_id);
e_s_map_t cli_export_collect_edg_schemas(graph_t g, schema_list_t* s_list, int* s_id);

void cli_export_collect_full_graph(graph_t cg);

void
cli_export(char *cmdline, int *pos)
{
  char fn[BUFSIZE];
	memset(fn, 0, BUFSIZE);
	nextarg(cmdline, pos, DEF_SEP, fn);

  char* ext = strstr(fn, ".grdb");
  if(ext == NULL || strlen(ext) != 5) {
      printf("Export file must have extension .grdb\n");
      return;
  }

  graph_t cg = (graph_t)malloc(sizeof(struct graph));
  graph_init(cg);

  cli_export_collect_full_graph(cg);

  int s_id = 0;
  schema_list_t* s_list = (schema_list_t*)malloc(sizeof(struct schema_list));
  memset(s_list, 0, sizeof(struct schema_list));

  v_s_map_t vtx_maps = cli_export_collect_vtx_schemas(cg, s_list, &s_id);
  e_s_map_t edg_maps = cli_export_collect_edg_schemas(cg, s_list, &s_id);

  FILE* fd;

  if((fd=fopen(fn, "w+")) == NULL)
  {
    perror("Error opening file");
		return;
  }

  cli_export_schemas(s_list, fd);

  cli_export_vertices(cg, vtx_maps, fd);

  cli_export_edges(cg, edg_maps, fd);

  fclose(fd);
}
