/**
 * @file ConstantInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.42
 *
 * @brief Defines all standard ConstantInfo types.
 **/
# ifndef __CONSTANTINFO_H__
# define __CONSTANTINFO_H__

# include <string>
# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

/**
 * @addtogroup JBC
 * @{
 **/
 
/**
 * @brief The JBC namespace of types and functions.
 **/
namespace JBC {

/**
 * @enum ConstantType
 * @brief An enumeration of the constant types found in Java class files.
 *
 * These values are used to determine which type of ConstantInfo object should
 * be produces when decoding, and are written during encoding.
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
	 * Children should use this constructor to identify their constant type
	 * upon instantiation.
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
	 * pool, the second of which is always @c NULL.
	 * By default, only LongConstantInfo and DoubleConstantInfo have this
	 * type of behavior.
	 *
	 * @return @c true if this is a 'long' constant.
	 **/
	virtual
	bool IsLongConstant() {
		return false;
	}

	/**
	 * @brief Used to decode constant information from a ClassBuffer.
	 *
	 * @param buffer The ClassBuffer to be read.
	 * @return This instance of ConstantInfo.
	 **/
	virtual
	ConstantInfo *DecodeConstant(ClassBuffer *buffer) = 0;

	/**
	 * @brief Used to encode constant information into a ClassBuilder.
	 *
	 * @param builder The ClassBuilder to be written to.
	 * @return This instance of ConstantInfo.
	 **/
	virtual
	ConstantInfo *EncodeConstant(ClassBuilder *buider) = 0;
};

/**
 * @struct ConstantClassInfo
 * @brief A ConstantInfo type for Java class constants.
 **/
struct ConstantClassInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the class name.
	 *
	 * References a ConstantUtf8Info object.
	 **/
	uint16_t	name_index;

	/**
	 * @brief Constructor for ConstantClassInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_CLASS.
	 **/
	ConstantClassInfo()
		: ConstantInfo(CONSTANT_CLASS) {
	}

	ConstantClassInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantClassInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantFieldRefInfo
 * @brief A ConstantInfo type for Java field constants.
 **/
struct ConstantFieldRefInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the class that declares this field.
	 *
	 * References a ConstantClassInfo object.
	 **/
	uint16_t	class_index;
	/**
	 * @brief Type constant pool index of the name and type of this field.
	 *
	 * References a ConstantNameAndTypeInfo object.
	 **/
	uint16_t	name_and_type_index;

	/**
	 * @brief Constructor for ConstantFieldRefInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_FIELD_REF.
	 **/
	ConstantFieldRefInfo()
		: ConstantInfo(CONSTANT_FIELD_REF) {
	}

	ConstantFieldRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantFieldRefInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantMethodRefInfo
 * @brief A ConstantInfo type for Java method constants.
 **/
struct ConstantMethodRefInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the class that declares this method.
	 *
	 * References a ConstantClassInfo object.
	 **/
	uint16_t	class_index;
	/**
	 * @brief Type constant pool index of the name and type of this method.
	 *
	 * References a ConstantNameAndTypeInfo object.
	 **/
	uint16_t	name_and_type_index;

	/**
	 * @brief Constructor for ConstantMethodRefInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_METHOD_REF.
	 **/
	ConstantMethodRefInfo()
		: ConstantInfo(CONSTANT_METHOD_REF) {
	}

	ConstantMethodRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodRefInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantInterfaceMethodRefInfo
 * @brief A ConstantInfo type for Java interface method constants.
 **/
struct ConstantInterfaceMethodRefInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the interface that declares this method.
	 *
	 * References a ConstantClassInfo object.
	 **/
	uint16_t	class_index;
	/**
	 * @brief The constant pool index of the name and type of this method.
	 *
	 * References a ConstantNameAndTypeInfo object.
	 **/
	uint16_t	name_and_type_index;

	/**
	 * @brief Constructor for ConstantInterfaceMethodRefInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of ConstantType::CONSTANT_INTERFACE_METHOD_REF.
	 **/
	ConstantInterfaceMethodRefInfo()
		: ConstantInfo(CONSTANT_INTERFACE_METHOD_REF) {
	}

	ConstantInterfaceMethodRefInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantInterfaceMethodRefInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantStringInfo
 * @brief A ConstantInfo type for Java string constants.
 **/
struct ConstantStringInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the value of this string.
	 *
	 * References a ConstantUtf8Info object.
	 **/
	uint16_t	string_index;

	/**
	 * @brief Constructor for ConstantStringInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_STRING.
	 **/
	ConstantStringInfo()
		: ConstantInfo(CONSTANT_STRING) {
	}

	ConstantStringInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantStringInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantIntegerInfo
 * @brief A ConstantInfo type for 32-bit integer constants.
 **/
struct ConstantIntegerInfo
		: public ConstantInfo {
	/**
	 * @brief The integer value of the constant.
	 **/
	uint32_t bytes;

	/**
	 * @brief Constructor for ConstantIntegerInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_INTEGER.
	 **/
	ConstantIntegerInfo()
		: ConstantInfo(CONSTANT_INTEGER) {
	}

	ConstantIntegerInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantIntegerInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantFloatInfo
 * @brief A ConstantInfo type for 32-bit float constants.
 **/
struct ConstantFloatInfo
		: public ConstantInfo {
	/**
	 * @brief The value of the constant, stored in a 32-bit integer.
	 **/
	uint32_t bytes;

	/**
	 * @brief Constructor for ConstantFloatInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_FLOAT.
	 **/
	ConstantFloatInfo()
		: ConstantInfo(CONSTANT_FLOAT) {
	}

	ConstantFloatInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantFloatInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantLongInfo
 * @brief A ConstantInfo type for 64-bit integer constants.
 *
 * This type is a 'long' constant.
 * @see ConstantInfo::IsLongConstant()
 **/
struct ConstantLongInfo
		: public ConstantInfo {
	/**
	 * @brief The upper half of the value of the constant.
	 **/
	uint32_t	high_bytes;
	/**
	 * @brief The lower half of the value of the constant.
	 **/
	uint32_t	low_bytes;

	/**
	 * @brief Constructor for ConstantLongInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_LONG.
	 **/
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

/**
 * @struct ConstantDoubleInfo
 * @brief A ConstantInfo type for 64-bit float constants.
 *
 * This type is a 'long' constant.
 * @see ConstantInfo::IsLongConstant()
 **/
struct ConstantDoubleInfo
		: public ConstantInfo {
	/**
	 * @brief The upper half of the value of the constant, stored in an integer.
	 **/
	uint32_t	high_bytes;
	/**
	 * @brief The lower half of the value of the constant, stored in an integer.
	 **/
	uint32_t	low_bytes;

	/**
	 * @brief Constructor for ConstantDoubleInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_DOUBLE.
	 **/
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

/**
 * @struct ConstantNameAndTypeInfo
 * @brief A ConstantInfo type for Name and Type pairs.
 **/
struct ConstantNameAndTypeInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the name half of this constant.
	 *
	 * References a ConstantUtf8Info object.
	 **/
	uint16_t	name_index;
	/**
	 * @brief The constant pool index of the type half of this constant.
	 *
	 * References a ConstantUtf8Info object.
	 **/
	uint16_t	descriptor_index;

	/**
	 * @brief Constructor for ConstantNameAndTypeInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_NAME_AND_TYPE.
	 **/
	ConstantNameAndTypeInfo()
		: ConstantInfo(CONSTANT_NAME_AND_TYPE) {
	}

	ConstantNameAndTypeInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantNameAndTypeInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantUtf8Info
 * @brief A ConstantInfo type for UTF-8 encoded string values.
 **/
struct ConstantUtf8Info
		: public ConstantInfo {
	/**
	 * @brief The length of the string value, sans the null-terminator.
	 **/
	uint16_t	length;
	/**
	 * @brief The UTF-8 encoded string, in a null-terminated form.
	 *
	 * This field is null-terminated for convinience and simplicity. This value
	 * is stored without the null-terminator in its encoded form.
	 **/
	uint8_t		*bytes;

	/**
	 * @brief Constructor for ConstantUtf8Info.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_UTF8_INFO.
	 **/
	ConstantUtf8Info()
		: ConstantInfo(CONSTANT_UTF8) {
	}

	/**
	 * @brief Destructor for ConstantUtf8Info.
	 *
	 * Deletes the the UTF-8 encoded string from memory.
	 **/
	~ConstantUtf8Info();

	ConstantUtf8Info *DecodeConstant(ClassBuffer *buffer);

	ConstantUtf8Info *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantMethodHandleInfo
 * @brief A ConstantInfo type representing a Java method handle.
 **/
struct ConstantMethodHandleInfo
		: public ConstantInfo {
	/**
	 * @brief Denotes the type of method handle, which controls bytecode behaviour.
	 **/
	uint8_t		reference_kind;
	/**
	 * @brief The constant pool index of the reference info constant for this handle.
	 *
	 * References one of the following ConstantInfo types:
	 *	- ConstantFieldRefInfo
	 *	- ConstantMethodRefInfo
	 *	- ConstantInterfaceMethodRefInfo
	 **/
	uint16_t	reference_index;

	/**
	 * @brief Constructor for ConstantMethodHandleInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_METHOD_HANDLE.
	 **/
	ConstantMethodHandleInfo()
		: ConstantInfo(CONSTANT_METHOD_HANDLE) {
	}

	ConstantMethodHandleInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodHandleInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantMethodTypeInfo
 * @brief A ConstantInfo type representing a Java method type.
 **/
struct ConstantMethodTypeInfo
		: public ConstantInfo {
	/**
	 * @brief The constant pool index of the method type descriptor.
	 *
	 * References a ConstantUtf8Info object.
	 **/
	uint16_t	descriptor_index;

	/**
	 * @brief Constructor for ConstantMethodTypeInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_METHOD_TYPE.
	 **/
	ConstantMethodTypeInfo()
		: ConstantInfo(CONSTANT_METHOD_TYPE) {
	}

	ConstantMethodTypeInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantMethodTypeInfo *EncodeConstant(ClassBuilder *builder);
};

/**
 * @struct ConstantInvokeDynamicInfo
 * @brief A ConstantInfo type denoting a dynamic invocation name.
 **/
struct ConstantInvokeDynamicInfo
		: public ConstantInfo {
	/**
	 * @brief The index of the bootstrap method in this class's bootstrap method table.
	 *
	 * @see BootstrapMethodsAttribute
	 **/
	uint16_t	bootstrap_method_attr_index;
	/**
	 * @brief Type constant pool index of the name and type of this bootstrap method.
	 *
	 * References a ConstantNameAndTypeInfo object.
	 **/
	uint16_t	name_and_type_index;

	/**
	 * @brief Constructor for ConstantInvokeDynamicInfo.
	 *
	 * Initializes the inheritted tag field to the enumerated value
	 * of CONSTANT_INVOKE_DYNAMIC.
	 **/
	ConstantInvokeDynamicInfo()
		: ConstantInfo(CONSTANT_INVOKE_DYNAMIC) {
	}

	ConstantInvokeDynamicInfo *DecodeConstant(ClassBuffer *buffer);

	ConstantInvokeDynamicInfo *EncodeConstant(ClassBuilder *builder);
};

/* Constant Producers */

/**
 * @brief A ConstantInfo producer function type.
 *
 * A function type definiton for ConstantInfo producers, registered to
 * decode ConstantInfo object with non-standard types.
 *
 * @see RegisterProducer(uint8_t, ConstantProducer)
 **/
typedef ConstantInfo *(* ConstantProducer)(uint8_t tag, ClassBuffer *);

/**
 * @brief Hooks a ConstantProducer to the Decoder,
 *			for reading nonstandard Constant types.
 *
 * ConstantProducers registered with this fuction can be unhooked by
 * registering @c NULL to the same tag index.
 *
 * @code{.cpp}
 *	ConstantInfo *MyConstantProducer(uint8_t, ClassBuffer *buffer) {
 *		// Decode my custom ConstantInfo type.
 *		return (new MyConstantInfo)->DecodeBuffer(buffer);
 *	}
 *	. . .
 *	void RegisterMyProducers() {
 *		// Register my new producer with the Decoder.
 *		RegisterProducer(MyConstantTag, MyConstantProducer);
 *	}
 * @endcode
 *
 * @param tag The tag index to register the ConstantProducer with.
 * @param producer The producer being hooked to the Decoder.
 * @see ConstantProducer
 **/
void RegisterProducer(uint8_t tag, ConstantProducer producer);

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

/**
 * }@
 **/

# endif /* ConstantInfo.h */
