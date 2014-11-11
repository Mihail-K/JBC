
# include "Debug.h"
# include "ClassFile.h"

# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

void ClassFile::EncodeConstants(ClassBuilder *builder) {
	uint16_t length;

	builder->NextShort(length = constant_pool.size());
	debug_printf(level1, "Constant Pool Count : %zu.\n", length);

	// 0 is a NULL constant.
	for(unsigned idx = 1; idx < length; idx++) {
		ConstantInfo *info = constant_pool[idx];
		debug_printf(level2, "Constant %d :\n", idx);

		encodeConstant(builder, info);
		if(isLongConstant(info)) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			idx++;
		}
	}
}

void ClassFile::EncodeClasses(ClassBuilder *builder) {
	// This class
	if(this_class != NULL) {
		uint16_t index =  this_class->index;
		debug_printf(level3, "This Class : %d.\n", index);
		builder->NextShort(index);
	} else {
		// No ThisClass Entry
		debug_printf(level2, "This Class : <NULL>.\n");
		builder->NextShort(0);
	}

	// Super class
	if(super_class != NULL) {
		uint16_t index =  super_class->index;
		debug_printf(level3, "Super Class : %d.\n", index);
		builder->NextShort(index);
	} else {
		// No SuperClass Entry
		debug_printf(level2, "Super Class : <NULL>.\n");
		builder->NextShort(0);
	}
}

void ClassFile::EncodeInterfaces(ClassBuilder *builder) {
	uint16_t length;

	length = interfaces.size();
	debug_printf(level1, "Interfaces Count : %d.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Interface %d : %d.\n", idx, interfaces[idx]->index);
		builder->NextShort(interfaces[idx]->index);
	}
}

void ClassFile::EncodeFields(ClassBuilder *builder) {
	uint16_t length;

	// Fields Table
	length = fields.size();
	debug_printf(level1, "Fields Count : %d.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		fields[idx]->EncodeMember(builder, this);
	}
}

void ClassFile::EncodeMethods(ClassBuilder *builder) {
	uint16_t length;

	// Methods Table
	length = methods.size();
	debug_printf(level1, "Methods Count : %d.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		methods[idx]->EncodeMember(builder, this);
	}
}

void ClassFile::EncodeAttributes(ClassBuilder *builder) {
	uint16_t length;

	// Attributes Table
	length = attributes.size();
	debug_printf(level1, "Attributes Count : %d.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		encodeAttribute(this, builder, attributes[idx]);
	}
}

void ClassFile::EncodeClassFile(ClassBuilder *builder) {
	debug_printf(level0, "Magic : %#X.\n", magic);
	debug_printf(level0, "Major Version : %d.\n", major_version);
	debug_printf(level0, "Minor Version : %d.\n", minor_version);

	builder->NextInt(magic);
	builder->NextShort(major_version);
	builder->NextShort(minor_version);

	EncodeConstants(builder);

	debug_printf(level3, "Access Flags : %#X.\n", access_flags);
	builder->NextShort(access_flags);

	EncodeClasses(builder);
	EncodeInterfaces(builder);

	EncodeFields(builder);
	EncodeMethods(builder);
	EncodeAttributes(builder);
}

int encodeClassFile(FILE *source, ClassFile *classFile) {
	debug_printf(level0, "Creating Class builder.\n");
	ClassBuilder *builder = new ClassBuilder(source);
	debug_printf(level0, "Encoding Class file :\n");
	classFile->EncodeClassFile(builder);
	debug_printf(level0, "Finished Class file.\n");
	debug_printf(level3, "Writes made : %u.\n", builder->GetWrites());
	delete builder;
	return 0;
}
