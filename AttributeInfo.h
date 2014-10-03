# ifndef __ATTRIBUTEINFO_H__
# define __ATTRIBUTEINFO_H__

# include "Types.h"
# include "ClassBuffer.h"

void deleteAttribute(AttributeInfo *info);

AttributeInfo *visitAttribute(ClassFile *classFile, ClassBuffer *buffer);

# endif /* AttributeInfo.h */
