 char *program_invocation_name;

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <jd297/lmap.h>
#include <jd297/vector.h>

#include "preprocessor_c.h"

void print_usage(void)
{
	fprintf(stderr, "usage: %s [-c][-C][-e epsym] [-D name[=value]]... [-E][-f][-F][-g]...\n", program_invocation_name);
	fprintf(stderr, "       %*s [-I directory]... [-L directory]... [-o outfile][-O][-p][-P]\n", (int)strlen(program_invocation_name), " ");
	fprintf(stderr, "       %*s [-q][-r][-s][-S][-u symname]... [-U name]... [-W options]... operand...\n", (int)strlen(program_invocation_name), " ");
}

int cflag;
int Cflag;
char *epsym;
lmap_t defines;
int Eflag;
int fflag;
int Fflag;
int gflag;
vector_t include_dirs;
vector_t lib_dirs;
char *outfile;
char *optlevel;
int pflag;
int Pflag;
int qflag;
int rflag;
int sflag;
int Sflag;
lmap_t undefsyms;
lmap_t undefines;
vector_t phaseoptions;
vector_t operands;

void free_globals(void)
{
    vec_free(&include_dirs);
    vec_free(&lib_dirs);
    vec_free(&phaseoptions);
    lmap_free(&defines);
    lmap_free(&undefsyms);
    lmap_free(&undefines);
}

int main(int argc, char **argv)
{
    program_invocation_name = argv[0];

	atexit(free_globals);

	int opt;

	while ((opt = getopt(argc, argv, "cCe:D:EfFgI:L:o:OpPqrsSu:U:W:")) != -1) {
		switch (opt) {
			case 'c': cflag = 1; break;
			case 'C': Cflag = 1; break;
			case 'e': epsym = optarg; break;
			case 'D': {
			    char *value = optarg;

			    while (*value != '\0') {
			        if (*value == '=') {
			            *value = '\0';
			            ++value;
			            break;
			        }

			        ++value;
			    }

			    lmap_add(&defines, optarg, (*value == '\0') ? "1" : value);
			} break;
			case 'E': Eflag = 1; break;
			case 'f': fflag = 1; break;
			case 'F': Fflag = 1; break;
			case 'g': gflag = 1; break;
			case 'I': vec_push_back(&include_dirs, optarg); break;
			case 'L': vec_push_back(&lib_dirs, optarg); break;
			case 'o': outfile = optarg; break;
			case 'O': optlevel = optarg; break;
			case 'p': pflag = 1; break;
			case 'P': Pflag = 1; break;
			case 'q': qflag = 1; break;
			case 'r': rflag = 1; break;
			case 's': sflag = 1; break;
			case 'S': Sflag = 1; break;
			case 'u': lmap_add(&undefsyms, optarg, NULL); break;
			case 'U': lmap_add(&undefines, optarg, NULL); break;
			case 'W': vec_push_back(&phaseoptions, optarg); break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}

	// usual places:
	vec_push_back(&include_dirs, "/usr/local/include");
	vec_push_back(&include_dirs, "/usr/include");

	vec_push_back(&lib_dirs, "/usr/local/lib");
	vec_push_back(&lib_dirs, "/usr/lib");
	vec_push_back(&lib_dirs, "/lib");

	if (Eflag == 0 && Sflag == 0 && cflag == 0) {
		cflag = 1;
	}

	if (optind == argc) {
		fprintf(stderr, "%s: error: no input file\n", program_invocation_name);
		exit(EXIT_FAILURE);
	}

	if (cflag == 0 && Sflag == 0 && Eflag == 0) {
		fprintf(stderr, "%s: error: linking is not supported\n", program_invocation_name);
		exit(EXIT_FAILURE);
	}

	if (Sflag == 1) {
		fprintf(stderr, "%s: error: assambling is not supported\n", program_invocation_name);
		exit(EXIT_FAILURE);
	}

	if (cflag == 1) {
		fprintf(stderr, "%s: error: compiling is not supported\n", program_invocation_name);
		exit(EXIT_FAILURE);
	}

	if (optind + 1 != argc && outfile != NULL && (Eflag == 1 || Sflag == 1 || cflag == 1)) {
		fprintf(stderr, "%s: error: cannot specify -o when generating multiple output files\n", program_invocation_name);
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; i++) {
		struct stat sb;

		if (stat(argv[i], &sb) == -1) {
			fprintf(stderr, "%s: error: %s: input file unused: %s\n", program_invocation_name, argv[i], strerror(errno));
			continue;
		}

		if ((sb.st_mode & S_IFMT) == S_IFDIR) {
			fprintf(stderr, "%s: error: %s: input file unused: %s\n", program_invocation_name, argv[i], strerror(EISDIR));
			continue;
		}

		vec_push_back(&operands, argv[i]);
	}

    // precompile regex patterns for lexer
    if (token_type_c_regex_create() == -1) {
        return EXIT_FAILURE;
    }

    atexit(token_type_c_regex_destroy);

    // PREPROCESSOR START
    FILE *output = Eflag == 1 ? stdout : tmpfile();

    Preprocessor_C preprocessor = {
        .include_dirs = &include_dirs,
        .source_files = &operands,
        .defines = &defines,
        .output = output
    };

    if (preprocessor_c_run(&preprocessor) == -1) {
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
