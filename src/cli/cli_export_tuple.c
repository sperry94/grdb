#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "cli_nf.h"

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