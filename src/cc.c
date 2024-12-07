#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void print_usage()
{
	printf("Usage: %s [-E] [-S] [-c] [-o outfile] file...\n", TARGET);
	printf("c compiler\n");
	printf("\n");
	printf("JD297 %s source code <https://github.com/jd297/cc>\n", TARGET);
}

int main(int argc, char **argv)
{
	int Eflag = 0;
	int Sflag = 0;
	int cflag = 0;

	const char *outfile = NULL;

	int opt;

	while ((opt = getopt(argc, argv, "ESco:")) != -1) {
		switch (opt) {
			case 'E':
				Eflag = 1;
			break;
			case 'S':
				Sflag = 1;
			break;
			case 'c':
				cflag = 1;
			break;
			case 'o':
				outfile = optarg;
			break;
			case '?':
				print_usage();
				exit(EXIT_FAILURE);
			break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}

	if (Eflag == 0 && Sflag == 0 && cflag == 0) {
		cflag = 1;
	}

	if (optind == argc) {
		fprintf(stderr, "%s: error: no input file\n", TARGET);
		exit(EXIT_FAILURE);
	}

	if (cflag == 0 && Sflag == 0 && Eflag == 0) {
		fprintf(stderr, "%s: error: linking is not supported\n", TARGET);
		exit(EXIT_FAILURE);
	}

	if (cflag == 1) {
		fprintf(stderr, "%s: error: assambling is not supported\n", TARGET);
		exit(EXIT_FAILURE);
	}

	if (Sflag == 1) {
		fprintf(stderr, "%s: error: compiling is not supported\n", TARGET);
		exit(EXIT_FAILURE);
	}

	if (optind + 1 != argc && outfile != NULL && (Eflag == 1 || Sflag == 1 || cflag == 1)) {
		fprintf(stderr, "%s: error: cannot specify ‘-o’ with ‘-E’, ‘-S’ and ‘-c’ with multiple files\n", TARGET);
		exit(EXIT_FAILURE);
	}

	for (int i = optind; i < argc; i++) {
		struct stat sb;

		if (stat(argv[i], &sb) == -1) {
			fprintf(stderr, "%s: error: %s: input file unused: %s\n", TARGET, argv[i], strerror(errno));
			continue;
		}

		if ((sb.st_mode & S_IFMT) == S_IFDIR) {
			fprintf(stderr, "%s: error: %s: input file unused: %s\n", TARGET, argv[i], strerror(EISDIR));
			continue;
		}

		printf("[FILE]: %s\n", argv[i]);
	}

	return 0;
}
