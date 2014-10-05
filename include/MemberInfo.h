# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include "Types.h"
# include "ClassBuffer.h"
# include "ClassBuilder.h"

# define encodeField encodeMember
# define decodeField decodeMember
# define encodeMethod encodeMember
# define decodeMethod decodeMember

MemberInfo *createMember();

void deleteMember(MemberInfo *member);

int encodeMember(ClassFile *classFile, ClassBuilder *builder, MemberInfo *info);

MemberInfo *decodeMember(ClassFile *classFile, ClassBuffer *buffer);

# endif /* MemberInfo.h */
