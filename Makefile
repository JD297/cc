.POSIX:

CC            = cc
CFLAGS        = -Wall -Wextra -Wpedantic -g -I src
LDFLAGS       = 

TARGET        = cc
PREFIX        = /usr/local
BINDIR        = $(PREFIX)/bin
MANDIR        = $(PREFIX)/share/man
SRCDIR        = src
BUILDDIR      = build

OBJFILES      = $(BUILDDIR)/lexer_c.o $(BUILDDIR)/lmap.o $(BUILDDIR)/parser_c.o \
                $(BUILDDIR)/parse_tree_node_c.o $(BUILDDIR)/preprocessor_c.o $(BUILDDIR)/token_type_c.o \
                $(BUILDDIR)/vector.o $(BUILDDIR)/logger.o $(BUILDDIR)/compiler_c.o \
                $(BUILDDIR)/list.o $(BUILDDIR)/optimizer.o $(BUILDDIR)/ir.o \
                $(BUILDDIR)/codegen_x86_64.o $(BUILDDIR)/codegen_aarch64.o \
                $(BUILDDIR)/codegen.o \
                $(BUILDDIR)/sv.o $(BUILDDIR)/lmap_sv.o $(BUILDDIR)/symtbl.o

HEADERS       = $(SRCDIR)/jd297/lmap.h $(SRCDIR)/jd297/vector.h \
                $(SRCDIR)/lexer_c.h $(SRCDIR)/parser_c.h \
                $(SRCDIR)/parse_tree_node_c.h  $(SRCDIR)/parse_tree_type_c.h \
                $(SRCDIR)/preprocessor_c.h $(SRCDIR)/token_c.h \
                $(SRCDIR)/token_type_c.h $(SRCDIR)/jd297/logger.h \
                $(SRCDIR)/compiler_c.h $(SRCDIR)/jd297/list.h \
                $(SRCDIR)/optimizer.h $(SRCDIR)/ir.h $(SRCDIR)/codegen.h \
                $(SRCDIR)/jd297/sv.h $(SRCDIR)/jd297/lmap_sv.h \
                $(SRCDIR)/symtbl.h

all: $(BUILDDIR)/$(TARGET) $(BUILDDIR)/cc_ir_test

lex: lexer_test.c $(BUILDDIR)/lexer_c.o $(BUILDDIR)/sv.o $(BUILDDIR)/lmap_sv.o $(BUILDDIR)/vector.o $(BUILDDIR)/token_type_c.o
	$(CC) $(CFLAGS) -o $@ lexer_test.c $(BUILDDIR)/lexer_c.o $(BUILDDIR)/sv.o $(BUILDDIR)/lmap_sv.o $(BUILDDIR)/vector.o $(BUILDDIR)/token_type_c.o
	./lex

$(BUILDDIR)/cc_ir_test: $(OBJFILES) $(BUILDDIR)/cc_ir_test.o
	$(CC) -o $@ $(OBJFILES) $(BUILDDIR)/cc_ir_test.o $(LDFLAGS)

$(BUILDDIR)/cc_ir_test.o: $(HEADERS) $(SRCDIR)/cc_ir_test.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/cc_ir_test.c

$(BUILDDIR)/$(TARGET): $(OBJFILES) $(BUILDDIR)/cc.o
	$(CC) -o $@ $(OBJFILES) $(BUILDDIR)/cc.o $(LDFLAGS)

$(BUILDDIR)/cc.o: $(HEADERS) $(SRCDIR)/cc.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/cc.c

$(BUILDDIR)/lexer_c.o: $(HEADERS) $(SRCDIR)/lexer_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/lexer_c.c

$(BUILDDIR)/lmap.o: $(HEADERS) $(SRCDIR)/lmap.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/lmap.c

$(BUILDDIR)/parser_c.o: $(HEADERS) $(SRCDIR)/parser_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/parser_c.c

$(BUILDDIR)/parse_tree_node_c.o: $(HEADERS) $(SRCDIR)/parse_tree_node_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/parse_tree_node_c.c

$(BUILDDIR)/preprocessor_c.o: $(HEADERS) $(SRCDIR)/preprocessor_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/preprocessor_c.c

$(BUILDDIR)/token_type_c.o: $(HEADERS) $(SRCDIR)/token_type_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/token_type_c.c

$(BUILDDIR)/vector.o: $(HEADERS) $(SRCDIR)/vector.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/vector.c

$(BUILDDIR)/logger.o: $(HEADERS) $(SRCDIR)/logger.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/logger.c

$(BUILDDIR)/compiler_c.o: $(HEADERS) $(SRCDIR)/compiler_c.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/compiler_c.c

$(BUILDDIR)/list.o: $(HEADERS) $(SRCDIR)/list.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/list.c

$(BUILDDIR)/optimizer.o: $(HEADERS) $(SRCDIR)/optimizer.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/optimizer.c

$(BUILDDIR)/ir.o: $(HEADERS) $(SRCDIR)/ir.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/ir.c

$(BUILDDIR)/codegen.o: $(HEADERS) $(SRCDIR)/codegen.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/codegen.c

$(BUILDDIR)/codegen_x86_64.o: $(HEADERS) $(SRCDIR)/codegen_x86_64.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/codegen_x86_64.c

$(BUILDDIR)/codegen_aarch64.o: $(HEADERS) $(SRCDIR)/codegen_aarch64.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/codegen_aarch64.c

$(BUILDDIR)/sv.o: $(HEADERS) $(SRCDIR)/sv.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/sv.c

$(BUILDDIR)/symtbl.o: $(HEADERS) $(SRCDIR)/symtbl.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/symtbl.c

$(BUILDDIR)/lmap_sv.o: $(HEADERS) $(SRCDIR)/lmap_sv.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/lmap_sv.c

clean:
	rm -f $(BUILDDIR)/*

install: $(BUILDDIR)/$(TARGET)
	cp $(BUILDDIR)/$(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
