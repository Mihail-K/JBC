
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo *createMember() {
	MemberInfo *info = NEW(MemberInfo);
	memset(info, 0, sizeof(MemberInfo));
	return info;
}

void deleteMember(MemberInfo *member) {
	if(member == NULL) return;
	debug_printf(level1, "Deleting member : %s.\n",
			member->name->bytes);
	if(member->attributes != NULL) {
		debug_printf(level2, "Deleting member attributes.\n");
		deleteList(member->attributes, (void(*)(void *))deleteAttribute);
	}
	DELETE(member);
}