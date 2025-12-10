#include "toolchain.h"

void toolchain_linker_gnu_linux_setup_args(ToolchainLinkerArgs *args)
{
	vec_push_back(args->ld_args, "ld");

	// TODO only shard linking is supported
	vec_push_back(args->ld_args, "-dynamic-linker=/lib64/ld-linux-x86-64.so.2");

	vec_push_back(args->ld_args, "-o");
	vec_push_back(args->ld_args, args->outfile);

	// TODO check for flags with nostdlib etc.
	vec_push_back(args->ld_args, "/usr/lib/x86_64-linux-gnu/crti.o");     // TODO HARD PATH to
	vec_push_back(args->ld_args, "/usr/lib/x86_64-linux-gnu/Scrt1.o");    // TODO HARD PATH to

	for (char **it = (char **)vec_begin(args->input_files); it < (char **)vec_end(args->input_files); it++) {
		vec_push_back(args->ld_args, *it);
	}

	for (char **it = (char **)vec_begin(args->lib_dirs); it < (char **)vec_end(args->lib_dirs); it++) {
		vec_push_back(args->ld_args, "-L");
		vec_push_back(args->ld_args, *it);
	}

	// TODO other libs ??
	vec_push_back(args->ld_args, "-lc");

	vec_push_back(args->ld_args, "/usr/lib/x86_64-linux-gnu/crtn.o"); // TODO HARD PATH to
	vec_push_back(args->ld_args, NULL);
}
