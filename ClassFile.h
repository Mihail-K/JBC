# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <stdio.h>

# include "Types.h"

int encodeClassFile(FILE *target, ClassFile *classFile);

ClassFile *decodeClassFile(FILE *source);

# endif /* ClassFile.h */
