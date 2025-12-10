#ifndef JD297_CC_TOOLCHAIN_H
#define JD297_CC_TOOLCHAIN_H

#include <jd297/vector.h>

typedef struct {
	vector_t *ld_args;
	vector_t *input_files;
	vector_t *lib_dirs;
	char *outfile;
} ToolchainLinkerArgs;

typedef void (*toolchain_linker_setup_args_func_t)(ToolchainLinkerArgs *args);

extern toolchain_linker_setup_args_func_t toolchain_linker_setup_args_func;

extern void toolchain_linker_openbsd_setup_args(ToolchainLinkerArgs *args);

extern void toolchain_linker_gnu_linux_setup_args(ToolchainLinkerArgs *args);

#endif
