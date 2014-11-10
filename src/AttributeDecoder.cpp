
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "Zalloc.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

AttributeInfo *decodeConstantValueAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ConstantValueAttribute *info = NEW(ConstantValueAttribute);

	// Constant Value
	index = bufferNextShort(buffer);
	info->constant_value = static_cast(ConstantInfo *, getConstant(classFile, index));

	return (AttributeInfo *)info;
}

ExceptionTableEntry *decodeExceptionTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	ExceptionTableEntry *entry = NEW(ExceptionTableEntry);

	entry->start_pc = bufferNextShort(buffer);
	entry->end_pc = bufferNextShort(buffer);
	entry->handler_pc = bufferNextShort(buffer);
	entry->catch_type = bufferNextShort(buffer);

	ignore_unused(classFile);
	return entry;
}

AttributeInfo *decodeCodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	CodeAttribute *code = NEW(CodeAttribute);

	// Maximums
	code->max_stack = bufferNextShort(buffer);
	code->max_locals = bufferNextShort(buffer);

	// Code
	code->code_length = bufferNextInt(buffer);
	debug_printf(level2, "Code length : %d.\n", code->code_length);
	code->code = ALLOC(uint8_t, code->code_length);

	for(idx = 0; idx < code->code_length; idx++) {
		// TODO : Added a mass read operation
		code->code[idx] = bufferNextByte(buffer);
	}

	// Exception Table
	length = bufferNextShort(buffer);
	code->exception_table = createList();
	debug_printf(level2, "Code Exception table length : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Exception table entry %d :\n", idx);
		listAdd(code->exception_table, decodeExceptionTableEntry(classFile, buffer));
	}

	// Attributes Table
	length = bufferNextShort(buffer);
	code->attributes = createList();
	debug_printf(level2, "Code Attributes count : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %d :\n", idx);
		listAdd(code->attributes, decodeAttribute(classFile, buffer));
	}

	return (AttributeInfo *)code;
}

VerificationTypeInfo *decodeVerificationTypeInfo(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	VerificationTypeInfo *info = NEW(VerificationTypeInfo);

	info->tag = bufferNextByte(buffer);
	switch(info->tag) {
		case 0:
			debug_printf(level3, "Top variable info.\n");
			break;
		case 1:
			debug_printf(level3, "Integer variable info.\n");
			break;
		case 2:
			debug_printf(level3, "Float variable info.\n");
			break;
		case 3:
			debug_printf(level3, "Double variable info.\n");
			break;
		case 4:
			debug_printf(level3, "Long variable info.\n");
			break;
		case 5:
			debug_printf(level3, "Null variable info.\n");
			break;
		case 6:
			debug_printf(level3, "Uninitialized this variable info.\n");
			break;
		case 7:
			debug_printf(level3, "Object variable info.\n");
			index = bufferNextShort(buffer);
			info->object_variable_info.object = static_cast(ConstantClassInfo *,
					getConstant(classFile, index));
			break;
		case 8:
			debug_printf(level3, "Uninitialized variable info.\n");
			index = bufferNextShort(buffer);
			info->uninitialized_variable_info.offset = index;
			break;
		default:
			fprintf(stderr, "Unknown verification type (ID : %d)!\n", info->tag);
			exit(EXIT_FAILURE);
	}

	return info;
}

StackMapFrame *decodeStackMapOffFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapOffFrame *frame = NEW(StackMapOffFrame);
	frame->offset_delta = bufferNextShort(buffer);

	ignore_unused(classFile);
	return (StackMapFrame *)frame;
}

StackMapFrame *decodeStackMapItemFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapItemFrame *frame = NEW(StackMapItemFrame);
	frame->stack = decodeVerificationTypeInfo(classFile, buffer);

	return (StackMapFrame *)frame;
}

StackMapFrame *decodeStackMapExtFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapExtFrame *frame = NEW(StackMapExtFrame);

	frame->offset_delta = bufferNextShort(buffer);
	frame->stack = decodeVerificationTypeInfo(classFile, buffer);

	return (StackMapFrame *)frame;
}

StackMapFrame *decodeStackMapListFrame(
		ClassFile *classFile, ClassBuffer *buffer, unsigned int count) {
	unsigned int idx;
	StackMapListFrame *frame = NEW(StackMapListFrame);

	frame->offset_delta = bufferNextShort(buffer);
	frame->stack = ALLOC(VerificationTypeInfo *, count);

	for(idx = 0; idx < count; idx++) {
		frame->stack[idx] = decodeVerificationTypeInfo(classFile, buffer);
	}

	return (StackMapFrame *)frame;
}

StackMapFrame *decodeStackMapFullFrame(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	StackMapFullFrame *frame = NEW(StackMapFullFrame);

	frame->offset_delta = bufferNextShort(buffer);

	// Stack Frame Locals
	length = bufferNextShort(buffer);
	frame->locals = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(frame->locals, decodeVerificationTypeInfo(classFile, buffer));
	}

	// Stack Frame Items
	length = bufferNextShort(buffer);
	frame->stack = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(frame->stack, decodeVerificationTypeInfo(classFile, buffer));
	}

	return (StackMapFrame *)frame;
}

StackMapFrame *decodeStackMapFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapFrame *frame;
	uint8_t tag = bufferNextByte(buffer);

	debug_printf(level3, "Decoding Stack Frame type : %d.\n", tag);

	// Stack Map Same Frame
	if(tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		frame = NEW(StackMapFrame);
	} else
	// Stack Map Same Locals 1
	if(tag >= 64 && tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		frame = decodeStackMapItemFrame(classFile, buffer);
	} else
	// Reserved Values
	if(tag >= 128 && tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		frame = decodeStackMapExtFrame(classFile, buffer);
	} else
	// Stack Map Chop Frame
	if(tag >= 248 && tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		frame = decodeStackMapOffFrame(classFile, buffer);
	} else
	// Stack Map Same Frame Extended
	if(tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		frame = decodeStackMapOffFrame(classFile, buffer);
	} else
	// Stack Map Append Frame
	if(tag >= 252 && tag <= 254) {
		int count = tag - 251;
		debug_printf(level3, "Stack Map append frame.\n");
		frame = decodeStackMapListFrame(classFile, buffer, count);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		frame = decodeStackMapFullFrame(classFile, buffer);
	}

	debug_printf(level3, "Finished Stack Frame.\n");

	frame->tag = tag;
	return frame;
}

AttributeInfo *decodeStackMapTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	StackMapTableAttribute *table = NEW(StackMapTableAttribute);

	debug_printf(level2, "Decoding StackMapTable.\n");

	// Stack Map Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Stack Frame count : %d.\n", length);
	table->entries = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Stack Map Frame %d :\n", idx);
		listAdd(table->entries,  decodeStackMapFrame(classFile, buffer));
	}

	debug_printf(level2, "Finished StackMapTable.\n");

	return (AttributeInfo *)table;
}

AttributeInfo *decodeExceptionsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ExceptionsAttribute *except = NEW(ExceptionsAttribute);

	// Exceptions Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Exceptions count : %d.\n", length);

	except->exception_table = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = bufferNextShort(buffer);
		debug_printf(level2, "Exception entry %d :\n", idx);
		listAdd(except->exception_table, getConstant(classFile, index));
	}

	return (AttributeInfo *)except;
}

InnerClassEntry *decodeInnerClassEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	InnerClassEntry *entry = NEW(InnerClassEntry);

	// Inner Class Info
	index = bufferNextShort(buffer);
	entry->inner_class_info = static_cast(ConstantClassInfo *,
			getConstant(classFile, index));

	// Outer Class Info
	index = bufferNextShort(buffer);
	entry->outer_class_info = static_cast(ConstantClassInfo *,
			getConstant(classFile, index));

	// Inner Class Name
	index = bufferNextShort(buffer);
	entry->inner_class_name = static_cast(ConstantUtf8Info *,
			getConstant(classFile, index));

	debug_printf(level2, "Inner class name : %s.\n",
			(index != 0 ? (char *)entry->inner_class_name->bytes
			: "<anonymous class>"));

	// Inner Class Flags
	index = bufferNextShort(buffer);
	entry->inner_class_access_flags = index;

	return entry;
}

AttributeInfo *decodeInnerClassesAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	InnerClassesAttribute *inner = NEW(InnerClassesAttribute);

	// Inner Classes Table
	length = bufferNextShort(buffer);
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
	EnclosingMethodAttribute *enclose = NEW(EnclosingMethodAttribute);

	// Enclosing Class
	index = bufferNextShort(buffer);
	enclose->enclosing_class = static_cast(ConstantClassInfo *,
			getConstant(classFile, index));

	//Enclosing Method
	index = bufferNextShort(buffer);
	enclose->enclosing_method = static_cast(ConstantNameAndTypeInfo *,
			getConstant(classFile, index));

	return (AttributeInfo *)enclose;
}

AttributeInfo *decodeSyntheticAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ignore_unused(buffer); ignore_unused(classFile);
	return (AttributeInfo *)NEW(SyntheticAttribute);
}

AttributeInfo *decodeSignatureAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SignatureAttribute *signature = NEW(SignatureAttribute);

	// Signature
	index = bufferNextShort(buffer);
	signature->signature = static_cast(ConstantUtf8Info *, getConstant(classFile, index));

	return (AttributeInfo *)signature;
}

AttributeInfo *decodeSourceFileAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SourceFileAttribute *file = NEW(SourceFileAttribute);

	// Source File
	index = bufferNextShort(buffer);
	file->source_file = static_cast(ConstantUtf8Info *, getConstant(classFile, index));

	debug_printf(level2, "Source file name : %s.\n", file->source_file->bytes);

	return (AttributeInfo *)file;
}

AttributeInfo *decodeSourceDebugExtensionAttribute(
		ClassFile *classFile, ClassBuffer *buffer, uint32_t length) {
	unsigned int idx;
	SourceDebugExtensionAttribute *source = NEW(SourceDebugExtensionAttribute);

	// Debug Extension
	source->debug_extension = ALLOC(uint8_t, length);

	for(idx = 0; idx < length; idx++) {
		// TODO : Added a mass read operation
		source->debug_extension[idx] = bufferNextByte(buffer);
	}

	ignore_unused(classFile);
	return (AttributeInfo *)source;
}

LineNumberTableEntry *decodeLineNumberTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	LineNumberTableEntry *entry = NEW(LineNumberTableEntry);

	entry->start_pc = bufferNextShort(buffer);
	entry->line_number = bufferNextShort(buffer);

	ignore_unused(classFile);
	return entry;
}

AttributeInfo *decodeLineNumberTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LineNumberTableAttribute *table = NEW(LineNumberTableAttribute);

	// Line Number Table
	length = bufferNextShort(buffer);
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
	LocalVariableTableEntry *entry = NEW(LocalVariableTableEntry);

	entry->start_pc = bufferNextShort(buffer);
	entry->length = bufferNextShort(buffer);

	// Variable Name
	index = bufferNextShort(buffer);
	entry->name = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Descriptor
	index = bufferNextShort(buffer);
	entry->descriptor = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level3, "Local variable descriptor : %s.\n", entry->descriptor->bytes);

	index = bufferNextShort(buffer);
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

AttributeInfo *decodeLocalVariableTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTableAttribute *local = NEW(LocalVariableTableAttribute);

	// Local Variable Table
	length = bufferNextShort(buffer);
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
	LocalVariableTypeTableEntry *entry = NEW(LocalVariableTypeTableEntry);

	entry->start_pc = bufferNextShort(buffer);
	entry->length = bufferNextShort(buffer);

	// Variable Type Name
	index = bufferNextShort(buffer);
	entry->name = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Type Signature
	index = bufferNextShort(buffer);
	entry->signature = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level3, "Local variable signature : %s.\n", entry->signature->bytes);

	index = bufferNextShort(buffer);
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

AttributeInfo *decodeLocalVariableTypeTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTypeTableAttribute *local = NEW(LocalVariableTypeTableAttribute);

	// Local Variable Type Table
	length = bufferNextShort(buffer);
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
	return (AttributeInfo *)NEW(DeprecatedAttribute);
}

ElementValue *decodeConstElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = NEW(ElementValue);

	// Constant Value
	index = bufferNextShort(buffer);
	value->value.const_value = static_cast(ConstantInfo *, getConstant(classFile, index));

	return value;
}

ElementValue *decodeEnumElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = NEW(ElementValue);

	// Type Name
	index = bufferNextShort(buffer);
	value->value.enum_const_value.type_name = static_cast(
			ConstantUtf8Info *, getConstant(classFile, index));

	// Constant Name
	index = bufferNextShort(buffer);
	value->value.enum_const_value.const_name = static_cast(
			ConstantUtf8Info *, getConstant(classFile, index));

	return value;
}

ElementValue *decodeClassElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = NEW(ElementValue);

	// Class Info
	index = bufferNextShort(buffer);
	value->value.class_info = static_cast(ConstantClassInfo *, getConstant(classFile, index));

	return value;
}

ElementValue *decodeAnnotationElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern AnnotationEntry *decodeAnnotationEntry(ClassFile *, ClassBuffer *);

	ElementValue *value = NEW(ElementValue);

	// Annotation Value
	value->value.annotation_value = decodeAnnotationEntry(classFile, buffer);

	return value;
}

ElementValue *decodeArrayElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern ElementValue *decodeElementValue(ClassFile *, ClassBuffer *);

	unsigned int idx, length;
	ElementValue *value = NEW(ElementValue);

	// Array Value Table
	length = bufferNextShort(buffer);
	value->value.array_values = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(value->value.array_values,
				decodeElementValue(classFile, buffer));
	}

	return value;
}

ElementValue *decodeElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	ElementValue *value;
	uint8_t tag = bufferNextByte(buffer);

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
	ElementValuePairsEntry *entry = NEW(ElementValuePairsEntry);

	// Element Name
	index = bufferNextShort(buffer);
	entry->element_name = static_cast(ConstantUtf8Info *, getConstant(classFile, index));
	debug_printf(level3, "Element-Value Name : %s.\n", entry->element_name->bytes);

	// Element Value
	entry->value = decodeElementValue(classFile, buffer);

	return entry;
}

AnnotationEntry *decodeAnnotationEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx, length;
	AnnotationEntry *entry = NEW(AnnotationEntry);

	// Annotation Entry Type
	index = bufferNextShort(buffer);
	entry->type = static_cast(ConstantUtf8Info *, getConstant(classFile, index));

	// Element Value Pairs Table
	length = bufferNextShort(buffer);
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
	RuntimeAnnotationsAttribute *annot = NEW(
			RuntimeAnnotationsAttribute);

	// Annotations Table
	length = bufferNextShort(buffer);
	annot->annotations = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(annot->annotations, decodeAnnotationEntry(classFile, buffer));
	}

	return (AttributeInfo *)annot;
}

ParameterAnnotationsEntry *decodeParameterAnnotationsEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ParameterAnnotationsEntry *entry = NEW(ParameterAnnotationsEntry);

	// Annotations Table
	length = bufferNextShort(buffer);
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
	RuntimeParameterAnnotationsAttribute *annot = NEW(
			RuntimeParameterAnnotationsAttribute);

	// Parameter Annotations Table
	length = bufferNextByte(buffer);
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
	AnnotationDefaultAttribute *annot = NEW(AnnotationDefaultAttribute);

	// Default Value
	annot->default_value = decodeElementValue(classFile, buffer);

	return (AttributeInfo *)annot;
}

BootstrapMethodEntry *decodeBootstrapMethodEntry(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodEntry *entry = NEW(BootstrapMethodEntry);

	// Bootstrap Method Parameters Table
	length = bufferNextShort(buffer);
	entry->bootstrap_arguments = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = bufferNextShort(buffer);
		listAdd(entry->bootstrap_arguments, getConstant(classFile, index));
	}

	return entry;
}

AttributeInfo *decodeBootstrapMethodsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodsAttribute *bootstrap = NEW(BootstrapMethodsAttribute);

	// Bootstrap Method Table
	length = bufferNextShort(buffer);
	bootstrap->bootstrap_methods = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(bootstrap->bootstrap_methods,
				decodeBootstrapMethodEntry(classFile, buffer));
	}

	return (AttributeInfo *)bootstrap;
}

AttributeInfo *decodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AttributeInfo *info;

	uint16_t name_index = bufferNextShort(buffer);
	uint32_t attribute_length = bufferNextInt(buffer);
	ConstantUtf8Info *name = static_cast(ConstantUtf8Info *, getConstant(classFile, name_index));
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
			bufferNextByte(buffer);
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