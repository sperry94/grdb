#ifndef __CLI_GRAPHT_IMPORT_H
#define __CLI_GRAPHT_IMPORT_H

#include "graph.h"

#define DEF_SEP " \n"
#define ITEM_SEP ":; \n"

/* Schema list for import */
struct schema_list {
	int s_id;
	schema_t s;
	struct schema_list* next;
};

struct vtx_schema_map_list {
	vertexid_t id;
	int s_id;
	struct vtx_schema_map_list* next;
};

struct edg_schema_map_list {
	vertexid_t id1, id2;
	int s_id;
	struct edg_schema_map_list* next;
};

typedef struct schema_list schema_list_t;
typedef struct vtx_schema_map_list vtx_schema_map_list_t;
typedef struct edg_schema_map_list edg_schema_map_list_t;

schema_list_t* s_list;

#endif