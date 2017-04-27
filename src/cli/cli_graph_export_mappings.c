#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void
cli_graph_export_mappings_print_vtx(vtx_schema_map_list_t vtx_maps)
{
  for(vtx_schema_map_list_t vm=vtx_maps; vm != NULL; vm=vm->next)
  {
    printf("%llu-%d\n", vm->id, vm->s_id);
  }
}

void
cli_graph_export_mappings_print_edg(edg_schema_map_list_t edg_maps)
{
  for(edg_schema_map_list_t em=edg_maps; em != NULL; em=em->next)
  {
    printf("%llu:%llu-%d\n", em->id1, em->id2, em->s_id);
  }
}

vtx_schema_map_list_t
cli_graph_export_mappings_vtx_lookup(vtx_schema_map_list_t vtx_maps,
  vertexid_t v_id)
{
  for(vtx_schema_map_list_t vm=vtx_maps; vm != NULL; vm=vm->next)
  {
    if(vm->id == v_id)
      return vm;
  }

  return NULL;
}

edg_schema_map_list_t
cli_graph_export_mappings_edg_lookup(edg_schema_map_list_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2)
{
  for(edg_schema_map_list_t em=edg_maps; em != NULL; em=em->next)
  {
    if(em->id1 == e_id1 && em->id2 == e_id2)
      return em;
  }

  return NULL;
}

void
cli_graph_export_mappings_insert_vtx(vtx_schema_map_list_t vtx_maps,
  vertexid_t v_id, int s_id)
{
  vtx_schema_map_list_t vm;
  for(vm=vtx_maps; vm->next != NULL; vm=vm->next);

  vm->next = (vtx_schema_map_list_t)malloc(sizeof(struct vtx_schema_map_list));
  memset(vm->next, 0, sizeof(struct vtx_schema_map_list));

  vm->next->id = v_id;
  vm->next->s_id = s_id;
}

void
cli_graph_export_mappings_insert_edg(edg_schema_map_list_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id)
{
  edg_schema_map_list_t em;
  for(em=edg_maps; em->next != NULL; em=em->next);

  em->next = (edg_schema_map_list_t)malloc(sizeof(struct edg_schema_map_list));
  memset(em->next, 0, sizeof(struct edg_schema_map_list));

  em->next->id1 = e_id1;
  em->next->id2 = e_id2;
  em->next->s_id = s_id;
}