#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void cli_graph_import_schema_print_list(schema_list_t* s_list);
void cli_graph_import_schema_insert(schema_list_t* s_list,
  schema_t schema, int s_id);

void cli_graph_export_mappings_print_vtx(vtx_schema_map_list_t vtx_maps);
void cli_graph_export_mappings_insert_vtx(vtx_schema_map_list_t vtx_maps,
  vertexid_t v_id, int s_id);
vtx_schema_map_list_t cli_graph_export_mappings_vtx_lookup(vtx_schema_map_list_t vtx_maps,
  vertexid_t v_id);

void cli_graph_export_mappings_print_edg(edg_schema_map_list_t edg_maps);
void cli_graph_export_mappings_insert_edg(edg_schema_map_list_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id);
edg_schema_map_list_t cli_graph_export_mappings_edg_lookup(edg_schema_map_list_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2);

void
cli_graph_export(char *cmdline, int *pos)
{
  graph_t cg = (graph_t)malloc(sizeof(struct graph));
  graph_init(cg);

  for(graph_t g = graphs; g != NULL; g=g->next)
  {
    for(vertex_t v=g->v; v != NULL; v=v->next)
    {
      vertex_t nv = (vertex_t)malloc(sizeof(struct vertex));
      memset(nv, 0, sizeof(struct vertex));
      vertex_copy(v, nv);
      nv->prev = NULL;
      nv->next = NULL;

      schema_print(nv->tuple->s);
      printf("\n");

      vertex_t gv = graph_find_vertex_by_id(cg, nv->id);
      if(gv == NULL)
      {
        graph_insert_vertex(cg, nv);
      }
      else
      {
        schema_attribute_insert(gv->tuple->s, nv->tuple->s->attrlist);
      }
    }

    for(edge_t e=g->e; e != NULL; e=e->next)
    {
      edge_t ne = (edge_t)malloc(sizeof(struct edge));
      memset(ne, 0, sizeof(struct edge));
      edge_copy(e, ne);
      ne->prev = NULL;
      ne->next = NULL;

      schema_print(ne->tuple->s);
      printf("\n");

      edge_t ge = graph_find_edge_by_ids(cg, ne->id1, ne->id2);
      if(ge == NULL)
      {
        graph_insert_edge(cg, ne);
      }
      else
      {
        schema_attribute_insert(ge->tuple->s, ne->tuple->s->attrlist);
      }
    }
  }

  graph_print(cg, 1);
  printf("\n");

  int s_id = 0;
  schema_list_t* s_list = (schema_list_t*)malloc(sizeof(struct schema_list));
  memset(s_list, 0, sizeof(struct schema_list));

  vtx_schema_map_list_t vtx_maps = NULL;
  edg_schema_map_list_t edg_maps = NULL;

  // add schemas to list
  for(vertex_t v=cg->v; v != NULL; v=v->next)
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
      cli_graph_import_schema_insert(s_list, v->tuple->s, s_id);
      if(vtx_maps == NULL)
      {
        vtx_maps = (vtx_schema_map_list_t)
          malloc(sizeof(struct vtx_schema_map_list));
        memset(vtx_maps, 0, sizeof(struct vtx_schema_map_list));

        vtx_maps->id = v->id;
        vtx_maps->s_id = s_id;
      }
      else
      {
        cli_graph_export_mappings_insert_vtx(vtx_maps, v->id, s_id);
      }
      ++s_id;
    }
  }

  for(edge_t e=cg->e; e != NULL; e=e->next)
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
      cli_graph_import_schema_insert(s_list, e->tuple->s, s_id);
      if(edg_maps == NULL)
      {
        edg_maps = (edg_schema_map_list_t)
          malloc(sizeof(struct edg_schema_map_list));
        memset(edg_maps, 0, sizeof(struct edg_schema_map_list));

        edg_maps->id1 = e->id1;
        edg_maps->id2 = e->id2;
        edg_maps->s_id = s_id;
      }
      else
      {
        cli_graph_export_mappings_insert_edg(edg_maps, e->id1, e->id2, s_id);
      }
      ++s_id;
    }
  }

  cli_graph_import_schema_print_list(s_list);

  cli_graph_export_mappings_print_vtx(vtx_maps);

  cli_graph_export_mappings_print_edg(edg_maps);

  for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
  {
    printf("S %d", sl->s_id);

    for(attribute_t a=sl->s->attrlist; a != NULL; a=a->next)
    {
      printf(" %s:%s", base_types_str[a->bt], a->name);
    }
    printf("\n");
  }

  for(vertex_t v=cg->v; v != NULL; v=v->next)
  {
    vtx_schema_map_list_t vm =
      cli_graph_export_mappings_vtx_lookup(vtx_maps, v->id);
    printf("V %llu %d\n", v->id, vm->s_id);

    // print tupless
  }

  for(edge_t e=cg->e; e != NULL; e=e->next)
  {
    edg_schema_map_list_t em =
      cli_graph_export_mappings_edg_lookup(edg_maps, e->id1, e->id2);
    printf("E %llu:%llu %d\n", e->id1, e->id2, em->s_id);

    // print tuples
  }
}