
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "ClassFile.h"

using namespace JBC;

int main(int argc, char **argv) {
	FILE *input, *output;
	ClassFile *classFile;

	if(argc < 3) {
		fprintf(stderr, "Requires arguments [input file] [output file].\n");
		exit(EXIT_FAILURE);
	}

	if((input = fopen(argv[1], "rb")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	if((output = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	classFile = decodeClassFile(input);
	encodeClassFile(output, classFile);
	delete classFile;

	printf("Done!\n");

	return 0;
}
