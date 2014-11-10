
# include <stdio.h>
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassBuffer.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

ClassFile *createClassFile() {
	return NEW(ClassFile);
}

void deleteClassFile(ClassFile *classFile) {
	if(classFile == NULL) return;

	// Clear interface list.
	if(!classFile->interfaces.empty()) {
		// These are disposed of later.
		classFile->interfaces.clear();
	}

	// Clear and release Fields.
	if(!classFile->fields.empty()) {
		for(std::vector<FieldInfo *>::iterator itr = classFile->fields.begin();
				itr != classFile->fields.end(); itr++)
			deleteMember(*itr);
		classFile->fields.clear();
	}

	// Clear and release Methods.
	if(!classFile->methods.empty()) {
		for(std::vector<MethodInfo *>::iterator itr = classFile->methods.begin();
				itr != classFile->methods.end(); itr++)
			deleteMember(*itr);
		classFile->methods.clear();
	}

	// Clear and release Attributes.
	if(!classFile->attributes.empty()) {
		for(std::vector<AttributeInfo *>::iterator itr = classFile->attributes.begin();
				itr != classFile->attributes.end(); itr++)
			deleteAttribute(*itr);
		classFile->attributes.clear();
	}

	// Finally, release the Constant Pool.
	if(!classFile->constant_pool.empty()) {
		for(std::vector<ConstantInfo *>::iterator itr = classFile->constant_pool.begin();
				itr != classFile->constant_pool.end(); itr++)
			deleteConstant(*itr);
		classFile->constant_pool.clear();
	}
	DELETE(classFile);
}
