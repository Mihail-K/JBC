
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "Types.h"
# include "ClassBuffer.h"

# define debug_printf(fmt, ...) printf(fmt, ## __VA_ARGS__)

void *zalloc(size_t size) {
	void *bytes = malloc(size);
	if(bytes == NULL) {
		perror("zalloc");
		exit(EXIT_FAILURE);
	}
	return bytes;
}

ConstantInfo *visitConstant(ClassBuffer *buffer) {
	int8_t tag = bufferNextByte(buffer);
	switch(tag) {
		case CONSTANT_UTF8: {
			int idx;
			ConstantUtf8Info *utf8Info;
			debug_printf("Constant UTF8.\n");
			utf8Info = zalloc(sizeof(ConstantUtf8Info));
			utf8Info->tag = tag;
			utf8Info->length = bufferNextShort(buffer);
			utf8Info->bytes = zalloc(utf8Info->length);
			for(idx = 0; idx < utf8Info->length; idx++)
				// TODO : Potentially add a mass-read operation
				utf8Info->bytes[idx] = bufferNextByte(buffer);
			return (ConstantInfo *)utf8Info;
		}
		case CONSTANT_INTEGER: {
			ConstantIntegerInfo *intInfo;
			debug_printf("Constant Integer.\n");
			intInfo = zalloc(sizeof(ConstantIntegerInfo));
			intInfo->tag = tag;
			intInfo->bytes = bufferNextInt(buffer);
			return (ConstantInfo *)intInfo;
		}
		case CONSTANT_FLOAT: {
			ConstantFloatInfo *floatInfo;
			debug_printf("Constant Float.\n");
			floatInfo = zalloc(sizeof(ConstantFloatInfo));
			floatInfo->tag = tag;
			floatInfo->bytes = bufferNextInt(buffer);
			return (ConstantInfo *)floatInfo;
		}
		case CONSTANT_LONG: {
			ConstantLongInfo *longInfo;
			debug_printf("Constant Long.\n");
			longInfo = zalloc(sizeof(ConstantLongInfo));
			longInfo->tag = tag;
			longInfo->high_bytes = bufferNextInt(buffer);
			longInfo->low_bytes = bufferNextInt(buffer);
			return (ConstantInfo *)longInfo;
		}
		case CONSTANT_DOUBLE: {
			ConstantDoubleInfo *doubleInfo;
			debug_printf("Constant Double.\n");
			doubleInfo = zalloc(sizeof(ConstantDoubleInfo));
			doubleInfo->tag = tag;
			doubleInfo->high_bytes = bufferNextInt(buffer);
			doubleInfo->low_bytes = bufferNextInt(buffer);
			return (ConstantInfo *)doubleInfo;
		}
		case CONSTANT_CLASS: {
			ConstantClassInfo *classInfo;
			debug_printf("Constant Class.\n");
			classInfo = zalloc(sizeof(ConstantClassInfo));
			classInfo->tag = tag;
			classInfo->name_index = bufferNextShort(buffer);
			return (ConstantInfo *)classInfo;
		}
		case CONSTANT_STRING: {
			ConstantStringInfo *stringInfo;
			debug_printf("Constant String.\n");
			stringInfo = zalloc(sizeof(ConstantStringInfo));
			stringInfo->tag = tag;
			stringInfo->string_index = bufferNextShort(buffer);
			return (ConstantInfo *)stringInfo;
		}
		case CONSTANT_FIELD_REF: {
			ConstantFieldRefInfo *fieldRefInfo;
			debug_printf("Constant Field Ref.\n");
			fieldRefInfo = zalloc(sizeof(ConstantFieldRefInfo));
			fieldRefInfo->tag = tag;
			fieldRefInfo->class_index = bufferNextShort(buffer);
			fieldRefInfo->name_and_type_index = bufferNextShort(buffer);
			return (ConstantInfo *)fieldRefInfo;
		}
		case CONSTANT_METHOD_REF: {
			ConstantMethodRefInfo *methodRefInfo;
			debug_printf("Constant Method Ref.\n");
			methodRefInfo = zalloc(sizeof(ConstantMethodRefInfo));
			methodRefInfo->tag = tag;
			methodRefInfo->class_index = bufferNextShort(buffer);
			methodRefInfo->name_and_type_index = bufferNextShort(buffer);
			return (ConstantInfo *)methodRefInfo;
		}
		case CONSTANT_INTERFACE_METHOD_REF: {
			ConstantInterfaceMethodRefInfo *methodRefInfo;
			debug_printf("Constant Interface Method Ref.\n");
			methodRefInfo = zalloc(sizeof(ConstantInterfaceMethodRefInfo));
			methodRefInfo->tag = tag;
			methodRefInfo->class_index = bufferNextShort(buffer);
			methodRefInfo->name_and_type_index = bufferNextShort(buffer);
			return (ConstantInfo *)methodRefInfo;
		}
		case CONSTANT_NAME_AND_TYPE: {
			ConstantNameAndTypeInfo *nameAndTypeInfo;
			debug_printf("Constant Name And Type.\n");
			nameAndTypeInfo = zalloc(sizeof(ConstantNameAndTypeInfo));
			nameAndTypeInfo->tag = tag;
			nameAndTypeInfo->name_index = bufferNextShort(buffer);
			nameAndTypeInfo->descriptor_index = bufferNextShort(buffer);
			return (ConstantInfo *)nameAndTypeInfo;
		}
		case CONSTANT_METHOD_HANDLE: {
			ConstantMethodHandleInfo *methodHandleInfo;
			debug_printf("Constant Method Handle.\n");
			methodHandleInfo = zalloc(sizeof(ConstantMethodHandleInfo));
			methodHandleInfo->tag = tag;
			methodHandleInfo->reference_kind = bufferNextByte(buffer);
			methodHandleInfo->reference_index = bufferNextShort(buffer);
			return (ConstantInfo *)methodHandleInfo;
		}
		case CONSTANT_METHOD_TYPE: {
			ConstantMethodTypeInfo *methodTypeInfo;
			debug_printf("Constant Method Type.\n");
			methodTypeInfo = zalloc(sizeof(ConstantMethodTypeInfo));
			methodTypeInfo->tag = tag;
			methodTypeInfo->descriptor_index = bufferNextShort(buffer);
			return (ConstantInfo *)methodTypeInfo;
		}
		case CONSTANT_INVOKE_DYNAMIC: {
			ConstantInvokeDynamicInfo *invokeDynamicInfo;
			debug_printf("Constant Invoke Dynamic.\n");
			invokeDynamicInfo = zalloc(sizeof(ConstantInvokeDynamicInfo));
			invokeDynamicInfo->tag = tag;
			invokeDynamicInfo->bootstrap_method_attr_index = bufferNextShort(buffer);
			invokeDynamicInfo->name_and_type_index = bufferNextShort(buffer);
			return (ConstantInfo *)invokeDynamicInfo;
		}
		case -1:
			fprintf(stderr, "Unexpected end of file.\n");
			exit(EXIT_FAILURE);
		default:
			fprintf(stderr, "Unknown tag : %d.\n", tag);
			exit(EXIT_FAILURE);
	}
}

ConstantInfo *getConstant(ClassFile *classFile, int idx) {
	ConstantInfo *info = classFile->constant_pool[idx];
	if(info == NULL) {
		fprintf(stderr, "Constant not found. (ID : %d)\n", idx);
		exit(EXIT_FAILURE);
	}
	return info;
}

void *visitAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	return NULL;
}

FieldInfo *visitField(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	FieldInfo *field = zalloc(sizeof(FieldInfo));

	field->access_flags = bufferNextShort(buffer);
	field->name = (void *)getConstant(classFile, bufferNextShort(buffer));
	field->descriptor = (void *)getConstant(classFile, bufferNextShort(buffer));

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

	classFile->constant_pool_count = bufferNextShort(buffer);
	debug_printf("Constant Pool Count : %d.\n", classFile->constant_pool_count);

	classFile->constant_pool = zalloc(sizeof(ConstantInfo *) *
			classFile->constant_pool_count);
	classFile->constant_pool[0] = NULL;
	for(idx = 1; idx < classFile->constant_pool_count; idx++) {
		debug_printf("Constant %d : ", idx);
		classFile->constant_pool[idx] = visitConstant(buffer);
	}

	classFile->access_flags = bufferNextShort(buffer);
	classFile->this_class = (void *)getConstant(classFile, bufferNextShort(buffer));
	classFile->super_class = (void *)getConstant(classFile, bufferNextShort(buffer));

	debug_printf("Access Flags : %#X.\n", classFile->access_flags);
	debug_printf("This Class : %d.\n", classFile->this_class->name_index);
	debug_printf("Super Class : %d.\n", classFile->super_class->name_index);

	classFile->interfaces_count = bufferNextShort(buffer);
	debug_printf("Interfaces Count : %d.\n", classFile->interfaces_count);
	classFile->interfaces = zalloc(sizeof(ConstantClassInfo *) * classFile->interfaces_count);

	for(idx = 0; idx < classFile->interfaces_count; idx++) {
		classFile->interfaces[idx] = (void *)getConstant(classFile, bufferNextShort(buffer));
		debug_printf("Interface %d : %d.\n", idx, classFile->interfaces[idx]->name_index);
	}

	classFile->fields_count = bufferNextShort(buffer);
	debug_printf("Fields Count : %d.\n", classFile->fields_count);
	classFile->fields = zalloc(sizeof(FieldInfo *) * classFile->fields_count);

	for(idx = 0; idx < classFile->fields_count; idx++) {
		classFile->fields[idx] = visitField(classFile, buffer);
	}
}

int main() {
	FILE *file;
	char *path = "C:\\Users\\Mihail.K\\Documents\\Clicker.class";
	if((file = fopen(path, "rb")) == NULL) {
		perror("fopen");
		return 1;
	}
	visitClassFile(createBuffer(file));
	return 0;
}
