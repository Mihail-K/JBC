
# include <string.h>

# include "Debug.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo::MemberInfo() {
}

MemberInfo::~MemberInfo() {
	debug_printf(level1, "Deleting member : %s.\n", 
			(name == NULL || name->bytes == NULL ? "<NULL>" : (char *)name->bytes));

	if(!attributes.empty()) {
		debug_printf(level2, "Deleting member attributes.\n");
		for(std::vector<AttributeInfo *>::iterator itr = attributes.begin();
				itr != attributes.end(); itr++) {
			deleteAttribute(*itr);
		}
	}
}
