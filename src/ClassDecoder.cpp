
# include "Debug.h"
# include "ClassFile.h"
# include "ErrorTypes.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

namespace JBC {

void ClassFile::DecodeConstants(ClassBuffer *buffer) {
	uint16_t length;

	length = buffer->NextShort();
	debug_printf(level1, "Constant Pool Count : %d.\n", length);

	// 0 is a NULL index.
	constant_pool.push_back(NULL);
	for(unsigned idx = 1; idx < length; idx++) {
		ConstantInfo *info;
		debug_printf(level2, "Constant %d :\n", idx);
		info = AddConstant(DecodeConstant(buffer));

		// "Long" constants take up two indexes.
		if(info->IsLongConstant()) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			constant_pool.push_back(NULL);
			idx++;
		}
	}
}

void ClassFile::DecodeClasses(ClassBuffer *buffer) {
	uint16_t index;

	// This class
	index = buffer->NextShort();
	debug_printf(level3, "This Class : %d.\n", index);
	this_class = static_cast<ConstantClassInfo *>(constant_pool[index]);

	// Super class
	index = buffer->NextShort();
	debug_printf(level3, "Super Class : %d.\n", index);
	super_class = static_cast<ConstantClassInfo *>(constant_pool[index]);
}

void ClassFile::DecodeInterfaces(ClassBuffer *buffer) {
	uint16_t length;

	length = buffer->NextShort();
	debug_printf(level1, "Interfaces Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		debug_printf(level2, "Interface %d : %d.\n", idx, index);
		AddInterface(static_cast<ConstantClassInfo *>(constant_pool[index]));
	}
}

void ClassFile::DecodeFields(ClassBuffer *buffer) {
	uint16_t length;

	// Fields Table
	length = buffer->NextShort();
	debug_printf(level1, "Fields Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		AddField((new MemberInfo)->DecodeMember(buffer, this));
	}
}

void ClassFile::DecodeMethods(ClassBuffer *buffer) {
	uint16_t length;

	// Methods Table
	length = buffer->NextShort();
	debug_printf(level1, "Methods Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		AddMethod((new MemberInfo)->DecodeMember(buffer, this));
	}
}

void ClassFile::DecodeAttributes(ClassBuffer *buffer) {
	uint16_t length;

	// Attributes Table
	length = buffer->NextShort();
	debug_printf(level1, "Attributes Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		AddAttribute(DecodeAttribute(buffer, this), false);
	}
}

void ClassFile::DecodeClassFile(ClassBuffer *buffer) {
	uint32_t magic = buffer->NextInt();
	if(this->magic && this->magic != magic) {
		char tmp[64];
		sprintf(tmp, "Magic number mismatch; expected %#010X, got %#010X.",
				this->magic, magic);
		throw DecodeError(tmp);
	}

	major_version = buffer->NextShort();
	minor_version = buffer->NextShort();

	debug_printf(level0, "Magic : %#X.\n", magic);
	debug_printf(level0, "Major Version : %d.\n", major_version);
	debug_printf(level0, "Minor Version : %d.\n", minor_version);

	DecodeConstants(buffer);

	access_flags = buffer->NextShort();
	debug_printf(level3, "Access Flags : %#X.\n", access_flags);

	DecodeClasses(buffer);
	DecodeInterfaces(buffer);

	DecodeFields(buffer);
	DecodeMethods(buffer);
	DecodeAttributes(buffer);
}

ClassFile *DecodeClassFile(FILE *source, uint32_t magic) {
	ClassBuffer *buffer;

	try {
		ClassFile *classFile;

		debug_printf(level0, "Creating Class buffer.\n");
		buffer = new ClassBuffer(source);

		debug_printf(level0, "Decoding Class file :\n");
		classFile = new ClassFile(buffer, magic);

		debug_printf(level0, "Finished Class file.\n");
		debug_printf(level3, "Reads made : %u.\n", buffer->GetReads());

		delete buffer;
		return classFile;
	} catch(DecodeError &ex) {
		// Rethrow after closing input.
		delete buffer;
		throw;
	}
}

} /* JBC */
