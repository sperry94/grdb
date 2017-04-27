include ../Makefile.rules

LIBDIRS = -L../db

OBJS =	bufdump.o 			\
	cli.o				\
	cli_enum.o			\
	cli_export_mappings.o		\
	cli_graph.o			\
	cli_graph_edge.o		\
	cli_graph_new.o			\
	cli_graph_export.o		\
	cli_graph_import.o		\
	cli_graph_import_edge.o		\
	cli_graph_import_schema.o		\
	cli_graph_import_tuples.o		\
	cli_graph_import_vertex.o		\
	cli_graph_schema.o		\
	cli_graph_schema_add.o		\
	cli_graph_tuple.o		\
	cli_graph_update_tuples.o	\
	lex.o				\
	main.o

LIBS = -ldb
TGT = db

all: $(OBJS)
	cd ../db; make;
	$(CC) $(LIBDIRS) $(GDB) -o $(TGT) $(OBJS) $(LIBS)

clean:
	cd ../db; make clean;
	rm -f $(TGT) $(OBJS)

wc: clean
	wc -l ../../include/*.h ../db/*.c *.c
