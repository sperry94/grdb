#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void
cli_export_map_print_vtx(v_s_map_t vtx_maps)
{
  for(v_s_map_t vm=vtx_maps; vm != NULL; vm=vm->next)
  {
    printf("%llu-%d\n", vm->v_id, vm->s_id);
  }
}

v_s_map_t
cli_export_map_find_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id)
{
  for(v_s_map_t vm=vtx_maps; vm != NULL; vm=vm->next)
  {
    if(vm->v_id == v_id)
      return vm;
  }

  return NULL;
}

void
cli_export_mappings_insert_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id, int s_id)
{
  v_s_map_t vm;
  for(vm=vtx_maps; vm->next != NULL; vm=vm->next);

  vm->next = (v_s_map_t)malloc(sizeof(struct v_s_map));
  memset(vm->next, 0, sizeof(struct v_s_map));

  vm->next->v_id = v_id;
  vm->next->s_id = s_id;
}

void
cli_export_map_print_edg(e_s_map_t edg_maps)
{
  for(e_s_map_t em=edg_maps; em != NULL; em=em->next)
  {
    printf("%llu:%llu-%d\n", em->e_id1, em->e_id2, em->s_id);
  }
}

e_s_map_t
cli_export_map_find_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2)
{
  for(e_s_map_t em=edg_maps; em != NULL; em=em->next)
  {
    if(em->e_id1 == e_id1 && em->e_id2 == e_id2)
      return em;
  }

  return NULL;
}

void
cli_export_mappings_insert_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id)
{
  e_s_map_t em;
  for(em=edg_maps; em->next != NULL; em=em->next);

  em->next = (e_s_map_t)malloc(sizeof(struct e_s_map));
  memset(em->next, 0, sizeof(struct e_s_map));

  em->next->e_id1 = e_id1;
  em->next->e_id2 = e_id2;
  em->next->s_id = s_id;
}