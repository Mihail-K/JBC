
# include "List.h"
# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"
# include "ClassBuffer.h"

# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

void decodeConstantPool(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;

	length = bufferNextShort(buffer);
	debug_printf(level1, "Constant Pool Count : %d.\n", length);

	classFile->constant_pool = createList();
	listAdd(classFile->constant_pool, NULL);

	for(idx = 1; idx < length; idx++) {
		ConstantInfo *info;
		debug_printf(level2, "Constant %d :\n", idx);
		info = decodeConstant(buffer);
		info->index = idx;

		listAdd(classFile->constant_pool, info);
		if(isLongConstant(info)) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			listAdd(classFile->constant_pool, NULL);
			idx++;
		}
	}
}

void decodeThisClass(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index = bufferNextShort(buffer);
	debug_printf(level3, "This Class : %d.\n", index);
	classFile->this_class = static_cast(ConstantClassInfo *, getConstant(classFile, index));
}

void decodeSuperClass(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index = bufferNextShort(buffer);
	debug_printf(level3, "Super Class : %d.\n", index);
	classFile->super_class = static_cast(ConstantClassInfo *, getConstant(classFile, index));
}

void decodeInterfaces(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;

	length = bufferNextShort(buffer);
	debug_printf(level1, "Interfaces Count : %d.\n", length);
	classFile->interfaces = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = bufferNextShort(buffer);
		debug_printf(level2, "Interface %d : %d.\n", idx, index);
		listAdd(classFile->interfaces, getConstant(classFile, index));
	}
}

ClassFile *decodeClassData(ClassBuffer *buffer) {
	unsigned int idx, length;
	ClassFile *classFile = createClassFile();

	classFile->magic = bufferNextInt(buffer);
	classFile->major_version = bufferNextShort(buffer);
	classFile->minor_version = bufferNextShort(buffer);

	debug_printf(level0, "Magic : %#X.\n", classFile->magic);
	debug_printf(level0, "Major Version : %d.\n", classFile->major_version);
	debug_printf(level0, "Minor Version : %d.\n", classFile->minor_version);

	decodeConstantPool(classFile, buffer);

	classFile->access_flags = bufferNextShort(buffer);
	debug_printf(level3, "Access Flags : %#X.\n", classFile->access_flags);

	decodeThisClass(classFile, buffer);
	decodeSuperClass(classFile, buffer);
	decodeInterfaces(classFile, buffer);

	// Fields Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Fields Count : %d.\n", length);
	classFile->fields = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		listAdd(classFile->fields, decodeField(classFile, buffer));
	}

	// Methods Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Methods Count : %d.\n", length);
	classFile->methods = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		listAdd(classFile->methods, decodeMethod(classFile, buffer));
	}

	// Attributes Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Attributes Count : %d.\n", length);
	classFile->attributes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		listAdd(classFile->attributes, decodeAttribute(classFile, buffer));
	}

	return classFile;
}

ClassFile *decodeClassFile(FILE *source) {
	debug_printf(level0, "Creating Class buffer.\n");
	ClassBuffer *buffer = createBuffer(source);
	debug_printf(level0, "Decoding Class file :\n");
	ClassFile *classFile = decodeClassData(buffer);
	debug_printf(level0, "Finished Class file.\n");
	return classFile;
}
