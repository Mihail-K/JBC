
# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo *visitMember(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx;
	MemberInfo *member = zalloc(sizeof(MemberInfo));

	member->access_flags = bufferNextShort(buffer);

	// Member Name
	index = bufferNextShort(buffer);
	member->name = getConstant(classFile, index);
	debug_printf(level1, "Member Name : %s.\n", member->name->bytes);

	// Member Descriptor
	index = bufferNextShort(buffer);
	member->descriptor = getConstant(classFile, index);
	debug_printf(level1, "Member Descriptor : %s.\n", member->descriptor->bytes);

	// Member Attributes Table
	member->attributes_count = bufferNextShort(buffer);
	debug_printf(level1, "Member Attributes Count : %d.\n", member->attributes_count);
	member->attributes = zalloc(sizeof(AttributeInfo *) * member->attributes_count);

	for(idx = 0; idx < member->attributes_count; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		member->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return member;
}
