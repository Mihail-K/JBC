
# include "Debug.h"
# include "Zalloc.h"
# include "ConstantInfo.h"

# define createConstant (void *)createConstant

ConstantInfo *decodeConstantUtf8(ClassBuffer *buffer) {
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

ConstantInfo *decodeConstantInteger(ClassBuffer *buffer) {
	ConstantIntegerInfo *intInfo;
	intInfo = createConstant(CONSTANT_INTEGER);

	intInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)intInfo;
}

ConstantInfo *decodeConstantFloat(ClassBuffer *buffer) {
	ConstantFloatInfo *floatInfo;
	floatInfo = createConstant(CONSTANT_FLOAT);

	floatInfo->bytes = bufferNextInt(buffer);
	return (ConstantInfo *)floatInfo;
}

ConstantInfo *decodeConstantLong(ClassBuffer *buffer) {
	ConstantLongInfo *longInfo;
	longInfo = createConstant(CONSTANT_LONG);

	longInfo->high_bytes = bufferNextInt(buffer);
	longInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)longInfo;
}

ConstantInfo *decodeConstantDouble(ClassBuffer *buffer) {
	ConstantDoubleInfo *doubleInfo;
	doubleInfo = createConstant(CONSTANT_DOUBLE);

	doubleInfo->high_bytes = bufferNextInt(buffer);
	doubleInfo->low_bytes = bufferNextInt(buffer);
	return (ConstantInfo *)doubleInfo;
}

ConstantInfo *decodeConstantClass(ClassBuffer *buffer) {
	ConstantClassInfo *classInfo;
	classInfo = createConstant(CONSTANT_CLASS);

	classInfo->name_index = bufferNextShort(buffer);
	return (ConstantInfo *)classInfo;
}

ConstantInfo *decodeConstantString(ClassBuffer *buffer) {
	ConstantStringInfo *stringInfo;
	stringInfo = createConstant(CONSTANT_STRING);

	stringInfo->string_index = bufferNextShort(buffer);
	return (ConstantInfo *)stringInfo;
}

ConstantInfo *decodeConstantFieldRef(ClassBuffer *buffer) {
	ConstantFieldRefInfo *fieldRefInfo;
	fieldRefInfo = createConstant(CONSTANT_FIELD_REF);

	fieldRefInfo->class_index = bufferNextShort(buffer);
	fieldRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)fieldRefInfo;
}

ConstantInfo *decodeConstantMethodRef(ClassBuffer *buffer) {
	ConstantMethodRefInfo *methodRefInfo;
	methodRefInfo = createConstant(CONSTANT_METHOD_REF);

	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *decodeConstantInterfaceMethodRef(ClassBuffer *buffer) {
	ConstantInterfaceMethodRefInfo *methodRefInfo;
	methodRefInfo = createConstant(CONSTANT_INTERFACE_METHOD_REF);

	methodRefInfo->class_index = bufferNextShort(buffer);
	methodRefInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodRefInfo;
}

ConstantInfo *decodeConstantNameAndType(ClassBuffer *buffer) {
	ConstantNameAndTypeInfo *nameAndTypeInfo;
	nameAndTypeInfo = createConstant(CONSTANT_NAME_AND_TYPE);

	nameAndTypeInfo->name_index = bufferNextShort(buffer);
	nameAndTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)nameAndTypeInfo;
}

ConstantInfo *decodeConstantMethodHandle(ClassBuffer *buffer) {
	ConstantMethodHandleInfo *methodHandleInfo;
	methodHandleInfo = createConstant(CONSTANT_METHOD_HANDLE);

	methodHandleInfo->reference_kind = bufferNextByte(buffer);
	methodHandleInfo->reference_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodHandleInfo;
}

ConstantInfo *decodeConstantMethodType(ClassBuffer *buffer) {
	ConstantMethodTypeInfo *methodTypeInfo;
	methodTypeInfo = createConstant(CONSTANT_METHOD_TYPE);

	methodTypeInfo->descriptor_index = bufferNextShort(buffer);
	return (ConstantInfo *)methodTypeInfo;
}

ConstantInfo *decodeConstantInvokeDynamic(ClassBuffer *buffer) {
	ConstantInvokeDynamicInfo *invokeDynamicInfo;
	invokeDynamicInfo = createConstant(CONSTANT_INVOKE_DYNAMIC);

	invokeDynamicInfo->bootstrap_method_attr_index = bufferNextShort(buffer);
	invokeDynamicInfo->name_and_type_index = bufferNextShort(buffer);
	return (ConstantInfo *)invokeDynamicInfo;
}

ConstantInfo *decodeConstant(ClassBuffer *buffer) {
	uint8_t tag = bufferNextByte(buffer);

	switch(tag) {
		case CONSTANT_UTF8:
			debug_printf(level2, "Constant UTF8.\n");
			return decodeConstantUtf8(buffer);
		case CONSTANT_INTEGER:
			debug_printf(level2, "Constant Integer.\n");
			return decodeConstantInteger(buffer);
		case CONSTANT_FLOAT:
			debug_printf(level2, "Constant Float.\n");
			return decodeConstantFloat(buffer);
		case CONSTANT_LONG:
			debug_printf(level2, "Constant Long.\n");
			return decodeConstantLong(buffer);
		case CONSTANT_DOUBLE:
			debug_printf(level2, "Constant Double.\n");
			return decodeConstantDouble(buffer);
		case CONSTANT_CLASS:
			debug_printf(level2, "Constant Class.\n");
			return decodeConstantClass(buffer);
		case CONSTANT_STRING:
			debug_printf(level2, "Constant String.\n");
			return decodeConstantString(buffer);
		case CONSTANT_FIELD_REF:
			debug_printf(level2, "Constant Field Ref.\n");
			return decodeConstantFieldRef(buffer);
		case CONSTANT_METHOD_REF:
			debug_printf(level2, "Constant Method Ref.\n");
			return decodeConstantMethodRef(buffer);
		case CONSTANT_INTERFACE_METHOD_REF:
			debug_printf(level2, "Constant Interface Method Ref.\n");
			return decodeConstantInterfaceMethodRef(buffer);
		case CONSTANT_NAME_AND_TYPE:
			debug_printf(level2, "Constant Name And Type.\n");
			return decodeConstantNameAndType(buffer);
		case CONSTANT_METHOD_HANDLE:
			debug_printf(level2, "Constant Method Handle.\n");
			return decodeConstantMethodHandle(buffer);
		case CONSTANT_METHOD_TYPE:
			debug_printf(level2, "Constant Method Type.\n");
			return decodeConstantMethodType(buffer);
		case CONSTANT_INVOKE_DYNAMIC:
			debug_printf(level2, "Constant Invoke Dynamic.\n");
			return decodeConstantInvokeDynamic(buffer);
		case (uint8_t)-1:
			fprintf(stderr, "Unexpected end of file.\n");
			exit(EXIT_FAILURE);
		default:
			fprintf(stderr, "Unknown tag : %d (%u).\n", tag, bufferPos(buffer));
			exit(EXIT_FAILURE);
	}
}
