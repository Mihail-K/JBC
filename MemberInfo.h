# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include "Types.h"
# include "ClassBuffer.h"

MemberInfo *visitField(ClassFile *classFile, ClassBuffer *buffer);

MemberInfo *visitMethod(ClassFile *classFile, ClassBuffer *buffer);

# endif /* MemberInfo.h */
