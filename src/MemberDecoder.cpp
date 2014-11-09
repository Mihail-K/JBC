
# include "List.h"
# include "Debug.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

MemberInfo *decodeMember(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx, length;
	MemberInfo *member = createMember();

	member->access_flags = bufferNextShort(buffer);

	// Member Name
	index = bufferNextShort(buffer);
	member->name = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level1, "Member Name : %s.\n", member->name->bytes);

	// Member Descriptor
	index = bufferNextShort(buffer);
	member->descriptor = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level1, "Member Descriptor : %s.\n", member->descriptor->bytes);

	// Member Attributes Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Member Attributes Count : %d.\n", length);
	member->attributes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		listAdd(member->attributes, decodeAttribute(classFile, buffer));
	}

	return member;
}
