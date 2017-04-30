#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema.h"

char *base_types_str[] = {
	"CHAR",
	"VARCHAR",
	"BOOL",
	"ENUM",
	"INT",
	"FLOAT",
	"DOUBLE",
	"DATE",
	"TIME"
};

int base_types_len[] = {
	1,			/* Single char */
	256,			/* Max string length */
	1,			/* Unsigned char 0=FALSE 1=TRUE */
	2,			/* Two unsigned chars 1) enum index */
				/*   2) item index */
	8,			/* For 64-bit */
	sizeof(float),		/* Should be an IEEE standard length */
	sizeof(double),		/* Should be an IEEE standard length */
	10,			/* "MM-DD-YYYY" string no terminator */
	8			/* "HH:MM:SE" string no terminator */
};

void
schema_attribute_init(
	attribute_t attr,
	void *name,
	enum base_types bt,
	enum_t e)
{
	assert (attr != NULL);
	assert (name != NULL);

	memset(attr, 0, sizeof(struct attribute));
	strncpy(attr->name, name, ATTR_NAME_MAXLEN - 1);
	attr->bt = bt;
	attr->e = e;
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr)
{
	assert (attr != NULL);

	printf("%s:%s",
	       (attr->bt == ENUM ?
		attr->e->name :
		base_types_str[attr->bt]),
	       attr->name);
}

void
schema_attribute_insert(schema_t s, attribute_t attr)
{
	attribute_t t;

	assert (s != NULL);
	assert (attr != NULL);

	if (s->attrlist == NULL) {
		s->attrlist = attr;
		return;
	}
	for (t = s->attrlist; t->next != NULL; t = t->next);
	t->next = attr;
}

void
schema_attribute_remove(schema_t s, attribute_t attr)
{
	attribute_t prev, curr;

	assert (s != NULL);
	assert (attr != NULL);

	for (prev = NULL, curr = s->attrlist;
	     curr != NULL;
	     prev = curr, curr = curr->next)
		if (curr == attr) {
			if (prev == NULL) {
				s->attrlist = curr->next;
			} else {
				prev->next = curr->next;
			}
		}
}

void
schema_init(schema_t *s)
{
	assert (s != NULL);

	if (*s != NULL)
		free(s);

	*s = malloc(sizeof(struct schema));
	memset(*s, 0, sizeof(struct schema));
}

void schema_copy(schema_t in, schema_t out)
{
	if(out->attrlist == NULL)
		out->attrlist = (attribute_t)malloc(sizeof(struct attribute));

	attribute_t in_attr, out_attr;
	for(in_attr=in->attrlist, out_attr=out->attrlist; in_attr != NULL;
		in_attr=in_attr->next, out_attr=out_attr->next)
	{
		schema_attribute_init(out_attr, in_attr->name, in_attr->bt, in_attr->e);
		if(in_attr->next != NULL)
			out_attr->next = (attribute_t)malloc(sizeof(struct attribute));
		else
			break;
	}
}

int
schema_size(schema_t s)
{
	attribute_t attr;
	int acc = 0;

	assert(s != NULL);

	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		acc += base_types_len[attr->bt];
	return acc;
}

short
schema_compare(schema_t s1, schema_t s2)
{
	for(attribute_t a1=s1->attrlist; a1 != NULL; a1=a1->next)
	{
		short found = 0;
		for(attribute_t a2=s2->attrlist; a2 != NULL; a2=a2->next)
		{
			if(strcmp(a1->name, a2->name) == 0 && a1->bt == a2->bt)
			{
				found = 1;
			}
		}
		if (!found)
			return 0;
	}

	for(attribute_t a2=s2->attrlist; a2 != NULL; a2=a2->next)
	{
		short found = 0;
		for(attribute_t a1=s1->attrlist; a1 != NULL; a1=a1->next)
		{
			if(strcmp(a1->name, a2->name) == 0 && a1->bt == a2->bt)
			{
				found = 1;
			}
		}
		if (!found)
			return 0;
	}

	return 1;
}

base_types_t
schema_find_type_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find schema [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr->bt;

	return BASE_TYPES_MAX;
}

attribute_t
schema_find_attr_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find attribute [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr;

	return NULL;
}

void
schema_print(schema_t s)
{
	attribute_t attr;

	assert (s != NULL);

	printf("[");
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		schema_attribute_print(attr);
		if (attr->next != NULL)
			printf(",");
	}
	printf("]");

	/* XXX Print enum list */
}
