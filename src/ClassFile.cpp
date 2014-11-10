
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
	if(!classFile->interfaces.empty()) {
		// These are disposed of later.
		classFile->interfaces.clear();
	}
	if(!classFile->fields.empty()) {
		for(unsigned idx = 0; idx < classFile->fields.size(); idx++)
			deleteMember(classFile->fields[idx]);
		classFile->fields.clear();
	}
	if(!classFile->methods.empty()) {
		for(unsigned idx = 0; idx < classFile->methods.size(); idx++)
			deleteMember(classFile->methods[idx]);
		classFile->methods.clear();
	}
	if(!classFile->attributes.empty()) {
		for(unsigned idx = 0; idx < classFile->attributes.size(); idx++)
			deleteAttribute(classFile->attributes[idx]);
		classFile->attributes.clear();
	}
	if(!classFile->constant_pool.empty()) {
		for(unsigned idx = 1; idx < classFile->constant_pool.size(); idx++)
			deleteConstant(classFile->constant_pool[idx]);
		classFile->constant_pool.clear();
	}
	DELETE(classFile);
}
