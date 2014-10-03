
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
	unsigned int idx;
	if(classFile == NULL) return;
	if(classFile->interfaces != NULL) {
		free(classFile->interfaces);
	}
	if(classFile->fields != NULL) {
		for(idx = 0; idx < classFile->fields_count; idx++)
			deleteMember(classFile->fields[idx]);
		free(classFile->fields);
	}
	if(classFile->methods != NULL) {
		for(idx = 0; idx < classFile->methods_count; idx++)
			deleteMember(classFile->methods[idx]);
		free(classFile->methods);
	}
	if(classFile->attributes != NULL) {
		for(idx = 0; idx < classFile->attributes_count; idx++)
			deleteAttribute(classFile->attributes[idx]);
		free(classFile->attributes);
	}
	if(classFile->constant_pool != NULL) {
		for(idx = 0; idx < classFile->constant_pool_count; idx++)
			deleteConstant(classFile->constant_pool[idx]);
		free(classFile->constant_pool);
	}
	free(classFile);
}

void visitConstantPool(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx;

	classFile->constant_pool_count = bufferNextShort(buffer);
	debug_printf(level1, "Constant Pool Count : %d.\n", classFile->constant_pool_count);

	classFile->constant_pool = zalloc(sizeof(ConstantInfo *) *
			classFile->constant_pool_count);
	classFile->constant_pool[0] = NULL;
	for(idx = 1; idx < classFile->constant_pool_count; idx++) {
		debug_printf(level2, "Constant %d :\n", idx);
		classFile->constant_pool[idx] = visitConstant(buffer);
		classFile->constant_pool[idx]->index = idx;

		if(isLongConstant(classFile->constant_pool[idx])) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			classFile->constant_pool[++idx] = NULL;
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
	unsigned int idx;

	classFile->interfaces_count = bufferNextShort(buffer);
	debug_printf(level1, "Interfaces Count : %d.\n", classFile->interfaces_count);
	classFile->interfaces = zalloc(sizeof(ConstantClassInfo *) * classFile->interfaces_count);

	for(idx = 0; idx < classFile->interfaces_count; idx++) {
		uint16_t index = bufferNextShort(buffer);
		debug_printf(level2, "Interface %d : %d.\n", idx, index);
		classFile->interfaces[idx] = getConstant(classFile, index);
	}
}

ClassFile *visitClassFile(ClassBuffer *buffer) {
	unsigned int idx;
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
	classFile->fields_count = bufferNextShort(buffer);
	debug_printf(level1, "Fields Count : %d.\n", classFile->fields_count);
	classFile->fields = zalloc(sizeof(FieldInfo *) * classFile->fields_count);

	for(idx = 0; idx < classFile->fields_count; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		classFile->fields[idx] = visitField(classFile, buffer);
	}

	// Methods Table
	classFile->methods_count = bufferNextShort(buffer);
	debug_printf(level1, "Methods Count : %d.\n", classFile->methods_count);
	classFile->methods = zalloc(sizeof(MethodInfo *) * classFile->methods_count);

	for(idx = 0; idx < classFile->methods_count; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		classFile->methods[idx] = visitMethod(classFile, buffer);
	}

	// Attributes Table
	classFile->attributes_count = bufferNextShort(buffer);
	debug_printf(level1, "Attributes Count : %d.\n", classFile->attributes_count);
	classFile->attributes = zalloc(sizeof(AttributeInfo *) * classFile->attributes_count);

	for(idx = 0; idx < classFile->attributes_count; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		classFile->attributes[idx] = visitAttribute(classFile, buffer);
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

