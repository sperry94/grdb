#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void
cli_export_collect_full_graph(graph_t cg)
{
  for(graph_t g = graphs; g != NULL; g=g->next)
  {
    // collect vertices in new graph
    for(vertex_t v=g->v; v != NULL; v=v->next)
    {
      // make a full vertex copy
      vertex_t nv = (vertex_t)malloc(sizeof(struct vertex));
      memset(nv, 0, sizeof(struct vertex));
      vertex_copy(v, nv);
      // remove links for copy
      nv->prev = NULL;
      nv->next = NULL;

      // determine if vertex id is already in new graph
      vertex_t gv = graph_find_vertex_by_id(cg, nv->id);
      if(gv == NULL)
      {
        // if not, insert vertex
        graph_insert_vertex(cg, nv);
      }
      else
      {
        // if it is, append schema
        // TODO: tuple data should be copied here
        schema_attribute_insert(gv->tuple->s, nv->tuple->s->attrlist);
      }
    }

    //collect edges in new graph
    for(edge_t e=g->e; e != NULL; e=e->next)
    {
      // make a copy of the edge
      edge_t ne = (edge_t)malloc(sizeof(struct edge));
      memset(ne, 0, sizeof(struct edge));
      edge_copy(e, ne);
      // remove links for copy
      ne->prev = NULL;
      ne->next = NULL;

      // determine if edge is already in new graph
      edge_t ge = graph_find_edge_by_ids(cg, ne->id1, ne->id2);
      if(ge == NULL)
      {
        // if not, insert edge
        graph_insert_edge(cg, ne);
      }
      else
      {
        // if it is, append schema
        // TODO: tuple data should be copied here
        schema_attribute_insert(ge->tuple->s, ne->tuple->s->attrlist);
      }
    }

  }
}