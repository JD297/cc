#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#include <jd297/lmap.h>
#include <jd297/vector.h>

#include "preprocessor_c.h"
#include "compiler_c.h"

#define CC_TEMPLATE_PATH "%s/%s-XXXXXXXXXX%s"

const char *tmp_path = "/tmp";

char *cc_gentemplate(const char *tmp_path, const char *path, const char *suffix)
{
	char *file_name = strdup(path);

	if (file_name == NULL) {
		return NULL;
	}

	char *file_suffix = strrchr(file_name, '.');
	
	if (file_suffix != NULL) {
		*file_suffix = '\0';
	}
	
	char *file_basename = strrchr(file_name, '/');
	
	if (file_basename != NULL) {
		++file_basename;
	}

	char *template = malloc(sizeof(char) * PATH_MAX);

	if (template == NULL) {
		return NULL;
	}

	snprintf(template, PATH_MAX, CC_TEMPLATE_PATH, tmp_path, file_basename, suffix);

	free(file_name);

	return template;
}

FILE *cc_tmpfile(char*template)
{
	int fd;
	FILE *fp = NULL;
	char *suffix = strrchr(template, '.');

	int suffixlen = 0;
	
	if (suffix != NULL) {
		suffixlen = (int)strlen(suffix);
	}

	if ((fd = mkstemps(template, suffixlen)) == -1 || (fp = fdopen(fd, "w+")) == NULL) {
		if (fd != -1) {
			unlink(template);
			close(fd);
		}
	}
	
	return fp;
}

pid_t cc_fork_execvp(vector_t *args)
{
	pid_t pid;

	if ((pid = fork()) == -1) {
		err(EXIT_FAILURE, NULL);
	}

	if (pid == 0) {
		if (execvp(*vec_begin(args), (char **)vec_begin(args)) == -1) {
			err(EXIT_FAILURE, "%s", (char *)*vec_begin(args));
		}
	}
	
	return pid;
}

void print_usage(void)
{
	fprintf(stderr, "usage: cc [-c][-C][-e epsym] [-D name[=value]]... [-E][-f][-F][-g]...\n");
	fprintf(stderr, "          [-i directory]... [-L directory]... [-o outfile][-O][-p][-P]\n");
	fprintf(stderr, "          [-q][-r][-s][-S][-u symname]... [-U name]... [-W options]... operand...\n");
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
			case 'i': vec_push_back(&include_dirs, optarg); break;
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

	if (optind == argc) {
		errx(EXIT_FAILURE, "error: no input file");
	}

	if (optind + 1 != argc && outfile != NULL && (Eflag == 1 || Sflag == 1 || cflag == 1)) {
		errx(EXIT_FAILURE, "error: cannot specify -o when generating multiple output files");
	}

	// usual places:
	vec_push_back(&include_dirs, "/usr/local/include");
	vec_push_back(&include_dirs, "/usr/include");

	vec_push_back(&lib_dirs, "/usr/local/lib");
	vec_push_back(&lib_dirs, "/usr/lib");
	vec_push_back(&lib_dirs, "/lib");

	// precompile regex patterns for lexer
    if (token_type_c_regex_create() == -1) {
        return EXIT_FAILURE;
    }

    atexit(token_type_c_regex_destroy);

	if (cflag == 0 && Sflag == 0 && Eflag == 0) {
		if (outfile == NULL) {
			outfile = "a.out";
		}

		vector_t ld_args = { 0 };

		vec_push_back(&ld_args, "ld");

		// TODO HARD works on OBSD but not on linux
		// TODO only shard linking is supported
		vec_push_back(&ld_args, "--dynamic-linker=/usr/libexec/ld.so");

		vec_push_back(&ld_args, "-o");
		vec_push_back(&ld_args, outfile);

		// TODO HARD works on OBSD but not on linux
		// TODO maybe differences between clang and gcc (compiler lib??)
		// TODO check for flags with nostdlib etc.
		vec_push_back(&ld_args, "/usr/lib/crt0.o");
		vec_push_back(&ld_args, "/usr/lib/crtbegin.o");

		for (int i = optind; i < argc; i++) {
			char *input_file = argv[i];

			struct stat sb;

			if (stat(input_file, &sb) == -1) {
				errx(EXIT_FAILURE, "error: %s: '%s'", strerror(errno), input_file);
			}

			if ((sb.st_mode & S_IFMT) == S_IFDIR) {
				errx(EXIT_FAILURE, "error: %s: '%s'", strerror(EISDIR), input_file);
			}

			char *extension = strrchr(input_file, '.');

			if (extension != NULL && strcmp(".c", extension) == 0) {
				char *preprocessor_template = cc_gentemplate(tmp_path, input_file, ".i");

				FILE *preprocessor_output = cc_tmpfile(preprocessor_template);

				if (preprocessor_output == NULL) {
					err(EXIT_FAILURE, NULL);
				}

				vector_t source_files = { 0 };
				vec_push_back(&source_files, input_file);

			    Preprocessor_C preprocessor = {
			        .include_dirs = &include_dirs,
			        .source_files = &source_files,
			        .defines = &defines,
			        .output = preprocessor_output
			    };

			    if (preprocessor_c_run(&preprocessor) == -1) {
					warnx("preprocessor crashed");
			        return EXIT_FAILURE;
			    }

				char *compiler_template = cc_gentemplate(tmp_path, input_file, ".s");

				FILE *compiler_output = cc_tmpfile(compiler_template);

				if (compiler_output == NULL) {
					err(EXIT_FAILURE, NULL);
				}

				Compiler_C compiler = {
			    	.input = preprocessor_output,
					.output = compiler_output
				};

				if (compiler_c_run(&compiler) == -1) {
					warnx("compiler crashed");
					return EXIT_FAILURE;
				}

				unlink(preprocessor_template);
				fclose(preprocessor_output);
				fclose(compiler_output);
			    vec_free(&source_files);

				char *assemler_template = cc_gentemplate(tmp_path, input_file, ".o");

				fclose(cc_tmpfile(assemler_template));
				
				vector_t as_args = { 0 };
				vec_push_back(&as_args, "as");
				vec_push_back(&as_args, compiler_template);
				vec_push_back(&as_args, "-o");
				vec_push_back(&as_args, assemler_template);
				vec_push_back(&as_args, NULL);

				if (waitpid(cc_fork_execvp(&as_args), NULL, 0) == -1) {
					warn(NULL);
				}

				vec_free(&as_args);

				vec_push_back(&ld_args, assemler_template);

				continue;
			}

			vec_push_back(&ld_args, input_file);
		}

		for (char **it = (char **)vec_begin(&lib_dirs); it < (char **)vec_end(&lib_dirs); it++) {
			vec_push_back(&ld_args, "-L");
			vec_push_back(&ld_args, *it);
		}

		vec_push_back(&ld_args, "-l");
		vec_push_back(&ld_args, "c");

		vec_push_back(&ld_args, "/usr/lib/crtend.o");
		vec_push_back(&ld_args, NULL);

		int wstatus;

		if (waitpid(cc_fork_execvp(&ld_args), &wstatus, 0) == -1) {
			err(EXIT_FAILURE, NULL);
		}

		vec_free(&ld_args);

		exit(WEXITSTATUS(wstatus));
	}

	if (Sflag == 1) {
		errx(EXIT_FAILURE, "error: assambling is not supported");
	}

	if (cflag == 1) {
		errx(EXIT_FAILURE, "error: compiling is not supported");
	}

	for (int i = optind; i < argc; i++) {
		struct stat sb;

		if (stat(argv[i], &sb) == -1) {
			warnx("warning: %s: input file unused: %s", argv[i], strerror(errno));
			continue;
		}

		if ((sb.st_mode & S_IFMT) == S_IFDIR) {
			warnx("warning: %s: input file unused: %s", argv[i], strerror(EISDIR));
			continue;
		}

		vec_push_back(&operands, argv[i]);
	}

    // PREPROCESSOR START
    FILE *preprocessor_output = Eflag == 1 ? stdout : tmpfile();

    Preprocessor_C preprocessor = {
        .include_dirs = &include_dirs,
        .source_files = &operands,
        .defines = &defines,
        .output = preprocessor_output
    };

    if (preprocessor_c_run(&preprocessor) == -1) {
        return EXIT_FAILURE;
    }

    Compiler_C compiler = {
    	.input = preprocessor_output,
		.output = stdout
	};

	if (compiler_c_run(&compiler) == -1) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
