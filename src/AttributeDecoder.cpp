
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

AttributeInfo *decodeConstantValueAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ConstantValueAttribute *info = new ConstantValueAttribute;

	// Constant Value
	index = buffer->NextShort();
	info->constant_value = classFile->constant_pool[index];

	return (AttributeInfo *)info;
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

AttributeInfo *decodeCodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	CodeAttribute *code = new CodeAttribute;

	// Maximums
	code->max_stack = buffer->NextShort();
	code->max_locals = buffer->NextShort();

	// Code
	code->code_length = buffer->NextInt();
	debug_printf(level2, "Code length : %d.\n", code->code_length);
	code->code = new uint8_t[code->code_length];

	for(idx = 0; idx < code->code_length; idx++) {
		// TODO : Added a mass read operation
		code->code[idx] = buffer->NextByte();
	}

	// Exception Table
	length = buffer->NextShort();
	code->exception_table = createList();
	debug_printf(level2, "Code Exception table length : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Exception table entry %d :\n", idx);
		listAdd(code->exception_table, decodeExceptionTableEntry(classFile, buffer));
	}

	// Attributes Table
	length = buffer->NextShort();
	code->attributes = createList();
	debug_printf(level2, "Code Attributes count : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %d :\n", idx);
		listAdd(code->attributes, decodeAttribute(classFile, buffer));
	}

	return (AttributeInfo *)code;
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

AttributeInfo *decodeStackMapTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	StackMapTableAttribute *table = new StackMapTableAttribute;

	debug_printf(level2, "Decoding StackMapTable.\n");

	// Stack Map Table
	length = buffer->NextShort();
	debug_printf(level2, "Stack Frame count : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Stack Map Frame %d :\n", idx);
		table->entries.push_back(decodeStackMapFrame(classFile, buffer));
	}

	debug_printf(level2, "Finished StackMapTable.\n");

	return (AttributeInfo *)table;
}

AttributeInfo *decodeExceptionsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ExceptionsAttribute *except = new ExceptionsAttribute;

	// Exceptions Table
	length = buffer->NextShort();
	debug_printf(level2, "Exceptions count : %d.\n", length);

	except->exception_table = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		debug_printf(level2, "Exception entry %d :\n", idx);
		listAdd(except->exception_table, classFile->constant_pool[index]);
	}

	return (AttributeInfo *)except;
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

AttributeInfo *decodeInnerClassesAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	InnerClassesAttribute *inner = new InnerClassesAttribute;

	// Inner Classes Table
	length = buffer->NextShort();
	debug_printf(level2, "Inner classes count : %d.\n", length);
	inner->classes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Inner class %d :\n", idx);
		listAdd(inner->classes, decodeInnerClassEntry(classFile, buffer));
	}

	return (AttributeInfo *)inner;
}

AttributeInfo *decodeEnclosingMethodAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	EnclosingMethodAttribute *enclose = new EnclosingMethodAttribute;

	// Enclosing Class
	index = buffer->NextShort();
	enclose->enclosing_class = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	//Enclosing Method
	index = buffer->NextShort();
	enclose->enclosing_method = static_cast<ConstantNameAndTypeInfo *>(
			classFile->constant_pool[index]);

	return (AttributeInfo *)enclose;
}

AttributeInfo *decodeSyntheticAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ignore_unused(buffer); ignore_unused(classFile);
	return (AttributeInfo *)new SyntheticAttribute;
}

AttributeInfo *decodeSignatureAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SignatureAttribute *signature = new SignatureAttribute;

	// Signature
	index = buffer->NextShort();
	signature->signature = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);

	return (AttributeInfo *)signature;
}

AttributeInfo *decodeSourceFileAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SourceFileAttribute *file = new SourceFileAttribute;

	// Source File
	index = buffer->NextShort();
	file->source_file = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);

	debug_printf(level2, "Source file name : %s.\n", file->source_file->bytes);

	return (AttributeInfo *)file;
}

AttributeInfo *decodeSourceDebugExtensionAttribute(
		ClassFile *classFile, ClassBuffer *buffer, uint32_t length) {
	unsigned int idx;
	SourceDebugExtensionAttribute *source = new SourceDebugExtensionAttribute;

	// Debug Extension
	source->debug_extension = new uint8_t[length];

	for(idx = 0; idx < length; idx++) {
		// TODO : Added a mass read operation
		source->debug_extension[idx] = buffer->NextByte();
	}

	ignore_unused(classFile);
	return (AttributeInfo *)source;
}

LineNumberTableEntry *decodeLineNumberTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	LineNumberTableEntry *entry = new LineNumberTableEntry;

	entry->start_pc = buffer->NextShort();
	entry->line_number = buffer->NextShort();

	ignore_unused(classFile);
	return entry;
}

AttributeInfo *decodeLineNumberTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LineNumberTableAttribute *table = new LineNumberTableAttribute;

	// Line Number Table
	length = buffer->NextShort();
	debug_printf(level2, "Line Number Table length : %d.\n", length);
	table->line_number_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(table->line_number_table,
				decodeLineNumberTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)table;
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

AttributeInfo *decodeLocalVariableTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTableAttribute *local = new LocalVariableTableAttribute;

	// Local Variable Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Table length : %d.\n", length);

	local->local_variable_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(local->local_variable_table,
				decodeLocalVariableTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)local;
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

AttributeInfo *decodeLocalVariableTypeTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTypeTableAttribute *local = new LocalVariableTypeTableAttribute;

	// Local Variable Type Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Type Table length : %d.\n", length);

	local->local_variable_type_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(local->local_variable_type_table,
				decodeLocalVariableTypeTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)local;
}

AttributeInfo *decodeDeprecatedAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ignore_unused(buffer);
	ignore_unused(classFile);
	return (AttributeInfo *)new DeprecatedAttribute;
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

AttributeInfo *decodeRuntimeAnnotationsAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	RuntimeAnnotationsAttribute *annot = new RuntimeAnnotationsAttribute;

	// Annotations Table
	length = buffer->NextShort();
	annot->annotations = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(annot->annotations, decodeAnnotationEntry(classFile, buffer));
	}

	return (AttributeInfo *)annot;
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

AttributeInfo *decodeRuntimeParameterAnnotationsAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	RuntimeParameterAnnotationsAttribute *annot = new RuntimeParameterAnnotationsAttribute;

	// Parameter Annotations Table
	length = buffer->NextByte();
	debug_printf(level2, "Parameter Annotation count : %u.\n", length);
	annot->parameter_annotations = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Parameter Annotation %u :\n", idx);
		listAdd(annot->parameter_annotations,
				decodeParameterAnnotationsEntry(classFile, buffer));
	}

	return (AttributeInfo *)annot;
}

AttributeInfo *decodeAnnotationDefaultAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AnnotationDefaultAttribute *annot = new AnnotationDefaultAttribute;

	// Default Value
	annot->default_value = decodeElementValue(classFile, buffer);

	return (AttributeInfo *)annot;
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

AttributeInfo *decodeBootstrapMethodsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodsAttribute *bootstrap = new BootstrapMethodsAttribute;

	// Bootstrap Method Table
	length = buffer->NextShort();
	bootstrap->bootstrap_methods = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(bootstrap->bootstrap_methods,
				decodeBootstrapMethodEntry(classFile, buffer));
	}

	return (AttributeInfo *)bootstrap;
}

AttributeInfo *decodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AttributeInfo *info;

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
		info = decodeConstantValueAttribute(classFile, buffer);
	} else
	// Code Attribute
	if(!strcmp("Code", (char *)name->bytes)) {
		info = decodeCodeAttribute(classFile, buffer);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", (char *)name->bytes)) {
		info = decodeStackMapTableAttribute(classFile, buffer);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", (char *)name->bytes)) {
		info = decodeExceptionsAttribute(classFile, buffer);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", (char *)name->bytes)) {
		info = decodeInnerClassesAttribute(classFile, buffer);
	} else
	// Enclosing Method Attribute
	if(!strcmp("EnclosingMethod", (char *)name->bytes)) {
		info = decodeEnclosingMethodAttribute(classFile, buffer);
	} else
	// Synthetic Attribute
	if(!strcmp("Synthetic", (char *)name->bytes)) {
		info = decodeSyntheticAttribute(classFile, buffer);
	} else
	// Signature Attribute
	if(!strcmp("Signature", (char *)name->bytes)) {
		info = decodeSignatureAttribute(classFile, buffer);
	} else
	// Source File Attribute
	if(!strcmp("SourceFile", (char *)name->bytes)) {
		info = decodeSourceFileAttribute(classFile, buffer);
	} else
	// Source Debug Extension Attribute
	if(!strcmp("SourceDebugExtension", (char *)name->bytes)) {
		info = decodeSourceDebugExtensionAttribute(
				classFile, buffer, attribute_length);
	} else
	// Line Number Table Attribute
	if(!strcmp("LineNumberTable", (char *)name->bytes)) {
		info = decodeLineNumberTableAttribute(classFile, buffer);
	} else
	// Local Variable Table Attribute
	if(!strcmp("LocalVariableTable", (char *)name->bytes)) {
		info = decodeLocalVariableTableAttribute(classFile, buffer);
	} else
	// Local Variable Type Table Attribute
	if(!strcmp("LocalVariableTypeTable", (char *)name->bytes)) {
		info = decodeLocalVariableTypeTableAttribute(classFile, buffer);
	} else
	// Deprecated Attribute
	if(!strcmp("Deprecated", (char *)name->bytes)) {
		info = decodeDeprecatedAttribute(classFile, buffer);
	} else
	// Runtime Visible Annotations Attribute
	if(!strcmp("RuntimeVisibleAnnotations", (char *)name->bytes)) {
		info = decodeRuntimeAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Invisible Annotations Attribute
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)name->bytes)) {
		info = decodeRuntimeAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)name->bytes)) {
		info = decodeRuntimeParameterAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)name->bytes)) {
		info = decodeRuntimeParameterAnnotationsAttribute(classFile, buffer);
	} else
	// Annotation Default Attribute
	if(!strcmp("AnnotationDefault", (char *)name->bytes)) {
		info = decodeAnnotationDefaultAttribute(classFile, buffer);
	} else
	// Bootstrap Methods Attribute
	if(!strcmp("BootstrapMethods", (char *)name->bytes)) {
		info = decodeBootstrapMethodsAttribute(classFile, buffer);
	} else {
		debug_printf(level2, "Unknown Attribute type : %s; Skipping.\n", name->bytes);
		while(bufferPos(buffer) - initpos < attribute_length) {
			// TODO : Implement a skip operation
			buffer->NextByte();
		}
	}

	if(bufferPos(buffer) - initpos != attribute_length) {
		fprintf(stderr, "Attribute length mismatch!\n");
		exit(EXIT_FAILURE);
	}

	debug_printf(level1, "Finished Attribute : %s.\n", name->bytes);
	info->attribute_length = attribute_length;
	info->name = name;

	return info;
}
