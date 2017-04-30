#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

schema_t cli_graph_import_schema_find_by_id(int s_id);

void
cli_graph_import_tuples(char* fl, int* pos, int s_id, tuple_t tuple)
{
	schema_t s = cli_graph_import_schema_find_by_id(s_id);

	tuple_init(tuple, s);

	/* Tuple value import not supported
	// collect field name
	char n[ATTR_NAME_MAXLEN];
	memset(n, 0, ATTR_NAME_MAXLEN);
	nextarg(fl, pos, ITEM_SEP, n);

	// collect field value
	char v[BUFSIZE];
	memset(v, 0, BUFSIZE);
	nextarg(fl, pos, ITEM_SEP, v);

	while(strlen(n) > 0 && strlen(v) > 0)
	{
		base_types_t bt = schema_find_type_by_name(s, n);

		char fv[BUFSIZE];
		memset(fv, 0, BUFSIZE);
		if(bt == VARCHAR)
		{
			// handle string
		}
		else
		{
			strncpy(fv, v, BUFSIZE - 1);
		}

		tuple_set(tuple, n, v);

		// collect field name
		memset(n, 0, ATTR_NAME_MAXLEN);
		nextarg(fl, pos, ITEM_SEP, n);

		// collect field value
		memset(v, 0, BUFSIZE);
		nextarg(fl, pos, ITEM_SEP, v);
	}
	*/
}
