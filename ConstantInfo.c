
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ConstantInfo.h"

ConstantInfo *createConstant(uint8_t type) {
	ConstantInfo *info;

	switch(type) {
		case CONSTANT_UTF8:
			info = zalloc(sizeof(ConstantUtf8Info));
			memset(info, 0, sizeof(ConstantUtf8Info));
			break;
		case CONSTANT_INTEGER:
			info = zalloc(sizeof(ConstantIntegerInfo));
			memset(info, 0, sizeof(ConstantIntegerInfo));
			break;
		case CONSTANT_FLOAT:
			info = zalloc(sizeof(ConstantFloatInfo));
			memset(info, 0, sizeof(ConstantFloatInfo));
			break;
		case CONSTANT_LONG:
			info = zalloc(sizeof(ConstantLongInfo));
			memset(info, 0, sizeof(ConstantLongInfo));
			break;
		case CONSTANT_DOUBLE:
			info = zalloc(sizeof(ConstantDoubleInfo));
			memset(info, 0, sizeof(ConstantDoubleInfo));
			break;
		case CONSTANT_CLASS:
			info = zalloc(sizeof(ConstantClassInfo));
			memset(info, 0, sizeof(ConstantClassInfo));
			break;
		case CONSTANT_STRING:
			info = zalloc(sizeof(ConstantStringInfo));
			memset(info, 0, sizeof(ConstantStringInfo));
			break;
		case CONSTANT_FIELD_REF:
		case CONSTANT_METHOD_REF:
		case CONSTANT_INTERFACE_METHOD_REF:
			info = zalloc(sizeof(ConstantFieldRefInfo));
			memset(info, 0, sizeof(ConstantFieldRefInfo));
			break;
		case CONSTANT_NAME_AND_TYPE:
			info = zalloc(sizeof(ConstantNameAndTypeInfo));
			memset(info, 0, sizeof(ConstantNameAndTypeInfo));
			break;
		case CONSTANT_METHOD_HANDLE:
			info = zalloc(sizeof(ConstantMethodHandleInfo));
			memset(info, 0, sizeof(ConstantMethodHandleInfo));
			break;
		case CONSTANT_METHOD_TYPE:
			info = zalloc(sizeof(ConstantMethodTypeInfo));
			memset(info, 0, sizeof(ConstantMethodTypeInfo));
			break;
		case CONSTANT_INVOKE_DYNAMIC:
			info = zalloc(sizeof(ConstantInvokeDynamicInfo));
			memset(info, 0, sizeof(ConstantInvokeDynamicInfo));
			break;
		default:
			return NULL;
	}

	info->tag = type;
	return info;
}

void deleteConstant(ConstantInfo *info) {
	if(info == NULL) return;
	if(info->tag == CONSTANT_UTF8)
		free(((ConstantUtf8Info *)info)->bytes);
	free(info);
}

# define createConstant (void *)createConstant

ConstantInfo *visitConstantUtf8(ClassBuffer *buffer) {
	unsigned int idx;
	ConstantUtf8Info *utf8Info;
	utf8Info = createConstant(CONSTANT_UTF8);

	utf8Info->length = bufferNextShort(buffer);
	debug_printf(level3, "Constant Length : %d.\n", utf8Info->length);

	utf8Info->bytes = zalloc(utf8Info->length + 1);
	utf8Info->bytes[utf8Info->length] = '\0';

	for(idx = 0; idx < utf8Info->length; idx++)
		// TODO : Potentially add a mass-read operation
		utf8Info->bytes[idx] = bufferNextByte(buffer);
	debug_printf(level3, "Constant Data : %s.\n", utf8Info->bytes);
	return (ConstantInfo *)utf8Info;
}

ConstantInfo *visitConstantInteger(ClassBuffer *buffer) {
	ConstantIntegerInfo *intInfo;
	intInfo = createConstant(CONSTANT_INTEGER);

	intInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)intInfo;
}

ConstantInfo *visitConstantFloat(ClassBuffer *buffer) {
	ConstantFloatInfo *floatInfo;
	floatInfo = createConstant(CONSTANT_FLOAT);

	floatInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)floatInfo;
}

ConstantInfo *visitConstantLong(ClassBuffer *buffer) {
	ConstantLongInfo *longInfo;
	longInfo = createConstant(CONSTANT_LONG);

	longInfo->high_bytes = bufferNextInt(buffer);
	longInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)longInfo;
}

ConstantInfo *visitConstantDouble(ClassBuffer *buffer) {
	ConstantDoubleInfo *doubleInfo;
	doubleInfo = createConstant(CONSTANT_DOUBLE);

	doubleInfo->high_bytes = bufferNextInt(buffer);
	doubleInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)doubleInfo;
}

ConstantInfo *visitConstantClass(ClassBuffer *buffer) {
	ConstantClassInfo *classInfo;
	classInfo = createConstant(CONSTANT_CLASS);

	classInfo->name_index = bufferNextShort(buffer);
	return (ConstantInfo *)classInfo;
}

ConstantInfo *visitConstantString(ClassBuffer *buffer) {
	ConstantStringInfo *stringInfo;
	stringInfo = createConstant(CONSTANT_STRING);

	stringInfo->string_index = bufferNextShort(buffer);
	return (ConstantInfo *)stringInfo;
}

ConstantInfo *visitConstantFieldRef(ClassBuffer *buffer) {
	ConstantFieldRefInfo *fieldRefInfo;
	fieldRefInfo = createConstant(CONSTANT_FIELD_REF);

	fieldRefInfo->class_index = bufferNextShort(buffer);
	fieldRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)fieldRefInfo;
}

ConstantInfo *visitConstantMethodRef(ClassBuffer *buffer) {
	ConstantMethodRefInfo *methodRefInfo;
	methodRefInfo = createConstant(CONSTANT_METHOD_REF);

	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *visitConstantInterfaceMethodRef(ClassBuffer *buffer) {
	ConstantInterfaceMethodRefInfo *methodRefInfo;
	methodRefInfo = createConstant(CONSTANT_INTERFACE_METHOD_REF);

	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *visitConstantNameAndType(ClassBuffer *buffer) {
	ConstantNameAndTypeInfo *nameAndTypeInfo;
	nameAndTypeInfo = createConstant(CONSTANT_NAME_AND_TYPE);

	nameAndTypeInfo->name_index = bufferNextShort(buffer);
	nameAndTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)nameAndTypeInfo;
}

ConstantInfo *visitConstantMethodHandle(ClassBuffer *buffer) {
	ConstantMethodHandleInfo *methodHandleInfo;
	methodHandleInfo = createConstant(CONSTANT_METHOD_HANDLE);

	methodHandleInfo->reference_kind = bufferNextByte(buffer);
	methodHandleInfo->reference_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodHandleInfo;
}

ConstantInfo *visitConstantMethodType(ClassBuffer *buffer) {
	ConstantMethodTypeInfo *methodTypeInfo;
	methodTypeInfo = createConstant(CONSTANT_METHOD_TYPE);

	methodTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodTypeInfo;
}

ConstantInfo *visitConstantInvokeDynamic(ClassBuffer *buffer) {
	ConstantInvokeDynamicInfo *invokeDynamicInfo;
	invokeDynamicInfo = createConstant(CONSTANT_INVOKE_DYNAMIC);

	invokeDynamicInfo->bootstrap_method_attr_index = bufferNextShort(buffer);
	invokeDynamicInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)invokeDynamicInfo;
}

ConstantInfo *visitConstant(ClassBuffer *buffer) {
	uint8_t tag = bufferNextByte(buffer);

	switch(tag) {
		case CONSTANT_UTF8:
			debug_printf(level2, "Constant UTF8.\n");
			return visitConstantUtf8(buffer);
		case CONSTANT_INTEGER:
			debug_printf(level2, "Constant Integer.\n");
			return visitConstantInteger(buffer);
		case CONSTANT_FLOAT:
			debug_printf(level2, "Constant Float.\n");
			return visitConstantFloat(buffer);
		case CONSTANT_LONG:
			debug_printf(level2, "Constant Long.\n");
			return visitConstantLong(buffer);
		case CONSTANT_DOUBLE:
			debug_printf(level2, "Constant Double.\n");
			return visitConstantDouble(buffer);
		case CONSTANT_CLASS:
			debug_printf(level2, "Constant Class.\n");
			return visitConstantClass(buffer);
		case CONSTANT_STRING:
			debug_printf(level2, "Constant String.\n");
			return visitConstantString(buffer);
		case CONSTANT_FIELD_REF:
			debug_printf(level2, "Constant Field Ref.\n");
			return visitConstantFieldRef(buffer);
		case CONSTANT_METHOD_REF:
			debug_printf(level2, "Constant Method Ref.\n");
			return visitConstantMethodRef(buffer);
		case CONSTANT_INTERFACE_METHOD_REF:
			debug_printf(level2, "Constant Interface Method Ref.\n");
			return visitConstantInterfaceMethodRef(buffer);
		case CONSTANT_NAME_AND_TYPE:
			debug_printf(level2, "Constant Name And Type.\n");
			return visitConstantNameAndType(buffer);
		case CONSTANT_METHOD_HANDLE:
			debug_printf(level2, "Constant Method Handle.\n");
			return visitConstantMethodHandle(buffer);
		case CONSTANT_METHOD_TYPE:
			debug_printf(level2, "Constant Method Type.\n");
			return visitConstantMethodType(buffer);
		case CONSTANT_INVOKE_DYNAMIC:
			debug_printf(level2, "Constant Invoke Dynamic.\n");
			return visitConstantInvokeDynamic(buffer);
		case (uint8_t)-1:
			fprintf(stderr, "Unexpected end of file.\n");
			exit(EXIT_FAILURE);
		default:
			fprintf(stderr, "Unknown tag : %d (%u).\n", tag, bufferPos(buffer));
			exit(EXIT_FAILURE);
	}
}
