
# include "Debug.h"
# include "Zalloc.h"
# include "ConstantInfo.h"

ConstantInfo *visitConstantUtf8(ClassBuffer *buffer) {
	unsigned int idx;
	ConstantUtf8Info *utf8Info;
	utf8Info = zalloc(sizeof(ConstantUtf8Info));

	utf8Info->tag = CONSTANT_UTF8;
	utf8Info->length = bufferNextShort(buffer);
	debug_printf("Constant Length : %d.\n", utf8Info->length);

	utf8Info->bytes = zalloc(utf8Info->length + 1);
	utf8Info->bytes[utf8Info->length] = '\0';

	for(idx = 0; idx < utf8Info->length; idx++)
		// TODO : Potentially add a mass-read operation
		utf8Info->bytes[idx] = bufferNextByte(buffer);
	debug_printf("Constant Data : %s.\n", utf8Info->bytes);
	return (ConstantInfo *)utf8Info;
}

ConstantInfo *visitConstantInteger(ClassBuffer *buffer) {
	ConstantIntegerInfo *intInfo;
	intInfo = zalloc(sizeof(ConstantIntegerInfo));

	intInfo->tag = CONSTANT_INTEGER;
	intInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)intInfo;
}

ConstantInfo *visitConstantFloat(ClassBuffer *buffer) {
	ConstantFloatInfo *floatInfo;
	floatInfo = zalloc(sizeof(ConstantFloatInfo));

	floatInfo->tag = CONSTANT_FLOAT;
	floatInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)floatInfo;
}

ConstantInfo *visitConstantLong(ClassBuffer *buffer) {
	ConstantLongInfo *longInfo;
	longInfo = zalloc(sizeof(ConstantLongInfo));

	longInfo->tag = CONSTANT_LONG;
	longInfo->high_bytes = bufferNextInt(buffer);
	longInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)longInfo;
}

ConstantInfo *visitConstantDouble(ClassBuffer *buffer) {
	ConstantDoubleInfo *doubleInfo;
	doubleInfo = zalloc(sizeof(ConstantDoubleInfo));

	doubleInfo->tag = CONSTANT_DOUBLE;
	doubleInfo->high_bytes = bufferNextInt(buffer);
	doubleInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)doubleInfo;
}

ConstantInfo *visitConstantClass(ClassBuffer *buffer) {
	ConstantClassInfo *classInfo;
	classInfo = zalloc(sizeof(ConstantClassInfo));

	classInfo->tag = CONSTANT_CLASS;
	classInfo->name_index = bufferNextShort(buffer);
	return (ConstantInfo *)classInfo;
}

ConstantInfo *visitConstantString(ClassBuffer *buffer) {
	ConstantStringInfo *stringInfo;
	stringInfo = zalloc(sizeof(ConstantStringInfo));

	stringInfo->tag = CONSTANT_STRING;
	stringInfo->string_index = bufferNextShort(buffer);
	return (ConstantInfo *)stringInfo;
}

ConstantInfo *visitConstantFieldRef(ClassBuffer *buffer) {
	ConstantFieldRefInfo *fieldRefInfo;
	fieldRefInfo = zalloc(sizeof(ConstantFieldRefInfo));

	fieldRefInfo->tag = CONSTANT_FIELD_REF;
	fieldRefInfo->class_index = bufferNextShort(buffer);
	fieldRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)fieldRefInfo;
}

ConstantInfo *visitConstantMethodRef(ClassBuffer *buffer) {
	ConstantMethodRefInfo *methodRefInfo;
	methodRefInfo = zalloc(sizeof(ConstantMethodRefInfo));

	methodRefInfo->tag = CONSTANT_METHOD_REF;
	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *visitConstantInterfaceMethodRef(ClassBuffer *buffer) {
	ConstantInterfaceMethodRefInfo *methodRefInfo;
	methodRefInfo = zalloc(sizeof(ConstantInterfaceMethodRefInfo));

	methodRefInfo->tag = CONSTANT_INTERFACE_METHOD_REF;
	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *visitConstantNameAndType(ClassBuffer *buffer) {
	ConstantNameAndTypeInfo *nameAndTypeInfo;
	nameAndTypeInfo = zalloc(sizeof(ConstantNameAndTypeInfo));

	nameAndTypeInfo->tag = CONSTANT_NAME_AND_TYPE;
	nameAndTypeInfo->name_index = bufferNextShort(buffer);
	nameAndTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)nameAndTypeInfo;
}

ConstantInfo *visitConstantMethodHandle(ClassBuffer *buffer) {
	ConstantMethodHandleInfo *methodHandleInfo;
	methodHandleInfo = zalloc(sizeof(ConstantMethodHandleInfo));

	methodHandleInfo->tag = CONSTANT_METHOD_HANDLE;
	methodHandleInfo->reference_kind = bufferNextByte(buffer);
	methodHandleInfo->reference_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodHandleInfo;
}

ConstantInfo *visitConstantMethodType(ClassBuffer *buffer) {
	ConstantMethodTypeInfo *methodTypeInfo;
	methodTypeInfo = zalloc(sizeof(ConstantMethodTypeInfo));

	methodTypeInfo->tag = CONSTANT_METHOD_TYPE;
	methodTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodTypeInfo;
}

ConstantInfo *visitConstantInvokeDynamic(ClassBuffer *buffer) {
	ConstantInvokeDynamicInfo *invokeDynamicInfo;
	invokeDynamicInfo = zalloc(sizeof(ConstantInvokeDynamicInfo));

	invokeDynamicInfo->tag = CONSTANT_INVOKE_DYNAMIC;
	invokeDynamicInfo->bootstrap_method_attr_index = bufferNextShort(buffer);
	invokeDynamicInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)invokeDynamicInfo;
}

ConstantInfo *visitConstant(ClassBuffer *buffer) {
	uint8_t tag = bufferNextByte(buffer);

	switch(tag) {
		case CONSTANT_UTF8:
			debug_printf("Constant UTF8.\n");
			return visitConstantUtf8(buffer);
		case CONSTANT_INTEGER:
			debug_printf("Constant Integer.\n");
			return visitConstantInteger(buffer);
		case CONSTANT_FLOAT:
			debug_printf("Constant Float.\n");
			return visitConstantFloat(buffer);
		case CONSTANT_LONG:
			debug_printf("Constant Long.\n");
			return visitConstantLong(buffer);
		case CONSTANT_DOUBLE:
			debug_printf("Constant Double.\n");
			return visitConstantDouble(buffer);
		case CONSTANT_CLASS:
			debug_printf("Constant Class.\n");
			return visitConstantClass(buffer);
		case CONSTANT_STRING:
			debug_printf("Constant String.\n");
			return visitConstantString(buffer);
		case CONSTANT_FIELD_REF:
			debug_printf("Constant Field Ref.\n");
			return visitConstantFieldRef(buffer);
		case CONSTANT_METHOD_REF:
			debug_printf("Constant Method Ref.\n");
			return visitConstantMethodRef(buffer);
		case CONSTANT_INTERFACE_METHOD_REF:
			debug_printf("Constant Interface Method Ref.\n");
			return visitConstantInterfaceMethodRef(buffer);
		case CONSTANT_NAME_AND_TYPE:
			debug_printf("Constant Name And Type.\n");
			return visitConstantNameAndType(buffer);
		case CONSTANT_METHOD_HANDLE:
			debug_printf("Constant Method Handle.\n");
			return visitConstantMethodHandle(buffer);
		case CONSTANT_METHOD_TYPE:
			debug_printf("Constant Method Type.\n");
			return visitConstantMethodType(buffer);
		case CONSTANT_INVOKE_DYNAMIC:
			debug_printf("Constant Invoke Dynamic.\n");
			return visitConstantInvokeDynamic(buffer);
		case (uint8_t)-1:
			fprintf(stderr, "Unexpected end of file.\n");
			exit(EXIT_FAILURE);
		default:
			fprintf(stderr, "Unknown tag : %d (%u).\n", tag, bufferPos(buffer));
			exit(EXIT_FAILURE);
	}
}
