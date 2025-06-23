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

OBJFILES      = $(BUILDDIR)/cc.o $(BUILDDIR)/lexer_c.o $(BUILDDIR)/lmap.o $(BUILDDIR)/parser_c.o \
                $(BUILDDIR)/parse_tree_node_c.o $(BUILDDIR)/preprocessor_c.o $(BUILDDIR)/token_type_c.o \
                $(BUILDDIR)/vector.o

HEADERS       = $(SRCDIR)/jd297/lmap.h $(SRCDIR)/jd297/vector.h \
                $(SRCDIR)/lexer_c.h $(SRCDIR)/parser_c.h $(SRCDIR)/parse_tree_node_c.h \
                $(SRCDIR)/parse_tree_type_c.h $(SRCDIR)/preprocessor_c.h $(SRCDIR)/token_c.h $(SRCDIR)/token_type_c.h

$(BUILDDIR)/$(TARGET): $(OBJFILES)
	$(CC) -o $@ $(OBJFILES) $(LDFLAGS)

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

$(BUILDDIR)/token_type_c.o: $(HEADERS) $(SRCDIR)/token_type_c.c $(SRCDIR)/token_type_skipable_lookup.h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/token_type_c.c

$(BUILDDIR)/vector.o: $(HEADERS) $(SRCDIR)/vector.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/vector.c

$(SRCDIR)/token_type_skipable_lookup.h: $(SRCDIR)/token_type_c.h
	sh -c -- "cd tools/token_type_skipable_lookup_generator && make run"

clean:
	rm -f $(BUILDDIR)/* $(SRCDIR)/token_type_skipable_lookup.h

install: $(BUILDDIR)/$(TARGET)
	cp $(BUILDDIR)/$(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
