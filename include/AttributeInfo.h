# ifndef __ATTRIBUTEINFO_H__
# define __ATTRIBUTEINFO_H__

# include "List.h"
# include <vector>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

# include "ConstantInfo.h"
# include "StackMapFrame.h"

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

struct AttributeInfo {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	AttributeInfo() {
	}

	AttributeInfo(ConstantUtf8Info *name, uint32_t length)
		: name(name), attribute_length(length) {
	}

	virtual
	~AttributeInfo() {
	}

	virtual
	AttributeInfo *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) = 0;

	virtual
	AttributeInfo *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) = 0;
};

struct ConstantValueAttribute
		: public AttributeInfo {
	// Constant Value
	ConstantInfo *constant_value;

	ConstantValueAttribute() {
	}

	ConstantValueAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	ConstantValueAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	ConstantValueAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct ExceptionTableEntry {
	uint16_t	start_pc;
	uint16_t	end_pc;
	uint16_t	handler_pc;
	uint16_t	catch_type;

	ExceptionTableEntry() {
	}
};

struct CodeAttribute
		: public AttributeInfo {
	// Maximums
	uint16_t	max_stack;
	uint16_t	max_locals;

	// Code
	uint32_t	code_length;
	uint8_t		*code;

	// Exception Table
	std::vector<ExceptionTableEntry *> exception_table;

	// Attribute Table
	std::vector<AttributeInfo *> attributes;

	CodeAttribute() {
	}

	CodeAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~CodeAttribute();

	CodeAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	CodeAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapTableAttribute
		: public AttributeInfo {
	// Stack Frame Map Entries
	std::vector<StackMapFrame *> entries;

	StackMapTableAttribute() {
	}

	StackMapTableAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~StackMapTableAttribute();

	StackMapTableAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	StackMapTableAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct ExceptionsAttribute
		: public AttributeInfo {
	// Exception Table
	List		*exception_table;

	ExceptionsAttribute() {
	}

	ExceptionsAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~ExceptionsAttribute();

	ExceptionsAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	ExceptionsAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

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

struct InnerClassEntry {
	// Inner Class Info
	ConstantClassInfo *inner_class_info;

	// Outer Class Info
	ConstantClassInfo *outer_class_info;

	// Inner Class Name
	ConstantUtf8Info *inner_class_name;

	// Inner Class Flags
	uint16_t	inner_class_access_flags;

	InnerClassEntry() {
	}
};

struct InnerClassesAttribute
		: public AttributeInfo {
	// Inner Classes
	List		*classes;

	InnerClassesAttribute() {
	}

	InnerClassesAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~InnerClassesAttribute();

	InnerClassesAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	InnerClassesAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct EnclosingMethodAttribute
		: public AttributeInfo {
	// Enclosing Class
	ConstantClassInfo *enclosing_class;

	// Enclosing Method
	ConstantNameAndTypeInfo *enclosing_method;

	EnclosingMethodAttribute() {
	}

	EnclosingMethodAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	EnclosingMethodAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	EnclosingMethodAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct SyntheticAttribute
		: public AttributeInfo {

	SyntheticAttribute() {
	}

	SyntheticAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	inline
	SyntheticAttribute *DecodeAttribute(ClassBuffer *, ClassFile *) {
		return this;
	}

	inline
	SyntheticAttribute *EncodeAttribute(ClassBuilder *, ClassFile *) {
		return this;
	}
};

struct SignatureAttribute
		: public AttributeInfo {
	// Signature
	ConstantUtf8Info *signature;

	SignatureAttribute() {
	}

	SignatureAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	SignatureAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SignatureAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct SourceFileAttribute
		: public AttributeInfo {
	// Source File
	ConstantUtf8Info *source_file;

	SourceFileAttribute() {
	}

	SourceFileAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	SourceFileAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SourceFileAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct SourceDebugExtensionAttribute
		: public AttributeInfo {
	// Debug Extension
	uint8_t		*debug_extension;

	SourceDebugExtensionAttribute() {
	}

	SourceDebugExtensionAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~SourceDebugExtensionAttribute();

	SourceDebugExtensionAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SourceDebugExtensionAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct LineNumberTableEntry {
	uint16_t	start_pc;
	uint16_t	line_number;

	LineNumberTableEntry() {
	}
};

struct LineNumberTableAttribute
		: public AttributeInfo {
	// Line Number Table
	List		*line_number_table;

	LineNumberTableAttribute() {
	}

	LineNumberTableAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~LineNumberTableAttribute();

	LineNumberTableAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	LineNumberTableAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct LocalVariableTableEntry {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	ConstantUtf8Info *name;

	// Descriptor
	ConstantUtf8Info *descriptor;

	uint16_t	index;

	LocalVariableTableEntry() {
	}
};

struct LocalVariableTableAttribute
		: public AttributeInfo {
	// Local Variable Table
	List		*local_variable_table;

	LocalVariableTableAttribute() {
	}

	LocalVariableTableAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~LocalVariableTableAttribute();

	LocalVariableTableAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	LocalVariableTableAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct LocalVariableTypeTableEntry {
	uint16_t	start_pc;
	uint16_t	length;

	// Variable Name
	ConstantUtf8Info *name;

	// Signature
	ConstantUtf8Info *signature;

	uint16_t	index;

	LocalVariableTypeTableEntry() {
	}
};

struct LocalVariableTypeTableAttribute
		: public AttributeInfo {
	// Local Variable Type Table
	List		*local_variable_type_table;

	LocalVariableTypeTableAttribute() {
	}

	LocalVariableTypeTableAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~LocalVariableTypeTableAttribute();

	LocalVariableTypeTableAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	LocalVariableTypeTableAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct DeprecatedAttribute
		: public AttributeInfo {

	DeprecatedAttribute() {
	}

	DeprecatedAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	inline
	DeprecatedAttribute *DecodeAttribute(ClassBuffer *, ClassFile *) {
		return this;
	}

	inline
	DeprecatedAttribute *EncodeAttribute(ClassBuilder *, ClassFile *) {
		return this;
	}
};

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

struct RuntimeAnnotationsAttribute
		: public AttributeInfo {
	// Annotations Table
	List		*annotations;

	RuntimeAnnotationsAttribute() {
	}

	RuntimeAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~RuntimeAnnotationsAttribute();

	RuntimeAnnotationsAttribute *DecodeAttribute(
			ClassBuffer *buffer, ClassFile *classFile);

	RuntimeAnnotationsAttribute *EncodeAttribute(
			ClassBuilder *builder, ClassFile *classFile);
};

struct RuntimeVisibleAnnotationsAttribute
		: public RuntimeAnnotationsAttribute {

	RuntimeVisibleAnnotationsAttribute() {
	}

	RuntimeVisibleAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: RuntimeAnnotationsAttribute(name, length) {
	}
};

struct RuntimeInvisibleAnnotationsAttribute
		: public RuntimeAnnotationsAttribute {

	RuntimeInvisibleAnnotationsAttribute() {
	}

	RuntimeInvisibleAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: RuntimeAnnotationsAttribute(name, length) {
	}
};

struct ParameterAnnotationsEntry {
	// Annotations Table
	List		*annotations;

	~ParameterAnnotationsEntry();
};

struct RuntimeParameterAnnotationsAttribute
		: public AttributeInfo {
	// Parameters Table
	List		*parameter_annotations;

	RuntimeParameterAnnotationsAttribute() {
	}

	RuntimeParameterAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~RuntimeParameterAnnotationsAttribute();

	RuntimeParameterAnnotationsAttribute *DecodeAttribute(
			ClassBuffer *buffer, ClassFile *classFile);

	RuntimeParameterAnnotationsAttribute *EncodeAttribute(
			ClassBuilder *builder, ClassFile *classFile);
};

struct RuntimeVisibleParameterAnnotationsAttribute
		: public RuntimeParameterAnnotationsAttribute {

	RuntimeVisibleParameterAnnotationsAttribute() {
	}

	RuntimeVisibleParameterAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: RuntimeParameterAnnotationsAttribute(name, length) {
	}
};

struct RuntimeInvisibleParameterAnnotationsAttribute
		: public RuntimeParameterAnnotationsAttribute {

	RuntimeInvisibleParameterAnnotationsAttribute() {
	}

	RuntimeInvisibleParameterAnnotationsAttribute(ConstantUtf8Info *name, uint32_t length)
		: RuntimeParameterAnnotationsAttribute(name, length) {
	}
};

struct AnnotationDefaultAttribute
		: public AttributeInfo {
	// Value
	ElementValue *default_value;

	AnnotationDefaultAttribute() {
	}

	AnnotationDefaultAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~AnnotationDefaultAttribute();

	AnnotationDefaultAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	AnnotationDefaultAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct BootstrapMethodEntry {
	// Bootstrap Method Reference
	ConstantMethodHandleInfo *bootstrap_method_ref;

	// Bootstrap Argument Table
	List		*bootstrap_arguments;

	~BootstrapMethodEntry();
};

struct BootstrapMethodsAttribute
		: public AttributeInfo {
	// Bootstrap Method Table
	std::vector<BootstrapMethodEntry *> bootstrap_methods;

	BootstrapMethodsAttribute() {
	}

	BootstrapMethodsAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~BootstrapMethodsAttribute();

	BootstrapMethodsAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	BootstrapMethodsAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info);

AttributeInfo *decodeAttribute(ClassFile *classFile, ClassBuffer *buffer);

# endif /* AttributeInfo.h */
