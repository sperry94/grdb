#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void cli_nf_schema_insert(s_list_t s_list, schema_t schema, int s_id);

void
cli_import_schema(char* fl, int* pos, s_list_t s_list)
{
	//collect id
	char id[BUFSIZE];
	memset(id, 0 , BUFSIZE);
	nextarg(fl, pos, DEF_SEP, id);

	schema_t schema = (schema_t)malloc(sizeof(struct schema));
	memset(schema, 0, sizeof(struct schema));

	//collect field type
	char t[BUFSIZE];
	memset(t, 0, BUFSIZE);
	nextarg(fl, pos, ITEM_SEP, t);
	//collect field name
	char n[ATTR_NAME_MAXLEN];
	memset(n, 0, ATTR_NAME_MAXLEN);
	nextarg(fl, pos, ITEM_SEP, n);

	while(strlen(t) > 0 && strlen(n) > 0)
	{
		for(int te=0; te < BASE_TYPES_MAX; te++)
		{
			if(strcmp(t, base_types_str[te]) == 0)
			{
					attribute_t attr = (attribute_t)
						malloc(sizeof(struct attribute));

					schema_attribute_init(attr, n, te, NULL);
					schema_attribute_insert(schema, attr);
					break;
			}
		}

		//collect field type
		memset(t, 0, BUFSIZE);
		nextarg(fl, pos, ITEM_SEP, t);
		//collect field name
		memset(n, 0, ATTR_NAME_MAXLEN);
		nextarg(fl, pos, ITEM_SEP, n);
	}

	if(s_list == NULL)
	{
		s_list = (s_list_t)malloc(sizeof(struct s_list));
		memset(s_list, 0, sizeof(struct s_list));

		s_list->s_id = strtol(id, NULL, 10);
		s_list->s = schema;
		return;
	}

	cli_nf_schema_insert(s_list, schema, strtol(id, NULL, 10));
}
