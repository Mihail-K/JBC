
# include <stdlib.h>

# include "Debug.h"
# include "ConstantInfo.h"

int encodeConstantUtf8(ClassBuilder *builder, ConstantInfo *info) {
	unsigned int idx;
	ConstantUtf8Info *utf8Info = (ConstantUtf8Info *)info;

	buildNextByte(builder, utf8Info->tag);
	buildNextShort(builder, utf8Info->length);
	for(idx = 0; idx < utf8Info->length; idx++)
		buildNextByte(builder, utf8Info->bytes[idx]);
	return 0;
}

int encodeConstantInteger(ClassBuilder *builder, ConstantInfo *info) {
	ConstantIntegerInfo *intInfo = (ConstantIntegerInfo *)info;

	buildNextByte(builder, intInfo->tag);
	buildNextInt(builder, intInfo->bytes);

	return 0;
}

int encodeConstantFloat(ClassBuilder *builder, ConstantInfo *info) {
	ConstantFloatInfo *floatInfo = (ConstantFloatInfo *)info;

	buildNextByte(builder, floatInfo->tag);
	buildNextInt(builder, floatInfo->bytes);

	return 0;
}

int encodeConstantLong(ClassBuilder *builder, ConstantInfo *info) {
	ConstantLongInfo *longInfo = (ConstantLongInfo *)info;

	buildNextByte(builder, longInfo->tag);
	buildNextInt(builder, longInfo->high_bytes);
	buildNextInt(builder, longInfo->low_bytes);

	return 0;
}

int encodeConstantDouble(ClassBuilder *builder, ConstantInfo *info) {
	ConstantDoubleInfo *doubleInfo = (ConstantDoubleInfo *)info;

	buildNextByte(builder, doubleInfo->tag);
	buildNextInt(builder, doubleInfo->high_bytes);
	buildNextInt(builder, doubleInfo->low_bytes);

	return 0;
}

int encodeConstantClass(ClassBuilder *builder, ConstantInfo *info) {
	ConstantClassInfo *classInfo = (ConstantClassInfo *)info;

	buildNextByte(builder, classInfo->tag);
	buildNextShort(builder, classInfo->name_index);

	return 0;
}

int encodeConstantString(ClassBuilder *builder, ConstantInfo *info) {
	ConstantStringInfo *stringInfo = (ConstantStringInfo *)info;

	buildNextByte(builder, stringInfo->tag);
	buildNextShort(builder, stringInfo->string_index);

	return 0;
}

# define encodeConstantFieldRef encodeConstantRef
# define encodeConstantMethodRef encodeConstantRef
# define encodeConstantInterfaceMethodRef encodeConstantRef

int encodeConstantRef(ClassBuilder *builder, ConstantInfo *info) {
	ConstantRefInfo *refInfo = (ConstantRefInfo *)info;

	buildNextByte(builder, refInfo->tag);
	buildNextShort(builder, refInfo->class_index);
	buildNextShort(builder, refInfo->name_and_type_index);

	return 0;
}

int encodeConstantNameAndType(ClassBuilder *builder, ConstantInfo *info) {
	ConstantNameAndTypeInfo *nameAndTypeInfo = (ConstantNameAndTypeInfo *)info;

	buildNextByte(builder, nameAndTypeInfo->tag);
	buildNextShort(builder, nameAndTypeInfo->name_index);
	buildNextShort(builder, nameAndTypeInfo->descriptor_index);

	return 0;
}

int encodeConstantMethodHandle(ClassBuilder *builder, ConstantInfo *info) {
	ConstantMethodHandleInfo *methodHandleInfo = (ConstantMethodHandleInfo *)info;

	buildNextByte(builder, methodHandleInfo->tag);
	buildNextByte(builder, methodHandleInfo->reference_kind);
	buildNextShort(builder, methodHandleInfo->reference_index);

	return 0;
}

int encodeConstantMethodType(ClassBuilder *builder, ConstantInfo *info) {
	ConstantMethodTypeInfo *methodTypeInfo = (ConstantMethodTypeInfo *)info;

	buildNextByte(builder, methodTypeInfo->tag);
	buildNextShort(builder, methodTypeInfo->descriptor_index);

	return 0;
}

int encodeConstantInvokeDynamic(ClassBuilder *builder, ConstantInfo *info) {
	ConstantInvokeDynamicInfo *invokeDynamicInfo = (ConstantInvokeDynamicInfo *)info;

	buildNextByte(builder, invokeDynamicInfo->tag);
	buildNextShort(builder, invokeDynamicInfo->bootstrap_method_attr_index);
	buildNextShort(builder, invokeDynamicInfo->name_and_type_index);

	return 0;
}

int encodeConstant(ClassBuilder *builder, ConstantInfo *info) {
	if(builder == NULL || info == NULL) return -1;

	switch(info->tag) {
		case CONSTANT_UTF8:
			debug_printf(level2, "Constant UTF8.\n");
			return encodeConstantUtf8(builder, info);
		case CONSTANT_INTEGER:
			debug_printf(level2, "Constant Integer.\n");
			return encodeConstantInteger(builder, info);
		case CONSTANT_FLOAT:
			debug_printf(level2, "Constant Float.\n");
			return encodeConstantFloat(builder, info);
		case CONSTANT_LONG:
			debug_printf(level2, "Constant Long.\n");
			return encodeConstantLong(builder, info);
		case CONSTANT_DOUBLE:
			debug_printf(level2, "Constant Double.\n");
			return encodeConstantDouble(builder, info);
		case CONSTANT_CLASS:
			debug_printf(level2, "Constant Class.\n");
			return encodeConstantClass(builder, info);
		case CONSTANT_STRING:
			debug_printf(level2, "Constant String.\n");
			return encodeConstantString(builder, info);
		case CONSTANT_FIELD_REF:
			debug_printf(level2, "Constant Field Ref.\n");
			return encodeConstantFieldRef(builder, info);
		case CONSTANT_METHOD_REF:
			debug_printf(level2, "Constant Method Ref.\n");
			return encodeConstantMethodRef(builder, info);
		case CONSTANT_INTERFACE_METHOD_REF:
			debug_printf(level2, "Constant Interface Method Ref.\n");
			return encodeConstantInterfaceMethodRef(builder, info);
		case CONSTANT_NAME_AND_TYPE:
			debug_printf(level2, "Constant Name And Type.\n");
			return encodeConstantNameAndType(builder, info);
		case CONSTANT_METHOD_HANDLE:
			debug_printf(level2, "Constant Method Handle.\n");
			return encodeConstantMethodHandle(builder, info);
		case CONSTANT_METHOD_TYPE:
			debug_printf(level2, "Constant Method Type.\n");
			return encodeConstantMethodType(builder, info);
		case CONSTANT_INVOKE_DYNAMIC:
			debug_printf(level2, "Constant Invoke Dynamic.\n");
			return encodeConstantInvokeDynamic(builder, info);
		default:
			fprintf(stderr, "Unknown tag : %d (%ld).\n", info->tag, builderPos(builder));
			exit(EXIT_FAILURE);
	}
}
