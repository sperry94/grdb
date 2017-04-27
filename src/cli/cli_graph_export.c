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

void cli_graph_export_print_tuples(tuple_t t, FILE* out)
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
cli_graph_export(char *cmdline, int *pos)
{
  char fn[BUFSIZE];

	memset(fn, 0, BUFSIZE);
	nextarg(cmdline, pos, DEF_SEP, fn);

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

      vertex_t gv = graph_find_vertex_by_id(cg, nv->id);
      if(gv == NULL)
      {
        graph_insert_vertex(cg, nv);
      }
      else
      {
        //copy tuple data
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

      edge_t ge = graph_find_edge_by_ids(cg, ne->id1, ne->id2);
      if(ge == NULL)
      {
        graph_insert_edge(cg, ne);
      }
      else
      {
        //copy tuple data
        schema_attribute_insert(ge->tuple->s, ne->tuple->s->attrlist);
      }
    }
  }

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

  FILE* fd;

  if((fd=fopen(fn, "w+")) == NULL)
  {
    perror("Error opening file");
		return;
  }

  for(schema_list_t* sl=s_list; sl != NULL && sl->s != NULL; sl=sl->next)
  {
    fprintf(fd, "S %d", sl->s_id);

    for(attribute_t a=sl->s->attrlist; a != NULL; a=a->next)
    {
      fprintf(fd, " %s:%s", base_types_str[a->bt], a->name);
    }
    fprintf(fd, "\n");
  }

  for(vertex_t v=cg->v; v != NULL; v=v->next)
  {
    vtx_schema_map_list_t vm =
      cli_graph_export_mappings_vtx_lookup(vtx_maps, v->id);
    fprintf(fd, "V %llu %d", v->id, vm->s_id);

    cli_graph_export_print_tuples(v->tuple, fd);
  }

  for(edge_t e=cg->e; e != NULL; e=e->next)
  {
    char el[BUFSIZE];
    memset(el, 0, BUFSIZE);

    edg_schema_map_list_t em =
      cli_graph_export_mappings_edg_lookup(edg_maps, e->id1, e->id2);
    fprintf(fd, "E %llu:%llu %d", e->id1, e->id2, em->s_id);

    cli_graph_export_print_tuples(e->tuple, fd);
  }

  fclose(fd);
}
