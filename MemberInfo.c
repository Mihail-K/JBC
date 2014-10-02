
# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo *visitMember(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx;
	MemberInfo *member = zalloc(sizeof(MemberInfo));

	member->access_flags = bufferNextShort(buffer);

	// Member Name
	member->name_index = bufferNextShort(buffer);
	member->name = getConstant(classFile, member->name_index);
	debug_printf("Member Name : %s.\n", member->name->bytes);

	// Member Descriptor
	member->descriptor_index = bufferNextShort(buffer);
	member->descriptor = getConstant(classFile, member->descriptor_index);
	debug_printf("Member Descriptor : %s.\n", member->descriptor->bytes);

	// Member Attributes Table
	member->attributes_count = bufferNextShort(buffer);
	debug_printf("Member Attributes Count : %d.\n", member->attributes_count);
	member->attributes = zalloc(sizeof(AttributeInfo *) * member->attributes_count);

	for(idx = 0; idx < member->attributes_count; idx++) {
		member->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return member;
}
