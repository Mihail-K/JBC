# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include "Types.h"
# include "ClassBuffer.h"

# define decodeField decodeMember
# define decodeMethod decodeMember

MemberInfo *createMember();

void deleteMember(MemberInfo *member);

MemberInfo *decodeMember(ClassFile *classFile, ClassBuffer *buffer);

# endif /* MemberInfo.h */
