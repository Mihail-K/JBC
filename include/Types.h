# ifndef __TYPES_H__
# define __TYPES_H__

# include <vector>
# include <stdint.h>

# include "List.h"

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

/* Constant Info */

typedef struct {
	uint8_t		tag;
	uint16_t	index;
} ConstantInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	name_index;
} ConstantClassInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	class_index;
	uint16_t	name_and_type_index;
}
ConstantRefInfo,
ConstantFieldRefInfo,
ConstantMethodRefInfo,
ConstantInterfaceMethodRefInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	string_index;
} ConstantStringInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint32_t	bytes;
}
ConstantIntegerInfo,
ConstantFloatInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint32_t	high_bytes;
	uint32_t	low_bytes;
}
ConstantLongInfo,
ConstantDoubleInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	name_index;
	uint16_t	descriptor_index;
} ConstantNameAndTypeInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	length;
	uint8_t		*bytes;
} ConstantUtf8Info;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint8_t		reference_kind;
	uint16_t	reference_index;
} ConstantMethodHandleInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	descriptor_index;
} ConstantMethodTypeInfo;

typedef struct {
	uint8_t		tag;
	uint16_t	index;
	uint16_t	bootstrap_method_attr_index;
	uint16_t	name_and_type_index;
} ConstantInvokeDynamicInfo;

/* Access Flags */

enum FieldAccessFlags {
	ACC_PUBLIC		= 0x0001,
	ACC_PRIVATE		= 0x0002,
	ACC_PROTECTED	= 0x0004,
	ACC_STATIC		= 0x0008,
	ACC_FINAL		= 0x0010,
	ACC_VOLATILE	= 0x0040,
	ACC_TRANSIENT	= 0x0080,
	ACC_SYNTHETIC	= 0x1000,
	ACC_ENUM		= 0x4000
};

enum MethodAccessFlags {
//	ACC_PUBLIC		= 0x0001,
//	ACC_PRIVATE		= 0x0002,
//	ACC_PROTECTED	= 0x0004,
//	ACC_STATIC		= 0x0008,
//	ACC_FINAL		= 0x0010,
	ACC_SYNCHRONIZED= 0x0020,
	ACC_BRIDGE		= 0x0040,
	ACC_VARARGS		= 0x0080,
	ACC_NATIVE		= 0x0100,
	ACC_ABSTRACT	= 0x0400,
	ACC_STRICT		= 0x0800,
//	ACC_SYNTHETIC	= 0x1000
};

/* Attribute Info */

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;
} AttributeInfo;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Constant Value
	ConstantInfo *constant_value;
} ConstantValueAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	end_pc;
	uint16_t	handler_pc;
	uint16_t	catch_type;
} ExceptionTableEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Maximums
	uint16_t	max_stack;
	uint16_t	max_locals;

	// Code
	uint32_t	code_length;
	uint8_t		*code;


	// Exception Table
	List		*exception_table;

	// Attribute Table
	List		*attributes;
} CodeAttribute;

typedef union {
	uint8_t		tag;
	struct {
		uint8_t		tag;
	} 
	top_variable_info,
	integer_variable_info,
	float_variable_info,
	long_variable_info,
	double_variable_info,
	null_variable_info,
	uninitialized_this_variable_info;
	struct {
		uint8_t		tag;
		ConstantClassInfo *object;
	} object_variable_info;
	struct {
		uint8_t		tag;
		uint16_t	offset;
	} uninitialized_variable_info;
} VerificationTypeInfo;

typedef struct {
	uint8_t		tag;
} StackMapFrame;

typedef struct {
	uint8_t		tag;
	uint16_t	offset_delta;
} StackMapOffFrame;

typedef struct {
	uint8_t		tag;

	// Stack Items
	VerificationTypeInfo *stack;
} StackMapItemFrame;

typedef struct {
	uint8_t		tag;
	uint16_t	offset_delta;

	// Stack Items
	VerificationTypeInfo *stack;
} StackMapExtFrame;

typedef struct {
	uint8_t		tag;
	uint16_t	offset_delta;

	// Stack Item Table
	VerificationTypeInfo **stack;
} StackMapListFrame;

typedef struct {
	uint8_t		tag;
	uint16_t	offset_delta;

	// Local Items
	List		*locals;

	// Stack Items
	List		*stack;
} StackMapFullFrame;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Stack Frame Map Entries
	List		*entries;
} StackMapTableAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Exception Table
	List		*exception_table;
} ExceptionsAttribute;

typedef enum {
//	ACC_PUBLIC		= 0x0001,
//	ACC_PRIVATE		= 0x0002,
//	ACC_PROTECTED	= 0x0004,
//	ACC_STATIC		= 0x0008,
//	ACC_FINAL		= 0x0010,
	ACC_INTERFACE	= 0x0200,
//	ACC_ABSTRACT	= 0x0400,
//	ACC_SYNTHETIC	= 0x1000,
	ACC_ANNOTATION	= 0x2000,
//	ACC_ENUM		= 0x4000
} InnerClassAccessFlags;

typedef struct {
	// Inner Class Info
	ConstantClassInfo *inner_class_info;

	// Outer Class Info
	ConstantClassInfo *outer_class_info;

	// Inner Class Name
	ConstantUtf8Info *inner_class_name;

	// Inner Class Flags
	uint16_t	inner_class_access_flags;
} InnerClassEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Inner Classes
	List		*classes;
} InnerClassesAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Enclosing Class
	ConstantClassInfo *enclosing_class;

	// Enclosing Method
	ConstantNameAndTypeInfo *enclosing_method;
} EnclosingMethodAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;
} SyntheticAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Signature
	ConstantUtf8Info *signature;
} SignatureAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Source File
	ConstantUtf8Info *source_file;
} SourceFileAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Debug Extension
	uint8_t		*debug_extension;
} SourceDebugExtensionAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	line_number;
} LineNumberTableEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Line Number Table
	List		*line_number_table;
} LineNumberTableAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	ConstantUtf8Info *name;

	// Descriptor
	ConstantUtf8Info *descriptor;

	uint16_t	index;
} LocalVariableTableEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Local Variable Table
	List		*local_variable_table;
} LocalVariableTableAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	ConstantUtf8Info *name;

	// Signature
	ConstantUtf8Info *signature;

	uint16_t	index;
} LocalVariableTypeTableEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Local Variable Type Table
	List		*local_variable_type_table;
} LocalVariableTypeTableAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;
} DeprecatedAttribute;

/* Declare types ahead of time */
typedef struct sElementValue ElementValue;
typedef struct sAnnotationEntry AnnotationEntry;

struct sElementValue {
	uint8_t		tag;
	union {
		// Constant Value
		ConstantInfo *const_value;

		// Enum Constant Value
		struct {
			// Type Name
			ConstantUtf8Info *type_name;

			// Constant Name
			ConstantUtf8Info *const_name;
		} enum_const_value;

		// Class Info Value
		ConstantClassInfo *class_info;

		// Annotation Value
		AnnotationEntry *annotation_value;

		// Array Value
		List		*array_values;
	} value;
};

typedef struct {
	// Name
	ConstantUtf8Info *element_name;

	// Element Value
	ElementValue *value;
} ElementValuePairsEntry;

struct sAnnotationEntry {
	// Type
	ConstantUtf8Info *type;

	// Element-Value Pairs Table
	List		*element_value_pairs;
};

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Annotations Table
	List		*annotations;
}
RuntimeAnnotationsAttribute,
RuntimeVisibleAnnotationsAttribute,
RuntimeInvisibleAnnotationsAttribute;

typedef struct {
	// Annotations Table
	List		*annotations;
} ParameterAnnotationsEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Parameters Table
	List		*parameter_annotations;
}
RuntimeParameterAnnotationsAttribute,
RuntimeVisibleParameterAnnotationsAttribute,
RuntimeInvisibleParameterAnnotationsAttribute;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Value
	ElementValue *default_value;
} AnnotationDefaultAttribute;

typedef struct {
	// Bootstrap Method Reference
	ConstantMethodHandleInfo *bootstrap_method_ref;

	// Bootstrap Argument Table
	List		*bootstrap_arguments;
} BootstrapMethodEntry;

typedef struct {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Bootstrap Method Table
	List		*bootstrap_methods;
} BootstrapMethodsAttribute;

/* Field/Method Info */

typedef enum {
	MT_FIELD,
	MT_METHOD
} MemberType;

typedef struct {
	uint16_t	access_flags;

	// Name
	ConstantUtf8Info *name;

	// Descriptor
	ConstantUtf8Info *descriptor;

	// Attributes Table
	List		*attributes;
}
FieldInfo,
MethodInfo,
MemberInfo;

typedef struct {
	uint32_t	magic;

	// Version Info
	uint16_t	major_version;
	uint16_t	minor_version;

	// Constants Table
	std::vector<ConstantInfo *> constant_pool;

	uint16_t	access_flags;

	// This Class
	ConstantClassInfo *this_class;

	// Super Class
	ConstantClassInfo *super_class;

	// Interfaces Table
	std::vector<ConstantClassInfo *> interfaces;

	// Fields Table
	std::vector<FieldInfo *> fields;

	// Methods Table
	std::vector<MethodInfo *> methods;

	// Attributes Table
	std::vector<AttributeInfo *> attributes;
} ClassFile;

# endif /* Types.h */
