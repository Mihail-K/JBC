
# include <map>

# include "Debug.h"
# include "ConstantInfo.h"

namespace JBC {

/* Constant Producers */

static
std::map<uint8_t, ConstantProducer> producer_map;

void RegisterProducer(uint8_t tag, ConstantProducer producer) {
	producer_map[tag] = producer;
}

/* Constant Decoders */

ConstantUtf8Info *ConstantUtf8Info
		::DecodeConstant(ClassBuffer *buffer) {
	length = buffer->NextShort();
	debug_printf(level3, "Constant Length : %d.\n", length);

	bytes = new uint8_t[length + 1];
	buffer->Next(bytes, length);
	bytes[length] = '\0';

	debug_printf(level3, "Constant Data : %s.\n", bytes);
	return this;
}

ConstantIntegerInfo *ConstantIntegerInfo
		::DecodeConstant(ClassBuffer *buffer) {
	bytes = buffer->NextInt();
	return this;
}

ConstantFloatInfo *ConstantFloatInfo
		::DecodeConstant(ClassBuffer *buffer) {
	bytes = buffer->NextInt();
	return this;
}

ConstantLongInfo *ConstantLongInfo
		::DecodeConstant(ClassBuffer *buffer) {
	high_bytes = buffer->NextInt();
	low_bytes = buffer->NextInt();
	return this;
}

ConstantDoubleInfo *ConstantDoubleInfo
		::DecodeConstant(ClassBuffer *buffer) {
	high_bytes = buffer->NextInt();
	low_bytes = buffer->NextInt();
	return this;
}

ConstantClassInfo *ConstantClassInfo
		::DecodeConstant(ClassBuffer *buffer) {
	name_index = buffer->NextShort();
	return this;
}

ConstantStringInfo *ConstantStringInfo
		::DecodeConstant(ClassBuffer *buffer) {
	string_index = buffer->NextShort();
	return this;
}

ConstantFieldRefInfo *ConstantFieldRefInfo
		::DecodeConstant(ClassBuffer *buffer) {
	class_index = buffer->NextShort();
	name_and_type_index = buffer->NextShort();
	return this;
}

ConstantMethodRefInfo *ConstantMethodRefInfo
		::DecodeConstant(ClassBuffer *buffer) {
	class_index = buffer->NextShort();
	name_and_type_index = buffer->NextShort();
	return this;
}

ConstantInterfaceMethodRefInfo *ConstantInterfaceMethodRefInfo
		::DecodeConstant(ClassBuffer *buffer) {
	class_index = buffer->NextShort();
	name_and_type_index = buffer->NextShort();
	return this;
}

ConstantNameAndTypeInfo *ConstantNameAndTypeInfo
		::DecodeConstant(ClassBuffer *buffer) {
	name_index = buffer->NextShort();
	descriptor_index = buffer->NextShort();
	return this;
}

ConstantMethodHandleInfo *ConstantMethodHandleInfo
		::DecodeConstant(ClassBuffer *buffer) {
	reference_kind = buffer->NextByte();
	reference_index = buffer->NextShort();
	return this;
}

ConstantMethodTypeInfo *ConstantMethodTypeInfo
		::DecodeConstant(ClassBuffer *buffer) {
	descriptor_index = buffer->NextShort();
	return this;
}

ConstantInvokeDynamicInfo *ConstantInvokeDynamicInfo
		::DecodeConstant(ClassBuffer *buffer) {
	bootstrap_method_attr_index = buffer->NextShort();
	name_and_type_index = buffer->NextShort();
	return this;
}

ConstantInfo *DecodeConstant(ClassBuffer *buffer) {
	uint8_t tag = buffer->NextByte();

	switch(tag) {
		case CONSTANT_UTF8:
			debug_printf(level2, "Constant UTF8.\n");
			return (new ConstantUtf8Info)->DecodeConstant(buffer);
		case CONSTANT_INTEGER:
			debug_printf(level2, "Constant Integer.\n");
			return (new ConstantIntegerInfo)->DecodeConstant(buffer);
		case CONSTANT_FLOAT:
			debug_printf(level2, "Constant Float.\n");
			return (new ConstantFloatInfo)->DecodeConstant(buffer);
		case CONSTANT_LONG:
			debug_printf(level2, "Constant Long.\n");
			return (new ConstantLongInfo)->DecodeConstant(buffer);
		case CONSTANT_DOUBLE:
			debug_printf(level2, "Constant Double.\n");
			return (new ConstantDoubleInfo)->DecodeConstant(buffer);
		case CONSTANT_CLASS:
			debug_printf(level2, "Constant Class.\n");
			return (new ConstantClassInfo)->DecodeConstant(buffer);
		case CONSTANT_STRING:
			debug_printf(level2, "Constant String.\n");
			return (new ConstantStringInfo)->DecodeConstant(buffer);
		case CONSTANT_FIELD_REF:
			debug_printf(level2, "Constant Field Ref.\n");
			return (new ConstantFieldRefInfo)->DecodeConstant(buffer);
		case CONSTANT_METHOD_REF:
			debug_printf(level2, "Constant Method Ref.\n");
			return (new ConstantMethodRefInfo)->DecodeConstant(buffer);
		case CONSTANT_INTERFACE_METHOD_REF:
			debug_printf(level2, "Constant Interface Method Ref.\n");
			return (new ConstantInterfaceMethodRefInfo)->DecodeConstant(buffer);
		case CONSTANT_NAME_AND_TYPE:
			debug_printf(level2, "Constant Name And Type.\n");
			return (new ConstantNameAndTypeInfo)->DecodeConstant(buffer);
		case CONSTANT_METHOD_HANDLE:
			debug_printf(level2, "Constant Method Handle.\n");
			return (new ConstantMethodHandleInfo)->DecodeConstant(buffer);
		case CONSTANT_METHOD_TYPE:
			debug_printf(level2, "Constant Method Type.\n");
			return (new ConstantMethodTypeInfo)->DecodeConstant(buffer);
		case CONSTANT_INVOKE_DYNAMIC:
			debug_printf(level2, "Constant Invoke Dynamic.\n");
			return (new ConstantInvokeDynamicInfo)->DecodeConstant(buffer);
		default: {
			ConstantProducer producer = producer_map[tag];

			// Create a Constant from a Producer
			if(producer != NULL) {
				debug_printf(level2, "Custom Constant from Producer (%hhu).\n", tag);
				return producer(tag, buffer);
			} else {
				char message[64];
				sprintf(message, "Unknown tag : %d (%zu).\n", tag, buffer->Position());
				throw DecodeError(message);
			}
		}
	}
}

} /* JBC */
