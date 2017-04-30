#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_export_map_print_vtx(v_s_map_t vtx_maps);
v_s_map_t cli_export_map_find_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id);
void cli_export_print_tuples(tuple_t t, FILE* out);

void
cli_export_vertices(graph_t g, v_s_map_t vtx_maps, FILE* out)
{
  for(vertex_t v=g->v; v != NULL; v=v->next)
  {
    v_s_map_t vm = cli_export_map_find_vtx(vtx_maps, v->id);
    fprintf(out, "V %llu %d", v->id, vm->s_id);

    // tuple value export not supported
    //cli_export_print_tuples(v->tuple, out);
    fprintf(out, "\n");
  }
}