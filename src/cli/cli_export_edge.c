#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_export_map_print_edg(e_s_map_t edg_maps);
e_s_map_t cli_export_map_find_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2);
void cli_export_print_tuples(tuple_t t, FILE* out);

void
cli_export_edges(graph_t g, e_s_map_t edg_maps, FILE* out)
{
  for(edge_t e=g->e; e != NULL; e=e->next)
  {
    char el[BUFSIZE];
    memset(el, 0, BUFSIZE);

    e_s_map_t em =
      cli_export_map_find_edg(edg_maps, e->id1, e->id2);
    fprintf(out, "E %llu:%llu %d", e->id1, e->id2, em->s_id);

    cli_export_print_tuples(e->tuple, out);
  }
}