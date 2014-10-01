# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include "Types.h"
# include "ClassBuffer.h"

# define visitField visitMember
# define visitMethod visitMember

MemberInfo *visitMember(ClassFile *classFile, ClassBuffer *buffer);

# endif /* MemberInfo.h */
