# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <stdio.h>
# include <stdlib.h>

# include "Types.h"

int encodeClassFile(FILE *target, ClassFile *classFile);

ClassFile *decodeClassFile(FILE *source);

static __inline__
void *getConstant(ClassFile *classFile, int idx) {
	if(idx < 0 || idx >= classFile->constant_pool_count) {
		fprintf(stderr, "Constant not found. (ID : %d)\n", idx);
		exit(EXIT_FAILURE);
	}

	return classFile->constant_pool[idx];
}

# endif /* ClassFile.h */
