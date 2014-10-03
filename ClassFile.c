
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
	ClassFile *classFile = zalloc(sizeof(ClassFile));
	return memset(classFile, 0, sizeof(ClassFile));
}

void deleteClassFile(ClassFile *classFile) {
	if(classFile == NULL) return;
	if(classFile->interfaces != NULL) {
		deleteList(classFile->interfaces, NULL);
	}
	if(classFile->fields != NULL) {
		deleteList(classFile->fields, deleteMember);
	}
	if(classFile->methods != NULL) {
		deleteList(classFile->methods, deleteMember);
	}
	if(classFile->attributes != NULL) {
		deleteList(classFile->attributes, deleteAttribute);
	}
	if(classFile->constant_pool != NULL) {
		deleteList(classFile->constant_pool, deleteConstant);
	}
	free(classFile);
}

void visitConstantPool(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;

	length = bufferNextShort(buffer);
	debug_printf(level1, "Constant Pool Count : %d.\n", length);

	classFile->constant_pool = createList();
	listAdd(classFile->constant_pool, NULL);

	for(idx = 1; idx < length; idx++) {
		ConstantInfo *info;
		debug_printf(level2, "Constant %d :\n", idx);
		info = visitConstant(buffer);
		info->index = idx;

		listAdd(classFile->constant_pool, info);
		if(isLongConstant(info)) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			listAdd(classFile->constant_pool, NULL);
			idx++;
		}
	}
}

void visitThisClass(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index = bufferNextShort(buffer);
	debug_printf(level3, "This Class : %d.\n", index);
	classFile->this_class = getConstant(classFile, index);
}

void visitSuperClass(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index = bufferNextShort(buffer);
	debug_printf(level3, "Super Class : %d.\n", index);
	classFile->super_class = getConstant(classFile, index);
}

void visitInterfaces(ClassFile *classFile, ClassBuffer *buffer) {
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

ClassFile *visitClassFile(ClassBuffer *buffer) {
	unsigned int idx, length;
	ClassFile *classFile = createClassFile();

	classFile->magic = bufferNextInt(buffer);
	classFile->major_version = bufferNextShort(buffer);
	classFile->minor_version = bufferNextShort(buffer);

	debug_printf(level0, "Magic : %#X.\n", classFile->magic);
	debug_printf(level0, "Major Version : %d.\n", classFile->major_version);
	debug_printf(level0, "Minor Version : %d.\n", classFile->minor_version);

	visitConstantPool(classFile, buffer);

	classFile->access_flags = bufferNextShort(buffer);
	debug_printf(level3, "Access Flags : %#X.\n", classFile->access_flags);

	visitThisClass(classFile, buffer);
	visitSuperClass(classFile, buffer);
	visitInterfaces(classFile, buffer);

	// Fields Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Fields Count : %d.\n", length);
	classFile->fields = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		listAdd(classFile->fields, visitField(classFile, buffer));
	}

	// Methods Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Methods Count : %d.\n", length);
	classFile->methods = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		listAdd(classFile->methods, visitMethod(classFile, buffer));
	}

	// Attributes Table
	length = bufferNextShort(buffer);
	debug_printf(level1, "Attributes Count : %d.\n", length);
	classFile->attributes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		listAdd(classFile->attributes, visitAttribute(classFile, buffer));
	}

	return classFile;
}

ClassFile *decodeClassFile(FILE *source) {
	debug_printf(level0, "Creating Class buffer.\n");
	ClassBuffer *buffer = createBuffer(source);
	debug_printf(level0, "Visiting Class file :\n");
	ClassFile *classFile = visitClassFile(buffer);
	debug_printf(level0, "Finished Class file.\n");
	deleteBuffer(buffer);
	return classFile;
}

