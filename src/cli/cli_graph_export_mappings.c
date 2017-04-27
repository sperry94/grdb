#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void
cli_graph_export_mappings_insert_vtx(vtx_schema_map_list_t* vtx_maps, vertexid_t v_id, int s_id)
{
  vtx_schema_map_list_t* vm;
  for(vm=vtx_maps; vm->next != NULL; vm=vm->next);

  vm = (vtx_schema_map_list_t*)malloc(sizeof(struct vtx_schema_map_list));
  memset(vm, 0, sizeof(struct vtx_schema_map_list));

  vm->id = v_id;
  vm->s_id = s_id;
}

void
cli_graph_export_mappings_insert_edg(edg_schema_map_list_t* edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id)
{
  edg_schema_map_list_t* em;
  for(em=edg_maps; em->next != NULL; em=em->next);

  em = (edg_schema_map_list_t*)malloc(sizeof(struct edg_schema_map_list));
  memset(em, 0, sizeof(struct edg_schema_map_list));

  em->id1 = e_id1;
  em->id2 = e_id2;
  em->s_id = s_id;
}