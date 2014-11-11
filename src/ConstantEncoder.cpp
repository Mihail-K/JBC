
# include <stdlib.h>

# include "Debug.h"
# include "ConstantInfo.h"

ConstantUtf8Info *ConstantUtf8Info
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant UTF8.\n");

	builder->NextByte(tag);
	builder->NextShort(length);
	builder->Next(bytes, length);

	return this;
}

ConstantIntegerInfo *ConstantIntegerInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Integer.\n");

	builder->NextByte(tag);
	builder->NextInt(bytes);

	return this;
}

ConstantFloatInfo *ConstantFloatInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Float.\n");

	builder->NextByte(tag);
	builder->NextInt(bytes);

	return this;
}

ConstantLongInfo *ConstantLongInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Long.\n");

	builder->NextByte(tag);
	builder->NextInt(high_bytes);
	builder->NextInt(low_bytes);

	return this;
}

ConstantDoubleInfo *ConstantDoubleInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Double.\n");

	builder->NextByte(tag);
	builder->NextInt(high_bytes);
	builder->NextInt(low_bytes);

	return this;
}

ConstantClassInfo *ConstantClassInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Class.\n");

	builder->NextByte(tag);
	builder->NextShort(name_index);

	return this;
}

ConstantStringInfo *ConstantStringInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant String.\n");

	builder->NextByte(tag);
	builder->NextShort(string_index);

	return this;
}

ConstantFieldRefInfo *ConstantFieldRefInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Field Ref.\n");

	builder->NextByte(tag);
	builder->NextShort(class_index);
	builder->NextShort(name_and_type_index);

	return this;
}

ConstantMethodRefInfo *ConstantMethodRefInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Method Ref.\n");

	builder->NextByte(tag);
	builder->NextShort(class_index);
	builder->NextShort(name_and_type_index);

	return this;
}

ConstantInterfaceMethodRefInfo *ConstantInterfaceMethodRefInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Interface Method Ref.\n");

	builder->NextByte(tag);
	builder->NextShort(class_index);
	builder->NextShort(name_and_type_index);

	return this;
}

ConstantNameAndTypeInfo *ConstantNameAndTypeInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Name And Type.\n");

	builder->NextByte(tag);
	builder->NextShort(name_index);
	builder->NextShort(descriptor_index);

	return this;
}

ConstantMethodHandleInfo *ConstantMethodHandleInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Method Handle.\n");

	builder->NextByte(tag);
	builder->NextByte(reference_kind);
	builder->NextShort(reference_index);

	return this;
}

ConstantMethodTypeInfo *ConstantMethodTypeInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Method Type.\n");

	builder->NextByte(tag);
	builder->NextShort(descriptor_index);

	return this;
}

ConstantInvokeDynamicInfo *ConstantInvokeDynamicInfo
		::EncodeConstant(ClassBuilder *builder) {
	debug_printf(level2, "Encoding Constant Invoke Dynamic.\n");

	builder->NextByte(tag);
	builder->NextShort(bootstrap_method_attr_index);
	builder->NextShort(name_and_type_index);

	return this;
}

int encodeConstant(ClassBuilder *builder, ConstantInfo *info) {
	if(builder == NULL || info == NULL) return -1;
	info->EncodeConstant(builder);
	return 0;
}
