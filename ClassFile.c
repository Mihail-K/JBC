
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
	int idx;

	classFile->constant_pool_count = bufferNextShort(buffer);
	debug_printf("Constant Pool Count : %d.\n", classFile->constant_pool_count);

	classFile->constant_pool = zalloc(sizeof(ConstantInfo *) *
			classFile->constant_pool_count);
	classFile->constant_pool[0] = NULL;
	for(idx = 1; idx < classFile->constant_pool_count; idx++) {
		debug_printf("Constant %d :\n", idx);
		classFile->constant_pool[idx] = visitConstant(buffer);
	}
}

void visitThisClass(ClassFile *classFile, ClassBuffer *buffer) {
	classFile->this_class_index = bufferNextShort(buffer);
	debug_printf("This Class : %d.\n", classFile->this_class_index);
	classFile->this_class = (void *)getConstant(classFile, classFile->this_class_index);
}

void visitSuperClass(ClassFile *classFile, ClassBuffer *buffer) {
	classFile->super_class_index = bufferNextShort(buffer);
	debug_printf("Super Class : %d.\n", classFile->super_class_index);
	classFile->super_class = (void *)getConstant(classFile, classFile->super_class_index);
}

void visitInterfaces(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;

	classFile->interfaces_count = bufferNextShort(buffer);
	debug_printf("Interfaces Count : %d.\n", classFile->interfaces_count);
	classFile->interface_indexes = zalloc(sizeof(uint16_t) * classFile->interfaces_count);
	classFile->interfaces = zalloc(sizeof(ConstantClassInfo *) * classFile->interfaces_count);

	for(idx = 0; idx < classFile->interfaces_count; idx++) {
		classFile->interface_indexes[idx] = bufferNextShort(buffer);
		debug_printf("Interface %d : %d.\n", idx, classFile->interface_indexes[idx]);
		classFile->interfaces[idx] = (void *)getConstant(classFile, classFile->interface_indexes[idx]);
	}
}

ClassFile *visitClassFile(ClassBuffer *buffer) {
	int idx;
	ClassFile *classFile = zalloc(sizeof(ClassFile));

	classFile->magic = bufferNextInt(buffer);
	classFile->major_version = bufferNextShort(buffer);
	classFile->minor_version = bufferNextShort(buffer);

	debug_printf("Magic : %#X.\n", classFile->magic);
	debug_printf("Major Version : %d.\n", classFile->major_version);
	debug_printf("Minor Version : %d.\n", classFile->minor_version);

	visitConstantPool(classFile, buffer);

	classFile->access_flags = bufferNextShort(buffer);
	debug_printf("Access Flags : %#X.\n", classFile->access_flags);

	visitThisClass(classFile, buffer);
	visitSuperClass(classFile, buffer);
	visitInterfaces(classFile, buffer);

	// Fields Table
	classFile->fields_count = bufferNextShort(buffer);
	debug_printf("Fields Count : %d.\n", classFile->fields_count);
	classFile->fields = zalloc(sizeof(FieldInfo *) * classFile->fields_count);

	for(idx = 0; idx < classFile->fields_count; idx++) {
		debug_printf("Field %d :\n", idx);
		classFile->fields[idx] = visitField(classFile, buffer);
		classFile->fields[idx]->member_type = MT_FIELD;
	}

	// Methods Table
	classFile->methods_count = bufferNextShort(buffer);
	debug_printf("Methods Count : %d.\n", classFile->methods_count);
	classFile->methods = zalloc(sizeof(MethodInfo *) * classFile->methods_count);

	for(idx = 0; idx < classFile->methods_count; idx++) {
		debug_printf("Method %d :\n", idx);
		classFile->methods[idx] = visitMethod(classFile, buffer);
		classFile->methods[idx]->member_type = MT_METHOD;
	}

	// Attributes Table
	classFile->attributes_count = bufferNextShort(buffer);
	debug_printf("Attributes Count : %d.\n", classFile->attributes_count);
	classFile->attributes = zalloc(sizeof(AttributeInfo *) * classFile->attributes_count);

	for(idx = 0; idx < classFile->attributes_count; idx++) {
		debug_printf("Attribute %d :\n", idx);
		classFile->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return classFile;
}

ClassFile *decodeClassFile(FILE *source) {
	ClassBuffer *buffer = createBuffer(source);
	return visitClassFile(buffer);
}

