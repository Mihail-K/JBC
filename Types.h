# ifndef __TYPES_H__
# define __TYPES_H__

# include <stdint.h>

enum ConstantType {
	CONSTANT_UTF8	= 1,
	CONSTANT_UNUSED0,
	CONSTANT_INTEGER,
	CONSTANT_FLOAT,
	CONSTANT_LONG,
	CONSTANT_DOUBLE,
	CONSTANT_CLASS,
	CONSTANT_STRING,
	CONSTANT_FIELD_REF,
	CONSTANT_METHOD_REF,
	CONSTANT_INTERFACE_METHOD_REF,
	CONSTANT_NAME_AND_TYPE,
	CONSTANT_UNUSED1,
	CONSTANT_UNUSED2,
	CONSTANT_METHOD_HANDLE,
	CONSTANT_METHOD_TYPE,
	CONSTANT_UNUSED3,
	CONSTANT_INVOKE_DYNAMIC
};

typedef struct {
	uint8_t		tag;
} ConstantInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	name_index;
} ConstantClassInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	class_index;
	uint16_t	name_and_type_index;
}
ConstantFieldRefInfo,
ConstantMethodRefInfo,
ConstantInterfaceMethodRefInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	string_index;
} ConstantStringInfo;

typedef struct {
	uint8_t		tag;
	uint32_t	bytes;
}
ConstantIntegerInfo,
ConstantFloatInfo;

typedef struct {
	uint8_t		tag;
	uint32_t	high_bytes;
	uint32_t	low_bytes;
}
ConstantLongInfo,
ConstantDoubleInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	name_index;
	uint16_t	descriptor_index;
} ConstantNameAndTypeInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	length;
	uint8_t		*bytes;
} ConstantUtf8Info;

typedef struct {
	uint8_t		tag;
	uint8_t		reference_kind;
	uint16_t	reference_index;
} ConstantMethodHandleInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	descriptor_index;
} ConstantMethodTypeInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	bootstrap_method_attr_index;
	uint16_t	name_and_type_index;
} ConstantInvokeDynamicInfo;

typedef struct {
	uint32_t	magic;
	uint16_t	major_version;
	uint16_t	minor_version;
	uint16_t	constant_pool_count;
	ConstantInfo **constant_pool;
	uint16_t	access_flags;
	ConstantClassInfo *this_class;
	ConstantClassInfo *super_class;
	uint16_t	interfaces_count;
	ConstantClassInfo **interfaces;
} ClassFile;

# endif /* Types.h */
