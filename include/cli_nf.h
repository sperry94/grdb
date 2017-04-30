#ifndef __CLI_GRAPHT_IMPORT_H
#define __CLI_GRAPHT_IMPORT_H

#include "graph.h"

#define DEF_SEP " \n"
#define ITEM_SEP ":; \n"

/* Schema list for import */
struct s_list {
	int s_id;
	schema_t s;
	struct s_list* next;
};

struct v_s_map {
	vertexid_t v_id;
	int s_id;
	struct v_s_map* next;
};

struct e_s_map {
	vertexid_t e_id1, e_id2;
	int s_id;
	struct e_s_map* next;
};

typedef struct s_list* s_list_t;
typedef struct v_s_map* v_s_map_t;
typedef struct e_s_map* e_s_map_t;

s_list_t s_list;

#endif