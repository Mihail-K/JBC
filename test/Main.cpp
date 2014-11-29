
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <vector>
# include <iostream>

# include "ClassFile.h"
# include "MemberInfo.h"

using namespace JBC;

int main(int argc, char **argv) {
	FILE *input, *output;
	ClassFile *clazz;

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

	try {
		clazz = DecodeClassFile(input);

		std::cout << "Listing Fields:" << std::endl;
		for(std::vector<MemberInfo *>::iterator itr = clazz->fields.begin();
				itr != clazz->fields.end(); itr++) {
			std::cout << "Field: " << (*itr)->Name() << "." << std::endl;
		}

		std::cout << "Listing Methods:" << std::endl;
		for(std::vector<MemberInfo *>::iterator itr = clazz->methods.begin();
				itr != clazz->methods.end(); itr++) {
			std::cout << "Method: " << (*itr)->Name() << "." << std::endl;
		}

		EncodeClassFile(output, clazz);
		delete clazz;
	} catch(JBCError &err) {
		std::cout << err.msg;
	}

	std::cout << "Done!\n";
	return 0;
}
