
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "ClassFile.h"

int main(int argc, char **argv) {
	FILE *file;

	if(argc < 2) {
		fprintf(stderr, "Requires argument path.\n");
		exit(EXIT_FAILURE);
	}

	if((file = fopen(argv[1], "rb")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	deleteClassFile(decodeClassFile(file));
	fclose(file);

	return 0;
}
