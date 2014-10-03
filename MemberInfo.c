
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo *createMember() {
	MemberInfo *info = zalloc(sizeof(MemberInfo));
	return memset(info, 0, sizeof(MemberInfo));
}

void deleteMember(MemberInfo *member) {
	if(member == NULL) return;
	if(member->attributes != NULL) {
		debug_printf(level2, "Deleting member attributes.\n");
		deleteList(member->attributes, deleteAttribute);
	}
	free(member);
}

MemberInfo *visitMember(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx, length;
	MemberInfo *member = createMember();

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
	length = bufferNextShort(buffer);
	debug_printf(level1, "Member Attributes Count : %d.\n", length);
	member->attributes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		listAdd(member->attributes, visitAttribute(classFile, buffer));
	}

	return member;
}
