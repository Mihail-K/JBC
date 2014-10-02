
# include <stdio.h>
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassBuffer.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

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
		if(isLongConstant(classFile->constant_pool[idx])) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			classFile->constant_pool[++idx] = NULL;
		}
	}
}

void visitThisClass(ClassFile *classFile, ClassBuffer *buffer) {
	classFile->this_class_index = bufferNextShort(buffer);
	debug_printf(level3, "This Class : %d.\n", classFile->this_class_index);
	classFile->this_class = getConstant(classFile, classFile->this_class_index);
}

void visitSuperClass(ClassFile *classFile, ClassBuffer *buffer) {
	classFile->super_class_index = bufferNextShort(buffer);
	debug_printf(level3, "Super Class : %d.\n", classFile->super_class_index);
	classFile->super_class = getConstant(classFile, classFile->super_class_index);
}

void visitInterfaces(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx;

	classFile->interfaces_count = bufferNextShort(buffer);
	debug_printf(level1, "Interfaces Count : %d.\n", classFile->interfaces_count);
	classFile->interface_indexes = zalloc(sizeof(uint16_t) * classFile->interfaces_count);
	classFile->interfaces = zalloc(sizeof(ConstantClassInfo *) * classFile->interfaces_count);

	for(idx = 0; idx < classFile->interfaces_count; idx++) {
		classFile->interface_indexes[idx] = bufferNextShort(buffer);
		debug_printf(level2, "Interface %d : %d.\n", idx, classFile->interface_indexes[idx]);
		classFile->interfaces[idx] = getConstant(classFile, classFile->interface_indexes[idx]);
	}
}

ClassFile *visitClassFile(ClassBuffer *buffer) {
	unsigned int idx;
	ClassFile *classFile = zalloc(sizeof(ClassFile));

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

