#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

void
cli_nf_schema_print_list(s_list_t s_list)
{
	s_list_t s;
	for(s = s_list; s != NULL; s = s->next)
	{
		printf("Schema %d: ", s->s_id);
		schema_print(s->s);
		printf("\n");
	}
}

schema_t
cli_nf_schema_find_by_id(s_list_t s_list, int s_id)
{
	s_list_t s;
	for(s = s_list; s != NULL; s = s->next)
	{
			if(s->s_id == s_id)
				return s->s;
	}
	return NULL;
}

void
cli_nf_schema_insert(s_list_t s_list, schema_t schema, int s_id)
{
	if(s_list->s == NULL)
	{
		s_list->s_id = s_id;
		s_list->s = schema;
		return;
	}

	s_list_t end;
	for(end = s_list; end->next != NULL; end = end->next);

	end->next = (s_list_t)malloc(sizeof(struct s_list));
	memset(end->next, 0, sizeof(struct s_list));

	end->next->s_id = s_id;
	end->next->s = schema;
}