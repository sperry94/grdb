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

typedef struct schema_list schema_list_t;

schema_list_t* s_list;

#endif