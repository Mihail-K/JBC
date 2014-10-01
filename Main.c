
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "Debug.h"
# include "Types.h"
# include "Zalloc.h"
# include "ClassBuffer.h"
# include "ConstantInfo.h"

void visitConstantPool(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;

	classFile->constant_pool_count = bufferNextShort(buffer);
	debug_printf("Constant Pool Count : %d.\n", classFile->constant_pool_count);

	classFile->constant_pool = zalloc(sizeof(ConstantInfo *) *
			classFile->constant_pool_count);
	classFile->constant_pool[0] = NULL;
	for(idx = 1; idx < classFile->constant_pool_count; idx++) {
		debug_printf("Constant %d : ", idx);
		classFile->constant_pool[idx] = visitConstant(buffer);
	}
}

ConstantInfo *getConstant(ClassFile *classFile, int idx) {
	if(idx < 0 || idx >= classFile->constant_pool_count) {
		fprintf(stderr, "Constant not found. (ID : %d)\n", idx);
		exit(EXIT_FAILURE);
	}

	return classFile->constant_pool[idx];
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

void *visitAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	return NULL;
}

FieldInfo *visitField(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	FieldInfo *field = zalloc(sizeof(FieldInfo));

	field->access_flags = bufferNextShort(buffer);

	field->name_index = bufferNextShort(buffer);
	field->name = (void *)getConstant(classFile, field->name_index);

	field->descriptor_index = bufferNextShort(buffer);
	field->descriptor = (void *)getConstant(classFile, field->descriptor_index);

	field->attributes_count = bufferNextShort(buffer);
	field->attributes = zalloc(sizeof(void *) * field->attributes_count);

	for(idx = 0; idx < field->attributes_count; idx++) {
		field->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return field;
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

	classFile->fields_count = bufferNextShort(buffer);
	debug_printf("Fields Count : %d.\n", classFile->fields_count);
	classFile->fields = zalloc(sizeof(FieldInfo *) * classFile->fields_count);

	for(idx = 0; idx < classFile->fields_count; idx++) {
		classFile->fields[idx] = visitField(classFile, buffer);
	}
}

int main(int argc, char **argv) {
	FILE *file;
	char *path = argv[1];
	if((file = fopen(path, "rb")) == NULL) {
		perror("fopen");
		return 1;
	}
	visitClassFile(createBuffer(file));
	return 0;
}