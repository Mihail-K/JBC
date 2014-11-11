
# include "Debug.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

namespace JBC {

MemberInfo *MemberInfo::DecodeMember(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index, length;

	// Access Flags
	access_flags = buffer->NextShort();

	// Member Name
	index = buffer->NextShort();
	name = dynamic_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level1, "Member Name : %s.\n", name->bytes);

	// Member Descriptor
	index = buffer->NextShort();
	descriptor = dynamic_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level1, "Member Descriptor : %s.\n", descriptor->bytes);

	// Member Attributes Table
	length = buffer->NextShort();
	debug_printf(level1, "Member Attributes Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		attributes.push_back(decodeAttribute(classFile, buffer));
	}

	return this;
}

} /* JBC */
