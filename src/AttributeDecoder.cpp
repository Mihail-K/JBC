
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

ConstantValueAttribute *ConstantValueAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	// Constant Value
	index = buffer->NextShort();
	constant_value = classFile->constant_pool[index];

	return this;
}

ExceptionTableEntry *decodeExceptionTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	ExceptionTableEntry *entry = new ExceptionTableEntry;

	entry->start_pc = buffer->NextShort();
	entry->end_pc = buffer->NextShort();
	entry->handler_pc = buffer->NextShort();
	entry->catch_type = buffer->NextShort();

	ignore_unused(classFile);
	return entry;
}

CodeAttribute *CodeAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Maximums
	max_stack = buffer->NextShort();
	max_locals = buffer->NextShort();

	// Code
	code_length = buffer->NextInt();
	debug_printf(level2, "Code length : %d.\n", code_length);

	code = new uint8_t[code_length];
	for(unsigned idx = 0; idx < code_length; idx++) {
		// TODO : Added a mass read operation
		code[idx] = buffer->NextByte();
	}

	// Exception Table
	length = buffer->NextShort();
	debug_printf(level2, "Code Exception table length : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Exception table entry %d :\n", idx);
		exception_table.push_back(decodeExceptionTableEntry(classFile, buffer));
	}

	// Attributes Table
	length = buffer->NextShort();
	debug_printf(level2, "Code Attributes count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %d :\n", idx);
		attributes.push_back(decodeAttribute(classFile, buffer));
	}

	return this;
}

StackMapFrame *decodeStackMapFrame(ClassFile *classFile, ClassBuffer *buffer) {
	uint8_t tag = buffer->NextByte();

	debug_printf(level3, "Decoding Stack Frame type : %d.\n", tag);

	// Stack Map Same Frame
	if(tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		return new StackMapFrame(tag);
	} else
	// Stack Map Same Locals 1
	if(tag >= 64 && tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		return (new StackMapItemFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Reserved Values
	if(tag >= 128 && tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		return (new StackMapExtFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Chop Frame
	if(tag >= 248 && tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		return (new StackMapOffFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Same Frame Extended
	if(tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		return (new StackMapOffFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Append Frame
	if(tag >= 252 && tag <= 254) {
		debug_printf(level3, "Stack Map append frame.\n");
		return (new StackMapListFrame(tag))->DecodeFrame(buffer, classFile);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		return (new StackMapFullFrame(tag))->DecodeFrame(buffer, classFile);
	}
}

StackMapTableAttribute *StackMapTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level2, "Decoding StackMapTable.\n");

	// Stack Map Table
	length = buffer->NextShort();
	debug_printf(level2, "Stack Frame count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Stack Map Frame %d :\n", idx);
		entries.push_back(decodeStackMapFrame(classFile, buffer));
	}

	debug_printf(level2, "Finished StackMapTable.\n");

	return this;
}

ExceptionsAttribute *ExceptionsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Exceptions Table
	length = buffer->NextShort();
	debug_printf(level2, "Exceptions count : %d.\n", length);

	exception_table = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		debug_printf(level2, "Exception entry %d :\n", idx);
		listAdd(exception_table, classFile->constant_pool[index]);
	}

	return this;
}

InnerClassEntry *decodeInnerClassEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	InnerClassEntry *entry = new InnerClassEntry;

	// Inner Class Info
	index = buffer->NextShort();
	entry->inner_class_info = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	// Outer Class Info
	index = buffer->NextShort();
	entry->outer_class_info = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	// Inner Class Name
	index = buffer->NextShort();
	entry->inner_class_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	debug_printf(level2, "Inner class name : %s.\n",
			(index != 0 ? (char *)entry->inner_class_name->bytes
			: "<anonymous class>"));

	// Inner Class Flags
	index = buffer->NextShort();
	entry->inner_class_access_flags = index;

	return entry;
}

InnerClassesAttribute *InnerClassesAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Inner Classes Table
	length = buffer->NextShort();
	debug_printf(level2, "Inner classes count : %d.\n", length);
	classes = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Inner class %d :\n", idx);
		listAdd(classes, decodeInnerClassEntry(classFile, buffer));
	}

	return this;
}

EnclosingMethodAttribute *EnclosingMethodAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	// Enclosing Class
	index = buffer->NextShort();
	enclosing_class = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	//Enclosing Method
	index = buffer->NextShort();
	enclosing_method = static_cast<ConstantNameAndTypeInfo *>(
			classFile->constant_pool[index]);

	return this;
}

SignatureAttribute *SignatureAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	// Signature
	index = buffer->NextShort();
	signature = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	return this;
}

SourceFileAttribute *SourceFileAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	// Source File
	index = buffer->NextShort();
	source_file = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	debug_printf(level2, "Source file name : %s.\n", source_file->bytes);

	return this;
}

SourceDebugExtensionAttribute *SourceDebugExtensionAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *) {
	// Debug Extension
	debug_extension = new uint8_t[attribute_length];

	for(unsigned idx = 0; idx < attribute_length; idx++) {
		// TODO : Added a mass read operation
		debug_extension[idx] = buffer->NextByte();
	}

	return this;
}

LineNumberTableEntry *decodeLineNumberTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	LineNumberTableEntry *entry = new LineNumberTableEntry;

	entry->start_pc = buffer->NextShort();
	entry->line_number = buffer->NextShort();

	ignore_unused(classFile);
	return entry;
}

LineNumberTableAttribute *LineNumberTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Line Number Table
	length = buffer->NextShort();
	debug_printf(level2, "Line Number Table length : %d.\n", length);
	line_number_table = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		listAdd(line_number_table,
				decodeLineNumberTableEntry(classFile, buffer));
	}

	return this;
}

LocalVariableTableEntry *decodeLocalVariableTableEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	LocalVariableTableEntry *entry = new LocalVariableTableEntry;

	entry->start_pc = buffer->NextShort();
	entry->length = buffer->NextShort();

	// Variable Name
	index = buffer->NextShort();
	entry->name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Descriptor
	index = buffer->NextShort();
	entry->descriptor = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable descriptor : %s.\n", entry->descriptor->bytes);

	index = buffer->NextShort();
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

LocalVariableTableAttribute *LocalVariableTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Local Variable Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Table length : %d.\n", length);

	local_variable_table = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		listAdd(local_variable_table,
				decodeLocalVariableTableEntry(classFile, buffer));
	}

	return this;
}

LocalVariableTypeTableEntry *decodeLocalVariableTypeTableEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	LocalVariableTypeTableEntry *entry = new LocalVariableTypeTableEntry;

	entry->start_pc = buffer->NextShort();
	entry->length = buffer->NextShort();

	// Variable Type Name
	index = buffer->NextShort();
	entry->name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Type Signature
	index = buffer->NextShort();
	entry->signature = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable signature : %s.\n", entry->signature->bytes);

	index = buffer->NextShort();
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

LocalVariableTypeTableAttribute *LocalVariableTypeTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Local Variable Type Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Type Table length : %d.\n", length);

	local_variable_type_table = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		listAdd(local_variable_type_table,
				decodeLocalVariableTypeTableEntry(classFile, buffer));
	}

	return this;
}

ElementValue *decodeConstElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = new ElementValue;

	// Constant Value
	index = buffer->NextShort();
	value->value.const_value = classFile->constant_pool[index];

	return value;
}

ElementValue *decodeEnumElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = new ElementValue;

	// Type Name
	index = buffer->NextShort();
	value->value.enum_const_value.type_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	// Constant Name
	index = buffer->NextShort();
	value->value.enum_const_value.const_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	return value;
}

ElementValue *decodeClassElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = new ElementValue;

	// Class Info
	index = buffer->NextShort();
	value->value.class_info = static_cast<ConstantClassInfo *>(classFile->constant_pool[index]);

	return value;
}

ElementValue *decodeAnnotationElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern AnnotationEntry *decodeAnnotationEntry(ClassFile *, ClassBuffer *);

	ElementValue *value = new ElementValue;

	// Annotation Value
	value->value.annotation_value = decodeAnnotationEntry(classFile, buffer);

	return value;
}

ElementValue *decodeArrayElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern ElementValue *decodeElementValue(ClassFile *, ClassBuffer *);

	unsigned int idx, length;
	ElementValue *value = new ElementValue;

	// Array Value Table
	length = buffer->NextShort();
	value->value.array_values = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(value->value.array_values,
				decodeElementValue(classFile, buffer));
	}

	return value;
}

ElementValue *decodeElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	ElementValue *value;
	uint8_t tag = buffer->NextByte();

	switch(tag) {
		case 'B': case 'C': case 'D':
		case 'F': case 'I': case 'J':
		case 'S': case 'Z': case 's':
			debug_printf(level3, "Constant Element Value.\n");
			value = decodeConstElementValue(classFile, buffer);
			break;
		case 'e':
			debug_printf(level3, "Enum Constant Element Value.\n");
			value = decodeEnumElementValue(classFile, buffer);
			break;
		case 'c':
			debug_printf(level3, "Class Element Value.\n");
			value = decodeClassElementValue(classFile, buffer);
			break;
		case '@':
			debug_printf(level3, "Annotation Element Value.\n");
			value = decodeAnnotationElementValue(classFile, buffer);
			break;
		case '[':
			debug_printf(level3, "Array Element Value.\n");
			value = decodeArrayElementValue(classFile, buffer);
			break;
	}

	value->tag = tag;
	return value;
}

ElementValuePairsEntry *decodeElementValuePairsEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValuePairsEntry *entry = new ElementValuePairsEntry;

	// Element Name
	index = buffer->NextShort();
	entry->element_name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Element-Value Name : %s.\n", entry->element_name->bytes);

	// Element Value
	entry->value = decodeElementValue(classFile, buffer);

	return entry;
}

AnnotationEntry *decodeAnnotationEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx, length;
	AnnotationEntry *entry = new AnnotationEntry;

	// Annotation Entry Type
	index = buffer->NextShort();
	entry->type = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);

	// Element Value Pairs Table
	length = buffer->NextShort();
	debug_printf(level2, "Element-Value Pairs count : %u.\n", length);
	entry->element_value_pairs = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Element-Value Pair %u :\n", idx);
		listAdd(entry->element_value_pairs,
				decodeElementValuePairsEntry(classFile, buffer));
	}

	return entry;
}

RuntimeAnnotationsAttribute *RuntimeAnnotationsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Annotations Table
	length = buffer->NextShort();
	annotations = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		listAdd(annotations, decodeAnnotationEntry(classFile, buffer));
	}

	return this;
}

ParameterAnnotationsEntry *decodeParameterAnnotationsEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ParameterAnnotationsEntry *entry = new ParameterAnnotationsEntry;

	// Annotations Table
	length = buffer->NextShort();
	debug_printf(level2, "Parameter Annotations entry count : %u.\n", length);
	entry->annotations = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Parameter Annotation entry %u :\n", idx);
		listAdd(entry->annotations, decodeAnnotationEntry(classFile, buffer));
	}

	return entry;
}

RuntimeParameterAnnotationsAttribute *RuntimeParameterAnnotationsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Parameter Annotations Table
	length = buffer->NextByte();
	debug_printf(level2, "Parameter Annotation count : %u.\n", length);
	parameter_annotations = createList();

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Parameter Annotation %u :\n", idx);
		listAdd(parameter_annotations,
				decodeParameterAnnotationsEntry(classFile, buffer));
	}

	return this;
}

AnnotationDefaultAttribute *AnnotationDefaultAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	// Default Value
	default_value = decodeElementValue(classFile, buffer);

	return this;
}

BootstrapMethodEntry *decodeBootstrapMethodEntry(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodEntry *entry = new BootstrapMethodEntry;

	// Bootstrap Method Parameters Table
	length = buffer->NextShort();
	entry->bootstrap_arguments = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		listAdd(entry->bootstrap_arguments, classFile->constant_pool[index]);
	}

	return entry;
}

BootstrapMethodsAttribute *BootstrapMethodsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Bootstrap Method Table
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		bootstrap_methods.push_back(decodeBootstrapMethodEntry(classFile, buffer));
	}

	return this;
}

AttributeInfo *decodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t name_index = buffer->NextShort();
	uint32_t attribute_length = buffer->NextInt();
	ConstantUtf8Info *name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[name_index]);
	unsigned long int initpos = bufferPos(buffer);

	if(name == NULL) {
		fprintf(stderr, "Error : Attribute with no name entry!\n");
		exit(EXIT_FAILURE);
	}

	debug_printf(level1, "Decoding Attribute type : %s.\n", name->bytes);

	// Constant Value Attribute
	if(!strcmp("ConstantValue", (char *)name->bytes)) {
		return (new ConstantValueAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Code Attribute
	if(!strcmp("Code", (char *)name->bytes)) {
		return (new CodeAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", (char *)name->bytes)) {
		return (new StackMapTableAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", (char *)name->bytes)) {
		return (new ExceptionsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", (char *)name->bytes)) {
		return (new InnerClassesAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Enclosing Method Attribute
	if(!strcmp("EnclosingMethod", (char *)name->bytes)) {
		return (new EnclosingMethodAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Synthetic Attribute
	if(!strcmp("Synthetic", (char *)name->bytes)) {
		return new SyntheticAttribute(name, attribute_length);
	} else
	// Signature Attribute
	if(!strcmp("Signature", (char *)name->bytes)) {
		return (new SignatureAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Source File Attribute
	if(!strcmp("SourceFile", (char *)name->bytes)) {
		return (new SourceFileAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Source Debug Extension Attribute
	if(!strcmp("SourceDebugExtension", (char *)name->bytes)) {
		return (new SourceDebugExtensionAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Line Number Table Attribute
	if(!strcmp("LineNumberTable", (char *)name->bytes)) {
		return (new LineNumberTableAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Local Variable Table Attribute
	if(!strcmp("LocalVariableTable", (char *)name->bytes)) {
		return (new LocalVariableTableAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Local Variable Type Table Attribute
	if(!strcmp("LocalVariableTypeTable", (char *)name->bytes)) {
		return (new LocalVariableTypeTableAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Deprecated Attribute
	if(!strcmp("Deprecated", (char *)name->bytes)) {
		return new DeprecatedAttribute(name, attribute_length);
	} else
	// Runtime Visible Annotations Attribute
	if(!strcmp("RuntimeVisibleAnnotations", (char *)name->bytes)) {
		return (new RuntimeVisibleAnnotationsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Runtime Invisible Annotations Attribute
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)name->bytes)) {
		return (new RuntimeVisibleAnnotationsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)name->bytes)) {
		return (new RuntimeVisibleParameterAnnotationsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)name->bytes)) {
		return (new RuntimeVisibleParameterAnnotationsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Annotation Default Attribute
	if(!strcmp("AnnotationDefault", (char *)name->bytes)) {
		return (new AnnotationDefaultAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else
	// Bootstrap Methods Attribute
	if(!strcmp("BootstrapMethods", (char *)name->bytes)) {
		return (new BootstrapMethodsAttribute(name, attribute_length))
				->DecodeAttribute(buffer, classFile);
	} else {
		debug_printf(level2, "Unknown Attribute type : %s; Skipping.\n", name->bytes);
		while(bufferPos(buffer) - initpos < attribute_length) {
			// TODO : Implement a skip operation
			buffer->NextByte();
		}
	}

	return NULL;
}
