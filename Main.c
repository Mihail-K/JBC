
# include <stdio.h>
# include <string.h>

# include "ClassFile.h"

int main(int argc, char **argv) {
	FILE *file;
	char *path = argv[1];
	if((file = fopen(path, "rb")) == NULL) {
		perror("fopen");
		return 1;
	}
	visitClassFile(createBuffer(file));
	return 0;
}
