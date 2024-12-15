#ifndef LIBC_LOCAL_EXAMPLE_STRING_H
#define LIBC_LOCAL_EXAMPLE_STRING_H

extern size_t strlen(const char *s);

// This is a feature of the local lib
extern char *strstr(const char *haystack, const char *needle);

#endif

