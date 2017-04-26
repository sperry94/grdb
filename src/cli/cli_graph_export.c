#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_export(char *cmdline, int *pos)
{
  vertex_t vertices = (vertex_t)malloc(sizeof(struct vertex));
  vertex_t rv = vertices;
  edge_t edges = (edge_t)malloc(sizeof(struct edge));
  edge_t re = edges;

  graph_t g;
  for(g = graphs; g != NULL; g = g->next)
  {

  }

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