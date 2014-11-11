
# include "Debug.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

namespace JBC {

MemberInfo *MemberInfo::EncodeMember(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	// Access Flags
	builder->NextShort(access_flags);

	// Member Name
	builder->NextShort(name->index);
	debug_printf(level1, "Member Name : %s.\n", name->bytes);

	// Member Descriptor
	builder->NextShort(descriptor->index);
	debug_printf(level1, "Member Descriptor : %s.\n", descriptor->bytes);

	builder->NextShort((uint16_t)(length = attributes.size()));
	debug_printf(level1, "Member Attributes Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		encodeAttribute(classFile, builder, attributes[idx]);
	}

	return this;
}

} /* JBC */
