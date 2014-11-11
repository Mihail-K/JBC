# ifndef __MEMBERINFO_H__
# define __MEMBERINFO_H__

# include <vector>
# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

namespace JBC {

// Forward Declarations.
class ClassFile;
struct AttributeInfo;
struct ConstantUtf8Info;

/* Field/Method Info */

class MemberInfo {
public:
	uint16_t	access_flags;

	// Name
	ConstantUtf8Info *name;

	// Descriptor
	ConstantUtf8Info *descriptor;

	// Attributes Table
	std::vector<AttributeInfo *> attributes;

public:
	MemberInfo();
	~MemberInfo();

public:
	MemberInfo *DecodeMember(ClassBuffer *buffer, ClassFile *classFile);
	MemberInfo *EncodeMember(ClassBuilder *builder, ClassFile *classFile);
};

} /* JBC */

# endif /* MemberInfo.h */
