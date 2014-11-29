/**
 * @file AttributeInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.36
 *
 * @brief Defines all standard AttributeInfo types.
 **/
# ifndef __ATTRIBUTEINFO_H__
# define __ATTRIBUTEINFO_H__

# include <string>
# include <vector>

# include "ClassBuffer.h"
# include "ClassBuilder.h"
# include "ConstantInfo.h"

# include "ElementValue.h"
# include "StackMapFrame.h"

namespace JBC {

/* Attribute Info */

struct AttributeInfo {
	ConstantUtf8Info *name;
	uint32_t	attribute_length;

	AttributeInfo()
		: name(NULL), attribute_length(0) {
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

	ConstantValueAttribute()
		: constant_value(NULL) {
	}

	ConstantValueAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length),
		  constant_value(NULL) {
	}

	ConstantValueAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	ConstantValueAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct ExceptionTableEntry {
	uint16_t	start_pc;
	uint16_t	end_pc;
	uint16_t	handler_pc;
	uint16_t	catch_type;

	ExceptionTableEntry()
		: start_pc(0), end_pc(0),
		  handler_pc(0), catch_type(0) {
	}

	ExceptionTableEntry *DecodeEntry(ClassBuffer *buffer);

	ExceptionTableEntry *EncodeEntry(ClassBuilder *builder);
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

	CodeAttribute()
		: max_stack(0), max_locals(0),
		  code_length(0), code(NULL) {
	}

	CodeAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length),
		  max_stack(0), max_locals(0),
		  code_length(0), code(NULL) {
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
	std::vector<ConstantInfo *> exception_table;

	ExceptionsAttribute() {
	}

	ExceptionsAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length) {
	}

	~ExceptionsAttribute();

	ExceptionsAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	ExceptionsAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

enum InnerClassFlags {
	INNER_CLASS_PUBLIC		= 0x0001,
	INNER_CLASS_PRIVATE		= 0x0002,
	INNER_CLASS_PROTECTED	= 0x0004,
	INNER_CLASS_STATIC		= 0x0008,
	INNER_CLASS_FINAL		= 0x0010,
	INNER_CLASS_INTERFACE	= 0x0200,
	INNER_CLASS_ABSTRACT	= 0x0400,
	INNER_CLASS_SYNTHETIC	= 0x1000,
	INNER_CLASS_ANNOTATION	= 0x2000,
	INNER_CLASS_ENUM		= 0x4000
};

struct InnerClassEntry {
	// Inner Class Info
	ConstantClassInfo *inner_class_info;

	// Outer Class Info
	ConstantClassInfo *outer_class_info;

	// Inner Class Name
	ConstantUtf8Info *inner_class_name;

	// Inner Class Flags
	uint16_t	inner_class_access_flags;

	InnerClassEntry()
		: inner_class_info(NULL), outer_class_info(NULL),
		  inner_class_name(NULL), inner_class_access_flags(0) {
	}

	/**
	 * @brief Returns a reference to this inner class's flags.
	 **/
	inline
	uint16_t &Flags() {
		return inner_class_access_flags;
	}

	/**
	 * @breif Returns the state of a single flag.
	 *
	 * @param flag The flag to check for.
	 **/
	bool GetFlag(InnerClassFlags flag) {
		return !!(inner_class_access_flags & flag);
	}

	/**
	 * @brief Sets the state of a single flag.
	 *
	 * @param flag The flag to be modified.
	 * @param state The new state for the flag.
	 **/
	void SetFlag(InnerClassFlags flag, bool state) {
		if(state) inner_class_access_flags |= flag;
		else inner_class_access_flags &= ~flag;
	}

	InnerClassEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	InnerClassEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct InnerClassesAttribute
		: public AttributeInfo {
	// Inner Classes
	std::vector<InnerClassEntry *> classes;

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

	EnclosingMethodAttribute()
		: enclosing_class(NULL), enclosing_method(NULL) {
	}

	EnclosingMethodAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length), enclosing_class(NULL),
		  enclosing_method(NULL) {
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

	SignatureAttribute()
		: signature(NULL) {
	}

	SignatureAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length), signature(NULL) {
	}

	SignatureAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SignatureAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct SourceFileAttribute
		: public AttributeInfo {
	// Source File
	ConstantUtf8Info *source_file;

	SourceFileAttribute()
		: source_file(NULL) {
	}

	SourceFileAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length), source_file(NULL) {
	}

	SourceFileAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SourceFileAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct SourceDebugExtensionAttribute
		: public AttributeInfo {
	// Debug Extension
	uint8_t		*debug_extension;

	SourceDebugExtensionAttribute()
		: debug_extension(NULL) {
	}

	SourceDebugExtensionAttribute(ConstantUtf8Info *name, uint32_t length)
		: AttributeInfo(name, length), debug_extension(NULL) {
	}

	~SourceDebugExtensionAttribute();

	SourceDebugExtensionAttribute *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

	SourceDebugExtensionAttribute *EncodeAttribute(ClassBuilder *builder, ClassFile *classFile);
};

struct LineNumberTableEntry {
	uint16_t	start_pc;
	uint16_t	line_number;

	LineNumberTableEntry()
		: start_pc(0), line_number(0) {
	}

	LineNumberTableEntry *DecodeEntry(ClassBuffer *buffer);

	LineNumberTableEntry *EncodeEntry(ClassBuilder *builder);
};

struct LineNumberTableAttribute
		: public AttributeInfo {
	// Line Number Table
	std::vector<LineNumberTableEntry *> line_number_table;

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

	LocalVariableTableEntry()
		: start_pc(0), length(0), name(NULL),
		  descriptor(NULL), index(0) {
	}

	LocalVariableTableEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	LocalVariableTableEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct LocalVariableTableAttribute
		: public AttributeInfo {
	// Local Variable Table
	std::vector<LocalVariableTableEntry *> local_variable_table;

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

	LocalVariableTypeTableEntry()
		: start_pc(0), length(0), name(NULL),
		  signature(NULL), index(0) {
	}

	LocalVariableTypeTableEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	LocalVariableTypeTableEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct LocalVariableTypeTableAttribute
		: public AttributeInfo {
	// Local Variable Type Table
	std::vector<LocalVariableTypeTableEntry *> local_variable_type_table;

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

struct RuntimeAnnotationsAttribute
		: public AttributeInfo {
	// Annotations Table
	std::vector<AnnotationEntry *> annotations;

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
	std::vector<AnnotationEntry *> annotations;

	ParameterAnnotationsEntry() {
	}

	~ParameterAnnotationsEntry();

	ParameterAnnotationsEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	ParameterAnnotationsEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct RuntimeParameterAnnotationsAttribute
		: public AttributeInfo {
	// Parameters Table
	std::vector<ParameterAnnotationsEntry *> parameter_annotations;

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
	std::vector<ConstantInfo *> bootstrap_arguments;

	BootstrapMethodEntry()
		: bootstrap_method_ref(NULL) {
	}

	~BootstrapMethodEntry();

	BootstrapMethodEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	BootstrapMethodEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
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

/* Attribute Producers */

typedef AttributeInfo *(* AttributeProducer)(
		ConstantUtf8Info *name, uint32_t attribute_length);

void RegisterProducer(std::string name, AttributeProducer producer);

/* Encode and Decode */

AttributeInfo *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile);

int EncodeAttribute(ClassBuilder *builder, ClassFile *classFile, AttributeInfo *info);

} /* JBC */

# endif /* AttributeInfo.h */
