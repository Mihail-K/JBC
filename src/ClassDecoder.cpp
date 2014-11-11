
# include "Debug.h"
# include "ClassFile.h"
# include "ClassBuffer.h"

# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

void ClassFile::DecodeConstants(ClassBuffer *buffer) {
	uint16_t length;

	length = buffer->NextShort();
	debug_printf(level1, "Constant Pool Count : %d.\n", length);

	// 0 is a NULL index.
	constant_pool.push_back(NULL);
	for(unsigned idx = 1; idx < length; idx++) {
		ConstantInfo *info;
		debug_printf(level2, "Constant %d :\n", idx);
		info = decodeConstant(buffer);
		info->index = idx;

		constant_pool.push_back(info);

		if(isLongConstant(info)) {
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
		interfaces.push_back((ConstantClassInfo *)constant_pool[index]);
	}
}

void ClassFile::DecodeFields(ClassBuffer *buffer) {
	uint16_t length;

	// Fields Table
	length = buffer->NextShort();
	debug_printf(level1, "Fields Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		fields.push_back((new MemberInfo)->DecodeMember(buffer, this));
	}
}

void ClassFile::DecodeMethods(ClassBuffer *buffer) {
	uint16_t length;

	// Methods Table
	length = buffer->NextShort();
	debug_printf(level1, "Methods Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		methods.push_back((new MemberInfo)->DecodeMember(buffer, this));
	}
}

void ClassFile::DecodeAttributes(ClassBuffer *buffer) {
	uint16_t length;

	// Attributes Table
	length = buffer->NextShort();
	debug_printf(level1, "Attributes Count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		attributes.push_back(decodeAttribute(this, buffer));
	}
}

void ClassFile::DecodeClassFile(ClassBuffer *buffer) {
	magic = buffer->NextInt();
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

ClassFile *decodeClassFile(FILE *source) {
	debug_printf(level0, "Creating Class buffer.\n");
	ClassBuffer *buffer = createBuffer(source);
	debug_printf(level0, "Decoding Class file :\n");
	ClassFile *classFile = new ClassFile(buffer);
	debug_printf(level0, "Finished Class file.\n");
	debug_printf(level3, "Reads made : %u.\n", buffer->GetReads());
	delete buffer;
	return classFile;
}
