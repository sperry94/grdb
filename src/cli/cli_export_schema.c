#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_import_schema_print_list(schema_list_t* s_list);
void cli_import_schema_insert(schema_list_t* s_list,
  schema_t schema, int s_id);

void cli_export_map_insert_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id, int s_id);

void cli_export_map_insert_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id);

void
cli_export_schemas(schema_list_t* s_list, FILE* out)
{
  for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
  {
    fprintf(out, "S %d", sl->s_id);

    for(attribute_t a=sl->s->attrlist; a != NULL; a=a->next)
    {
      fprintf(out, " %s:%s", base_types_str[a->bt], a->name);
    }
    fprintf(out, "\n");
  }
}

v_s_map_t
cli_export_collect_vtx_schemas(graph_t g, schema_list_t* s_list, int* s_id)
{
  v_s_map_t vtx_maps = NULL;
  for(vertex_t v=g->v; v != NULL; v=v->next)
  {
    short found = 0;
    for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
    {
      if(schema_compare(sl->s, v->tuple->s))
      {
        found = 1;
        break;
      }
    }
    if(!found)
    {
      cli_import_schema_insert(s_list, v->tuple->s, *s_id);
      if(vtx_maps == NULL)
      {
        vtx_maps = (v_s_map_t)
          malloc(sizeof(struct v_s_map));
        memset(vtx_maps, 0, sizeof(struct v_s_map));

        vtx_maps->v_id = v->id;
        vtx_maps->s_id = *s_id;
      }
      else
      {
        cli_export_map_insert_vtx(vtx_maps, v->id, *s_id);
      }
      ++(*s_id);
    }
  }

  return vtx_maps;
}

e_s_map_t
cli_export_collect_edg_schemas(graph_t g, schema_list_t* s_list, int* s_id)
{
  e_s_map_t edg_maps = NULL;
  for(edge_t e=g->e; e != NULL; e=e->next)
  {
    short found = 0;
    for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
    {
      if(schema_compare(sl->s, e->tuple->s))
      {
        found = 1;
        break;
      }
    }
    if(!found)
    {
      cli_import_schema_insert(s_list, e->tuple->s, *s_id);
      if(edg_maps == NULL)
      {
        edg_maps = (e_s_map_t)
          malloc(sizeof(struct e_s_map));
        memset(edg_maps, 0, sizeof(struct e_s_map));

        edg_maps->e_id1 = e->id1;
        edg_maps->e_id2 = e->id2;
        edg_maps->s_id = *s_id;
      }
      else
      {
        cli_export_map_insert_edg(edg_maps, e->id1, e->id2, *s_id);
      }
      ++(*s_id);
    }
  }
  return edg_maps;
}