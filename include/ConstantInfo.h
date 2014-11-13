/**
 * @file ConstantInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.42
 *
 * @brief Defines all ConstantInfo types.
 **/
# ifndef __CONSTANTINFO_H__
# define __CONSTANTINFO_H__

# include <string>
# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

namespace JBC {

/**
 * @file ConstantInfo.h
 * @enum ConstantType
 * @brief An enumeration of the constant types found in Java class files.
 **/
enum ConstantType {
	CONSTANT_UTF8					= 1,  /**< A UTF-8 encoded string constant.					*/
	CONSTANT_UNUSED0,					  /**< Unused											*/
	CONSTANT_INTEGER				= 3,  /**< A 32-bit integer value constant.					*/
	CONSTANT_FLOAT					= 4,  /**< A 32-bit float value constant.					*/
	CONSTANT_LONG					= 5,  /**< A 64-bit integer value constant					*/
	CONSTANT_DOUBLE					= 6,  /**< A 64-bit float value constant.					*/
	CONSTANT_CLASS					= 7,  /**< A Java Class constant.							*/
	CONSTANT_STRING					= 8,  /**< A Java String constant.							*/
	CONSTANT_FIELD_REF				= 9,  /**< A constant referencing a Java field.				*/
	CONSTANT_METHOD_REF				= 10, /**< A constant referencing a Java method.			*/
	CONSTANT_INTERFACE_METHOD_REF	= 11, /**< A constant referencing an interface method.		*/
	CONSTANT_NAME_AND_TYPE			= 12, /**< A constant defining a Name and Type pair.		*/
	CONSTANT_UNUSED1,					  /**< Unused											*/
	CONSTANT_UNUSED2,					  /**< Unused											*/
	CONSTANT_METHOD_HANDLE			= 15, /**< A constant defining a Java method handle.		*/
	CONSTANT_METHOD_TYPE			= 16, /**< A constant defining a method descriptor.			*/
	CONSTANT_UNUSED3,					  /**< Unused											*/
	CONSTANT_INVOKE_DYNAMIC			= 18  /**< A constant specifying a dynamic invocation name.	*/
};

/* Constant Info */

/**
 * @struct ConstantInfo
 * @brief The parent type to all constant values.
 *
 * ConstantInfo represents a common type between all constant values,
 * including the definition of their identifier tag, and the position
 * of the constant in the constant pool (which is used when encoding).
 **/
struct ConstantInfo {
	/**
	 * @brief The identifier tag, used to differenciate constant types.
	 **/
	uint8_t		tag;
	/**
	 * @brief The position of the constant in the constant pool.
	 *
	 * The index into the constant pool is used when encoding the constant
	 * into a file, as all references to constants are made using this value.
	 **/
	uint16_t	index;

	/**
	 * @brief Blank constructor for the ConstantInfo type.
	 *
	 * Creates a empty instance of ConstantInfo, with all fields initialized
	 * to 0.
	 **/
	ConstantInfo()
		: tag(0), index(0) {
	}

	/**
	 * @brief Tagged constructor for the ConstantInfo type.
	 *
	 * Creates an instance of ConstantInfo, populating its tag field with the
	 * value given to this function. The constant pool index is initialized
	 * to 0.
	 *
	 * @param tag The value for the constant's identifier tag.
	 **/
	ConstantInfo(uint8_t tag)
		: tag(tag), index(0) {
	}

	/**
	 * @brief Virtual desctructor for the ConstantInfo type.
	 **/
	virtual
	~ConstantInfo() {
	}

	/**
	 * @brief Used to determine is this is a 'long' constant.
	 *
	 * This function is used to determine whether a constant is defined as a
	 * long constant. Long constants take up two indexes within the constant
	 * pool, the second of which is always NULL.
	 * By default, only LongConstantInfo and DoubleConstantInfo have this
	 * type of behavior.
	 *
	 * @return true if this is a 'long' constant.
	 **/
	virtual
	bool IsLongConstant() {
		return false;
	}

	/**
	 * @brief Used to decode constant information from a ClassBuffer.
	 *
	 * @param buffer The ClassBuffer to be read.
	 **/
	virtual
	ConstantInfo *DecodeConstant(ClassBuffer *buffer) = 0;

	/**
	 * @brief Used to encode constant information into a ClassBuilder.
	 *
	 * @param builder The ClassBuilder to be written to.
	 **/
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

	~ConstantUtf8Info();

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

/* Constant Producers */

typedef ConstantInfo *(* ConstantProducer)(uint8_t tag, ClassBuffer *);

void RegisterProducer(uint8_t tag, ConstantProducer);

/* Encode and Decode */

/**
 * @brief Reads and creates a constant from a ClassBuffer.
 *
 * Decoder helper function. Creates a ConstantInfo object by reading
 * from a ClassBuffer, identifying and creating the necessary object
 * type, if able.
 *
 * @param buffer The ClassBuffer to be read from.
 * @return The newly read constant.
 **/
ConstantInfo *DecodeConstant(ClassBuffer *buffer);

/**
 * @brief Writes a constant info a ClassBuilder.
 *
 * Encoder helper function. Encodes and writes a ConstantInfo object
 * into a ClassBuilder.
 *
 * @param builder The ClassBuilder to be written to.
 * @param constant The ConstantInfo object to encode.
 **/
void EncodeConstant(ClassBuilder *builder, ConstantInfo *constant);

} /* JBC */

# endif /* ConstantInfo.h */
