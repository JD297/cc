#include "toolchain.h"

#include <jd297/lmap.h>
#include <jd297/vector.h>

#include <assert.h>
#include <err.h>
#include <sys/utsname.h>

lmap_t toolchain_lookup_functions;

static ToolchainFunctionLookupEntry toolchain_function_lookup_entries[] = {
	{
		.key = "OpenBSD",
		.value = Toolchain_OpenBSD
	},
	{
		.key = "Linux",
		.value = Toolchain_Linux
	},
};

int toolchain_create_lookups(void)
{
	int ret = 0;
	
	for (size_t i = 0; i < sizeof(toolchain_function_lookup_entries) / sizeof(ToolchainFunctionLookupEntry) && ret == 0; i++) {
		ret = lmap_add(&toolchain_lookup_functions, toolchain_function_lookup_entries[i].key, &toolchain_function_lookup_entries[i]);
	}

	return ret;
}

// TODO extern int toolchain_destroy_lookups(void);

int toolchain_create(Toolchain *toolchain, ToolchainArgs *args)
{
	toolchain->ld_args = (vector_t) { 0 };
	toolchain->input_files = (vector_t) { 0 };

	if (args->outfile == NULL) {
		args->outfile = "a.out";
	}
	
	toolchain->args = args;

	struct utsname name;
	
	if (uname(&name)) {
		warn("uname");
		return -1;
	}

	ToolchainFunctionLookupEntry *entry = lmap_get(&toolchain_lookup_functions, name.sysname);

	if (entry == NULL) {
		warnx("toolchain for operating system \"%s\" is not supported!", name.sysname);
		return -1;
	}
	
	switch (entry->value) {
		case Toolchain_OpenBSD: {
			toolchain->linker_setup_args_func = toolchain_linker_openbsd_setup_args;
		} break;
		case Toolchain_Linux: {
			toolchain->linker_setup_args_func = toolchain_linker_gnu_linux_setup_args;
		} break;
		default:
			assert(0 && "TODO: toolchain not implemented");
	}
	
	return 0;
}

// TODO extern int toolchain_destroy(Toolchain *toolchain);

//vec_free(&ld_args);
//vec_free(&input_files);
