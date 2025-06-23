.POSIX:

PREFIX        = ../../src

CC            = cc
CFLAGS        = -Wall -Wextra -Wpedantic -g -I $(PREFIX)
LDFLAGS       = 

TARGET        = token_type_skipable_lookup_generator
SRCDIR        = src
BUILDDIR      = build

OBJFILES      = $(BUILDDIR)/token_type_skipable_lookup_generator.o

HEADERS       = $(PREFIX)/token_type_c.h

$(BUILDDIR)/$(TARGET): $(OBJFILES)
	$(CC) -o $@ $(OBJFILES) $(LDFLAGS)

$(BUILDDIR)/token_type_skipable_lookup_generator.o: $(HEADERS) $(SRCDIR)/token_type_skipable_lookup_generator.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/token_type_skipable_lookup_generator.c

clean:
	rm -f $(BUILDDIR)/*

run: $(BUILDDIR)/$(TARGET)
	./build/token_type_skipable_lookup_generator > $(PREFIX)/token_type_skipable_lookup.h
