# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <stdio.h>
# include <stdlib.h>

# include "Types.h"

ClassFile *createClassFile();

void deleteClassFile(ClassFile *classFile);

int encodeClassFile(FILE *target, ClassFile *classFile);

ClassFile *decodeClassFile(FILE *source);

static inline
void *getConstant(ClassFile *classFile, unsigned int idx) {
	if(idx > (unsigned int)classFile->constant_pool.size()) {
		fprintf(stderr, "Constant not found. (ID : %d/%zu)\n", idx,
				classFile->constant_pool.size());
		exit(EXIT_FAILURE);
	}

	return (void *)classFile->constant_pool[idx];
}

# endif /* ClassFile.h */
