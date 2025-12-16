#ifndef JD297_CC_TOOLCHAIN_H
#define JD297_CC_TOOLCHAIN_H

#include <jd297/vector.h>
#include <jd297/lmap.h>

typedef enum {
	Toolchain_OpenBSD,
	Toolchain_Linux
} ToolchainOS;

typedef struct {
	const char *key;
	ToolchainOS value;
} ToolchainFunctionLookupEntry;

extern lmap_t toolchain_lookup_functions;

extern int toolchain_create_lookups(void);

// TODO extern int toolchain_destroy_lookups(void);

typedef struct {
	vector_t *lib_dirs;
	char *outfile;
	// TODO ...
} ToolchainArgs;

typedef struct Toolchain Toolchain;

typedef void (*toolchain_linker_setup_args_func_t)(Toolchain *toolchain);

struct Toolchain {
	vector_t ld_args;
	vector_t input_files;
	
	ToolchainArgs *args;

	toolchain_linker_setup_args_func_t linker_setup_args_func;
};

extern int toolchain_create(Toolchain *toolchain, ToolchainArgs *args);

// TODO extern int toolchain_destroy(Toolchain *toolchain);

extern void toolchain_linker_openbsd_setup_args(Toolchain *toolchain);

extern void toolchain_linker_gnu_linux_setup_args(Toolchain *toolchain);

#endif
