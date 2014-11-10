
# include <stdio.h>
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassBuffer.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

ClassFile::ClassFile()
	: magic(0), major_version(0), minor_version(0),
		access_flags(0), this_class(NULL), super_class(NULL) {
}

ClassFile::ClassFile(ClassBuffer *buffer) {
	this->ReadBuffer(buffer);
}

ClassFile::~ClassFile() {
	// Clear interface list.
	if(!interfaces.empty()) {
		// These are disposed of later.
		interfaces.clear();
	}

	// Clear and release Fields.
	if(!fields.empty()) {
		for(std::vector<FieldInfo *>::iterator itr = fields.begin();
				itr != fields.end(); itr++)
			deleteMember(*itr);
		fields.clear();
	}

	// Clear and release Methods.
	if(!methods.empty()) {
		for(std::vector<MethodInfo *>::iterator itr = methods.begin();
				itr != methods.end(); itr++)
			deleteMember(*itr);
		methods.clear();
	}

	// Clear and release Attributes.
	if(!attributes.empty()) {
		for(std::vector<AttributeInfo *>::iterator itr = attributes.begin();
				itr != attributes.end(); itr++)
			deleteAttribute(*itr);
		attributes.clear();
	}

	// Finally, release the Constant Pool.
	if(!constant_pool.empty()) {
		for(std::vector<ConstantInfo *>::iterator itr = constant_pool.begin();
				itr != constant_pool.end(); itr++)
			deleteConstant(*itr);
		constant_pool.clear();
	}
}

ClassFile *createClassFile() {
	return NEW(ClassFile);
}

void deleteClassFile(ClassFile *classFile) {
	if(classFile == NULL) return;
	DELETE(classFile);
}
