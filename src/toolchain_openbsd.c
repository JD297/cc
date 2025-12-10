#include "toolchain.h"

void toolchain_linker_openbsd_setup_args(ToolchainLinkerArgs *args)
{
	vec_push_back(args->ld_args, "ld");

	// TODO only shard linking is supported
	vec_push_back(args->ld_args, "-dynamic-linker=/usr/libexec/ld.so");

	vec_push_back(args->ld_args, "-o");
	vec_push_back(args->ld_args, args->outfile);

	// TODO check for flags with nostdlib etc.
	vec_push_back(args->ld_args, "/usr/lib/crt0.o");     // TODO HARD PATH to
	vec_push_back(args->ld_args, "/usr/lib/crtbegin.o"); // TODO HARD PATH to

	for (char **it = (char **)vec_begin(args->input_files); it < (char **)vec_end(args->input_files); it++) {
		vec_push_back(args->ld_args, *it);
	}

	for (char **it = (char **)vec_begin(args->lib_dirs); it < (char **)vec_end(args->lib_dirs); it++) {
		vec_push_back(args->ld_args, "-L");
		vec_push_back(args->ld_args, *it);
	}

	// TODO other libs ??
	vec_push_back(args->ld_args, "-lc");

	vec_push_back(args->ld_args, "/usr/lib/crtend.o"); // TODO HARD PATH to
	vec_push_back(args->ld_args, NULL);
}
