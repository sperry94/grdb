#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_import.h"

void cli_graph_import_schema_print_list(schema_list_t* s_list);
void cli_graph_import_schema_insert(schema_list_t* s_list, schema_t schema, int s_id);


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
      s_id++;
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
      s_id++;
    }
  }

  cli_graph_import_schema_print_list(s_list);


  for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
  {
    printf("S %d", sl->s_id);

    for(attribute_t a=sl->s->attrlist; a != NULL; a=a->next)
    {
      printf(" %s:%s", base_types_str[a->bt], a->name);
    }
    printf("\n");
  }
  //print schema
  //print vertices
  //print edges
}