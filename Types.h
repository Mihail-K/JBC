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

/* Constant Info */

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
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;
} AttributeInfo;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Constant Value
	uint16_t	constant_value_index;
	ConstantInfo *constant_value;
} ConstantValueAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	end_pc;
	uint16_t	handler_pc;
	uint16_t	catch_type;
} ExceptionTableEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Maximums
	uint16_t	max_stack;
	uint16_t	max_locals;

	// Code
	uint32_t	code_length;
	uint8_t		*code;

	// Exception Table
	uint16_t	exception_table_length;
	ExceptionTableEntry **exception_table;

	// Attribute Table
	uint16_t	attributes_count;
	AttributeInfo **attributes;
} CodeAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Stack Frame Map Entries
	uint16_t	number_of_entries;
	void		**entries;
} StackMapTableAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Exception Table
	uint16_t	number_of_exceptions;
	uint16_t	*exception_index_table;
	ConstantClassInfo **exception_table;
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
	uint16_t	inner_class_info_index;
	ConstantClassInfo *inner_class_info;

	// Outer Class Info
	uint16_t	outer_class_info_index;
	ConstantClassInfo *outer_class_info;

	// Inner Class Name
	uint16_t	inner_class_name_index;
	ConstantUtf8Info *inner_class_name;

	// Inner Class Flags
	uint16_t	inner_class_access_flags;
} InnerClassEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Inner Classes
	uint16_t	number_of_classes;
	InnerClassEntry **classes;
} InnerClassesAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Enclosing Class
	uint16_t	enclosing_class_index;
	ConstantClassInfo *enclosing_class;

	// Enclosing Method
	uint16_t	enclosing_method_index;
	ConstantNameAndTypeInfo *enclosing_method;
} EnclosingMethodAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;
} SyntheticAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Signature
	uint16_t	signature_index;
	ConstantUtf8Info *signature;
} SignatureAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Source File
	uint16_t	source_file_index;
	ConstantUtf8Info *source_file;
} SourceFileAttribute;

typedef struct {
	uint16_t	name_index;
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
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Line Number Table
	uint16_t	line_number_table_length;
	LineNumberTableEntry **line_number_table;
} LineNumberTableAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	uint16_t	name_index;
	ConstantUtf8Info *name;

	// Descriptor
	uint16_t	descriptor_index;
	ConstantUtf8Info *descriptor;

	uint16_t	index;
} LocalVariableTableEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Local Variable Table
	uint16_t	local_variable_table_length;
	LocalVariableTableEntry **local_variable_table;
} LocalVariableTableAttribute;

typedef struct {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	uint16_t	name_index;
	ConstantUtf8Info *name;

	// Signature
	uint16_t	signature_index;
	ConstantUtf8Info *signature;

	uint16_t	index;
} LocalVariableTypeTableEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Local Variable Type Table
	uint16_t	local_variable_type_table_length;
	LocalVariableTypeTableEntry **local_variable_type_table;
} LocalVariableTypeTableAttribute;

typedef struct {
	uint16_t	name_index;
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
		struct {
			// Constant Value
			uint16_t	constant_value_index;
			ConstantInfo *constant_value;
		} constant_value;

		// Enum Constant Value
		struct {
			// Type Name
			uint16_t	type_name_index;
			ConstantUtf8Info *type_name;

			// Constant Name
			uint16_t	const_name_index;
			ConstantUtf8Info *const_name;
		} enum_const_value;

		// Class Info Value
		struct {
			// Class Info
			uint16_t	class_info_index;
			ConstantClassInfo *class_info;
		} class_info_value;

		// Annotation Value
		AnnotationEntry *annotation_value;

		// Array Value
		struct {
			uint16_t	num_values;
			ElementValue **values;
		} array_value;
	} value;
};

typedef struct {
	// Name
	uint16_t	element_name_index;
	ConstantUtf8Info *element_name;

	// Element Value
	ElementValue *value;
} ElementValuePairsEntry;

struct sAnnotationEntry {
	// Type
	uint16_t	type_index;
	ConstantUtf8Info *type;

	// Element-Value Pairs Table
	uint16_t	num_element_value_pairs;
	ElementValuePairsEntry **element_value_pairs;
};

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Annotations Table
	uint16_t	num_annotations;
	AnnotationEntry **annotations;
}
RuntimeAnnotationsAttribute,
RuntimeVisibleAnnotationsAttribute,
RuntimeInvisibleAnnotationsAttribute;

typedef struct {
	// Annotations Table
	uint16_t	num_annotations;
	AnnotationEntry **annotations;
} ParameterAnnotationsEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Parameters Table
	uint8_t		num_parameters;
	ParameterAnnotationsEntry **parameter_annotations;
}
RuntimeVisibleParameterAnnotationsAttribute,
RuntimeInvisibleParameterAnnotationsAttribute;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Value
	ElementValue *default_value;
} AnnotationDefaultAttribute;

typedef struct {
	// Bootstrap Method Reference
	uint16_t	bootstrap_method_ref_index;
	ConstantMethodHandleInfo *bootstrap_method_ref;

	// Bootstrap Argument Table
	uint16_t	num_arguments;
	uint16_t	*bootstrap_argument_indexes;
	ConstantInfo **bootstrap_arguments;
} BootstrapMethodEntry;

typedef struct {
	uint16_t	name_index;
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	// Bootstrap Method Table
	uint16_t	num_bootstrap_methods;
	BootstrapMethodEntry **bootstrap_methods;
} BootstrapMethodsAttribute;

/* Field/Method Info */

typedef enum {
	MT_FIELD,
	MT_METHOD
} MemberType;

typedef struct {
	// Member Type
	// [Not present in classfile]
	uint8_t		member_type;

	uint16_t	access_flags;

	// Name
	uint16_t	name_index;
	ConstantUtf8Info *name;

	// Descriptor
	uint16_t	descriptor_index;
	ConstantUtf8Info *descriptor;

	// Attributes Table
	uint16_t	attributes_count;
	AttributeInfo **attributes;
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
	uint16_t	constant_pool_count;
	ConstantInfo **constant_pool;

	uint16_t	access_flags;

	// This Class
	uint16_t	this_class_index;
	ConstantClassInfo *this_class;

	// Super Class
	uint16_t	super_class_index;
	ConstantClassInfo *super_class;

	// Interfaces Table
	uint16_t	interfaces_count;
	uint16_t	*interface_indexes;
	ConstantClassInfo **interfaces;

	// Fields Table
	uint16_t	fields_count;
	FieldInfo	**fields;

	// Methods Table
	uint16_t	methods_count;
	MethodInfo	**methods;
} ClassFile;

# endif /* Types.h */
