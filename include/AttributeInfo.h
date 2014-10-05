# ifndef __ATTRIBUTEINFO_H__
# define __ATTRIBUTEINFO_H__

# include "Types.h"
# include "ClassBuffer.h"
# include "ClassBuilder.h"

# define createAttribute(TYPE)	\
	memset(zalloc(sizeof(TYPE)), 0, sizeof(TYPE))

void deleteAttribute(AttributeInfo *info);

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info);

AttributeInfo *decodeAttribute(ClassFile *classFile, ClassBuffer *buffer);

# endif /* AttributeInfo.h */
