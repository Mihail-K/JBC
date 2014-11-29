
# include <stdio.h>
# include <string.h>

# include "Debug.h"
# include "ClassBuffer.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

namespace JBC {

ClassFile::ClassFile()
	: magic(0), major_version(0), minor_version(0),
		access_flags(0), this_class(NULL), super_class(NULL) {
}

ClassFile::ClassFile(ClassBuffer *buffer, uint32_t magic)
	: magic(magic), major_version(0), minor_version(0),
		access_flags(0), this_class(NULL), super_class(NULL) {
	this->DecodeClassFile(buffer);
}

ClassFile::~ClassFile() {
	// Clear interface list.
	if(!interfaces.empty()) {
		// These are disposed of later.
		interfaces.clear();
	}

	// Clear and release Fields.
	if(!fields.empty()) {
		for(std::vector<MemberInfo *>::iterator itr = fields.begin();
				itr != fields.end(); itr++) {
			delete *itr;
		}
	}

	// Clear and release Methods.
	if(!methods.empty()) {
		for(std::vector<MemberInfo *>::iterator itr = methods.begin();
				itr != methods.end(); itr++) {
			delete *itr;
		}
	}

	// Clear and release Attributes.
	if(!attributes.empty()) {
		for(std::vector<AttributeInfo *>::iterator itr = attributes.begin();
				itr != attributes.end(); itr++) {
			delete *itr;
		}
	}

	// Finally, release the Constant Pool.
	if(!constant_pool.empty()) {
		for(std::vector<ConstantInfo *>::iterator itr = constant_pool.begin();
				itr != constant_pool.end(); itr++) {
			delete *itr;
		}
	}
}

ConstantInfo *&ClassFile::AddConstant(ConstantInfo *info) {
	uint16_t index = constant_pool.size();
	constant_pool.push_back(info);
	if(info) info->index = index;

	return constant_pool.back();
}

AttributeInfo *&ClassFile::AddAttribute(AttributeInfo *info, bool validate) {
	if(validate) {
		// TODO
	}

	attributes.push_back(info);
	return attributes.back();
}

} /* JBC */
