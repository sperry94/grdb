#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_graph_import_schema_print_list(schema_list_t* s_list);
void cli_graph_import_schema_insert(schema_list_t* s_list,
  schema_t schema, int s_id);

void cli_export_map_print_vtx(v_s_map_t vtx_maps);
v_s_map_t cli_export_map_find_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id);
void cli_export_mappings_insert_vtx(v_s_map_t vtx_maps,
  vertexid_t v_id, int s_id);

void cli_export_map_print_edg(e_s_map_t edg_maps);
e_s_map_t cli_export_map_find_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2);
void cli_export_mappings_insert_edg(e_s_map_t edg_maps,
  vertexid_t e_id1, vertexid_t e_id2, int s_id);

void
cli_export_print_tuples(tuple_t t, FILE* out)
{
  // code sourced from tuple_print function in tuple_print.c
  for(attribute_t attr = t->s->attrlist; attr != NULL; attr = attr->next)
  {
    int offset = tuple_get_offset(t, attr->name);
    short val;
    int i;
    float fval;
    double dval;
    char out_val[BUFSIZE];
    memset(out_val, 0, BUFSIZE);
    if (offset >= 0) {
      switch (attr->bt) {
      case CHARACTER:
        sprintf(out_val, "%c",
          tuple_get_char(t->buf + offset));
        break;

      case VARCHAR:
        sprintf(out_val, "\"%s\"",
          (char *) (t->buf + offset));
        break;

      case BOOLEAN:
        val = tuple_get_bool(t->buf + offset);
        if (val == 0)
          sprintf(out_val, "FALSE");
        else
          sprintf(out_val, "TRUE");
        break;

      case ENUM:
        break;

      case INTEGER:
        i = tuple_get_int(t->buf + offset);
        sprintf(out_val, "%d", i);
        break;

      case FLOAT:
        fval = tuple_get_float(t->buf + offset);
        sprintf(out_val, "%4.2f", fval);
        break;

      case DOUBLE:
        dval = tuple_get_double(t->buf + offset);
        sprintf(out_val, "%4.2f", dval);
        break;

      case DATE:
        {
          char s[base_types_len[DATE] + 1];

          memset(s, 0,
            base_types_len[DATE] + 1);
          tuple_get_date(t->buf + offset, s);
          sprintf(out_val, "%s", s);
        }
        break;

      case TIME:
        {
          char s[base_types_len[TIME] + 1];

          memset(s, 0,
            base_types_len[TIME] + 1);
          tuple_get_time(t->buf + offset, s);
          sprintf(out_val, "%s", s);
        }
        break;

      case BASE_TYPES_MAX:
        break;
      }
    }
    if(strlen(out_val) > 0)
      fprintf(out, " %s:%s", attr->name, out_val);
  }
  fprintf(out, "\n");
}

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

void
cli_export_vertices(graph_t g, v_s_map_t vtx_maps, FILE* out)
{
  for(vertex_t v=g->v; v != NULL; v=v->next)
  {
    v_s_map_t vm =
      cli_export_map_find_vtx(vtx_maps, v->id);
    fprintf(out, "V %llu %d", v->id, vm->s_id);

    cli_export_print_tuples(v->tuple, out);
  }
}

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
      cli_graph_import_schema_insert(s_list, v->tuple->s, *s_id);
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
        cli_export_mappings_insert_vtx(vtx_maps, v->id, *s_id);
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
      cli_graph_import_schema_insert(s_list, e->tuple->s, *s_id);
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
        cli_export_mappings_insert_edg(edg_maps, e->id1, e->id2, *s_id);
      }
      ++(*s_id);
    }
  }
  return edg_maps;
}

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

void
cli_export(char *cmdline, int *pos)
{
  graph_t cg = (graph_t)malloc(sizeof(struct graph));
  graph_init(cg);

  cli_export_collect_full_graph(cg);

  int s_id = 0;
  schema_list_t* s_list = (schema_list_t*)malloc(sizeof(struct schema_list));
  memset(s_list, 0, sizeof(struct schema_list));

  v_s_map_t vtx_maps = cli_export_collect_vtx_schemas(cg, s_list, &s_id);
  e_s_map_t edg_maps = cli_export_collect_edg_schemas(cg, s_list, &s_id);

  char fn[BUFSIZE];
	memset(fn, 0, BUFSIZE);
	nextarg(cmdline, pos, DEF_SEP, fn);

  FILE* fd;

  if((fd=fopen(fn, "w+")) == NULL)
  {
    perror("Error opening file");
		return;
  }

  cli_export_schemas(s_list, fd);

  cli_export_vertices(cg, vtx_maps, fd);

  cli_export_edges(cg, edg_maps, fd);

  fclose(fd);
}
