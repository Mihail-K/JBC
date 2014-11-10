# ifndef __CONSTANTINFO_H__
# define __CONSTANTINFO_H__

# include <string>
# include <stdint.h>

# include "Defines.h"

# include "ClassBuffer.h"
# include "ClassBuilder.h"

enum ConstantType {
	CONSTANT_UTF8					= 1,
	CONSTANT_UNUSED0,
	CONSTANT_INTEGER				= 3,
	CONSTANT_FLOAT					= 4,
	CONSTANT_LONG					= 5,
	CONSTANT_DOUBLE					= 6,
	CONSTANT_CLASS					= 7,
	CONSTANT_STRING					= 8,
	CONSTANT_FIELD_REF				= 9,
	CONSTANT_METHOD_REF				= 10,
	CONSTANT_INTERFACE_METHOD_REF	= 11,
	CONSTANT_NAME_AND_TYPE			= 12,
	CONSTANT_UNUSED1,
	CONSTANT_UNUSED2,
	CONSTANT_METHOD_HANDLE			= 15,
	CONSTANT_METHOD_TYPE			= 16,
	CONSTANT_UNUSED3,
	CONSTANT_INVOKE_DYNAMIC			= 18
};

struct ConstantError {

	const std::string msg;

	inline
	ConstantError(const char *msg)
		: msg(std::string(msg)) {
	}

};

/* Constant Info */

struct ConstantInfo {
	uint8_t		tag;
	uint16_t	index;

	ConstantInfo() {
	}

	ConstantInfo(uint8_t tag)
		: tag(tag) {
	}

	virtual
	~ConstantInfo() {
	}

	virtual
	bool IsLongConstant() {
		return false;
	}

	virtual
	ConstantInfo *DecodeConstant(ClassBuffer *buffer) = 0;

	virtual
	ConstantInfo *EncodeConstant(ClassBuilder *buider) = 0;
};

struct ConstantClassInfo
		: public ConstantInfo {
	uint16_t	name_index;

	ConstantClassInfo()
		: ConstantInfo(CONSTANT_CLASS) {
	}

	ConstantClassInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantClassInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantFieldRefInfo
		: public ConstantInfo {
	uint16_t	class_index;
	uint16_t	name_and_type_index;

	ConstantFieldRefInfo()
		: ConstantInfo(CONSTANT_FIELD_REF) {
	}

	ConstantFieldRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantFieldRefInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantMethodRefInfo
		: public ConstantInfo {
	uint16_t	class_index;
	uint16_t	name_and_type_index;

	ConstantMethodRefInfo()
		: ConstantInfo(CONSTANT_METHOD_REF) {
	}

	ConstantMethodRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodRefInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantInterfaceMethodRefInfo
		: public ConstantInfo {
	uint16_t	class_index;
	uint16_t	name_and_type_index;

	ConstantInterfaceMethodRefInfo()
		: ConstantInfo(CONSTANT_INTERFACE_METHOD_REF) {
	}

	ConstantInterfaceMethodRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantInterfaceMethodRefInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantStringInfo
		: public ConstantInfo {
	uint16_t	string_index;

	ConstantStringInfo()
		: ConstantInfo(CONSTANT_STRING) {
	}

	ConstantStringInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantStringInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantIntegerInfo
		: public ConstantInfo {
	uint32_t bytes;

	ConstantIntegerInfo()
		: ConstantInfo(CONSTANT_INTEGER) {
	}

	ConstantIntegerInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantIntegerInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantFloatInfo
		: public ConstantInfo {
	uint32_t bytes;

	ConstantFloatInfo()
		: ConstantInfo(CONSTANT_FLOAT) {
	}

	ConstantFloatInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantFloatInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantLongInfo
		: public ConstantInfo {
	uint32_t	high_bytes;
	uint32_t	low_bytes;

	ConstantLongInfo()
		: ConstantInfo(CONSTANT_LONG) {
	}

	virtual
	bool IsLongConstant() {
		return true;
	}

	ConstantLongInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantLongInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantDoubleInfo
		: public ConstantInfo {
	uint32_t	high_bytes;
	uint32_t	low_bytes;

	ConstantDoubleInfo()
		: ConstantInfo(CONSTANT_DOUBLE) {
	}

	virtual
	bool IsLongConstant() {
		return true;
	}

	ConstantDoubleInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantDoubleInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantNameAndTypeInfo
		: public ConstantInfo {
	uint16_t	name_index;
	uint16_t	descriptor_index;

	ConstantNameAndTypeInfo()
		: ConstantInfo(CONSTANT_NAME_AND_TYPE) {
	}

	ConstantNameAndTypeInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantNameAndTypeInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantUtf8Info
		: public ConstantInfo {
	uint16_t	length;
	uint8_t		*bytes;

	ConstantUtf8Info()
		: ConstantInfo(CONSTANT_UTF8) {
	}

	~ConstantUtf8Info() {
		if(bytes != NULL) {
			delete bytes;
		}
	}

	ConstantUtf8Info *DecodeConstant(ClassBuffer *buffer);

	ConstantUtf8Info *EncodeConstant(ClassBuilder *builder);
};

struct ConstantMethodHandleInfo
		: public ConstantInfo {
	uint8_t		reference_kind;
	uint16_t	reference_index;

	ConstantMethodHandleInfo()
		: ConstantInfo(CONSTANT_METHOD_HANDLE) {
	}

	ConstantMethodHandleInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodHandleInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantMethodTypeInfo
		: public ConstantInfo {
	uint16_t	descriptor_index;

	ConstantMethodTypeInfo()
		: ConstantInfo(CONSTANT_METHOD_TYPE) {
	}

	ConstantMethodTypeInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodTypeInfo *EncodeConstant(ClassBuilder *builder);
};

struct ConstantInvokeDynamicInfo
		: public ConstantInfo {
	uint16_t	bootstrap_method_attr_index;
	uint16_t	name_and_type_index;

	ConstantInvokeDynamicInfo()
		: ConstantInfo(CONSTANT_INVOKE_DYNAMIC) {
	}

	ConstantInvokeDynamicInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantInvokeDynamicInfo *EncodeConstant(ClassBuilder *builder);
};

int encodeConstant(ClassBuilder *builder, ConstantInfo *info);

ConstantInfo *decodeConstant(ClassBuffer *buffer);

static inline
int isLongConstant(ConstantInfo *info) {
	if(info == NULL) return 0;
	return info->IsLongConstant();
}

# endif /* ConstantInfo.h */
