#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

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

  

  //loop through graphs
    //add to running set of vertices and edges
    //if vertex/edge not in set, add
    //if vertex/edge in set, append tuple schema
  //loop through running sets
    //make a running set of schema
    //if schema exists already (full compare), don't add

  //print schema
  //print vertices
  //print edges
}