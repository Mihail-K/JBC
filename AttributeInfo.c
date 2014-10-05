
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

AttributeInfo *visitConstantValueAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ConstantValueAttribute *info = zalloc(sizeof(ConstantValueAttribute));

	// Constant Value
	index = bufferNextShort(buffer);
	info->constant_value = getConstant(classFile, index);

	return (AttributeInfo *)info;
}

ExceptionTableEntry *visitExceptionTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	ExceptionTableEntry *entry = zalloc(sizeof(ExceptionTableEntry));

	ignore_unused(classFile);

	entry->start_pc = bufferNextShort(buffer);
	entry->end_pc = bufferNextShort(buffer);
	entry->handler_pc = bufferNextShort(buffer);
	entry->catch_type = bufferNextShort(buffer);

	return entry;
}

AttributeInfo *visitCodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	CodeAttribute *code = zalloc(sizeof(CodeAttribute));

	// Maximums
	code->max_stack = bufferNextShort(buffer);
	code->max_locals = bufferNextShort(buffer);

	// Code
	code->code_length = bufferNextInt(buffer);
	debug_printf(level2, "Code length : %d.\n", code->code_length);
	code->code = zalloc(sizeof(uint8_t) * code->code_length);

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
		listAdd(code->exception_table, visitExceptionTableEntry(classFile, buffer));
	}

	// Attributes Table
	length = bufferNextShort(buffer);
	code->attributes = createList();
	debug_printf(level2, "Code Attributes count : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %d :\n", idx);
		listAdd(code->attributes, visitAttribute(classFile, buffer));
	}

	return (AttributeInfo *)code;
}

VerificationTypeInfo *visitVerificationTypeInfo(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint8_t tag;
	uint16_t index;
	VerificationTypeInfo *info = zalloc(sizeof(VerificationTypeInfo));

	tag = bufferNextByte(buffer);
	switch(tag) {
		case 0:
			debug_printf(level3, "Top variable info.\n");
			info->top_variable_info.tag = tag;
			break;
		case 1:
			debug_printf(level3, "Integer variable info.\n");
			info->integer_variable_info.tag = tag;
			break;
		case 2:
			debug_printf(level3, "Float variable info.\n");
			info->float_variable_info.tag = tag;
			break;
		case 3:
			debug_printf(level3, "Double variable info.\n");
			info->double_variable_info.tag = tag;
			break;
		case 4:
			debug_printf(level3, "Long variable info.\n");
			info->long_variable_info.tag = tag;
			break;
		case 5:
			debug_printf(level3, "Null variable info.\n");
			info->null_variable_info.tag = tag;
			break;
		case 6:
			debug_printf(level3, "Uninitialized this variable info.\n");
			info->uninitialized_this_variable_info.tag = tag;
			break;
		case 7:
			debug_printf(level3, "Object variable info.\n");
			index = bufferNextShort(buffer);
			info->object_variable_info.tag = tag;
			info->object_variable_info.object = (void *)
					getConstant(classFile, index);
			break;
		case 8:
			debug_printf(level3, "Uninitialized variable info.\n");
			index = bufferNextShort(buffer);
			info->uninitialized_variable_info.tag = tag;
			info->uninitialized_variable_info.offset = index;
			break;
		default:
			fprintf(stderr, "Unknown verification type (ID : %d)!\n", tag);
			exit(EXIT_FAILURE);
	}

	return info;
}

StackMapFrame *visitStackMapOffFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapOffFrame *frame = zalloc(sizeof(StackMapOffFrame));

	ignore_unused(classFile);
	frame->offset_delta = bufferNextShort(buffer);

	return (StackMapFrame *)frame;
}

StackMapFrame *visitStackMapItemFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapItemFrame *frame = zalloc(sizeof(StackMapItemFrame));

	frame->stack = visitVerificationTypeInfo(classFile, buffer);

	return (StackMapFrame *)frame;
}

StackMapFrame *visitStackMapExtFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapExtFrame *frame = zalloc(sizeof(StackMapExtFrame));

	frame->offset_delta = bufferNextShort(buffer);
	frame->stack = visitVerificationTypeInfo(classFile, buffer);

	return (StackMapFrame *)frame;
}

StackMapFrame *visitStackMapListFrame(
		ClassFile *classFile, ClassBuffer *buffer, unsigned int count) {
	unsigned int idx;
	StackMapListFrame *frame = zalloc(sizeof(StackMapListFrame));

	frame->offset_delta = bufferNextShort(buffer);
	frame->stack = zalloc(sizeof(VerificationTypeInfo *) * count);

	for(idx = 0; idx < count; idx++) {
		frame->stack[idx] = visitVerificationTypeInfo(classFile, buffer);
	}

	return (StackMapFrame *)frame;
}

StackMapFrame *visitStackMapFullFrame(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	StackMapFullFrame *frame = zalloc(sizeof(StackMapFullFrame));

	frame->offset_delta = bufferNextShort(buffer);

	// Stack Frame Locals
	length = bufferNextShort(buffer);
	frame->locals = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(frame->locals, visitVerificationTypeInfo(classFile, buffer));
	}

	// Stack Frame Items
	length = bufferNextShort(buffer);
	frame->stack = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(frame->stack, visitVerificationTypeInfo(classFile, buffer));
	}

	return (StackMapFrame *)frame;
}

StackMapFrame *visitStackMapFrame(ClassFile *classFile, ClassBuffer *buffer) {
	StackMapFrame *frame;
	uint8_t tag = bufferNextByte(buffer);

	debug_printf(level3, "Visiting Stack Frame type : %d.\n", tag);

	// Stack Map Same Frame
	if(tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		frame = zalloc(sizeof(StackMapFrame));
	} else
	// Stack Map Same Locals 1
	if(tag >= 64 && tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		frame = visitStackMapItemFrame(classFile, buffer);
	} else
	// Reserved Values
	if(tag >= 128 && tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		frame = visitStackMapExtFrame(classFile, buffer);
	} else
	// Stack Map Chop Frame
	if(tag >= 248 && tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		frame = visitStackMapOffFrame(classFile, buffer);
	} else
	// Stack Map Same Frame Extended
	if(tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		frame = visitStackMapOffFrame(classFile, buffer);
	} else
	// Stack Map Append Frame
	if(tag >= 252 && tag <= 254) {
		int count = tag - 251;
		debug_printf(level3, "Stack Map append frame.\n");
		frame = visitStackMapListFrame(classFile, buffer, count);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		frame = visitStackMapFullFrame(classFile, buffer);
	}

	debug_printf(level3, "Finished Stack Frame.\n");

	frame->tag = tag;
	return frame;
}

AttributeInfo *visitStackMapTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	StackMapTableAttribute *table = zalloc(sizeof(StackMapTableAttribute));

	debug_printf(level2, "Visiting StackMapTable.\n");

	// Stack Map Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Stack Frame count : %d.\n", length);
	table->entries = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Stack Map Frame %d :\n", idx);
		listAdd(table->entries,  visitStackMapFrame(classFile, buffer));
	}

	debug_printf(level2, "Finished StackMapTable.\n");

	return (AttributeInfo *)table;
}

AttributeInfo *visitExceptionsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ExceptionsAttribute *except = zalloc(sizeof(ExceptionsAttribute));

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

InnerClassEntry *visitInnerClassEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	InnerClassEntry *entry = zalloc(sizeof(InnerClassEntry));

	// Inner Class Info
	index = bufferNextShort(buffer);
	entry->inner_class_info = getConstant(classFile, index);

	// Outer Class Info
	index = bufferNextShort(buffer);
	entry->outer_class_info = getConstant(classFile, index);

	// Inner Class Name
	index = bufferNextShort(buffer);
	entry->inner_class_name = getConstant(classFile, index);
	debug_printf(level2, "Inner class name : %s.\n",
			(index != 0 ? (char *)entry->inner_class_name->bytes
			: "<anonymous class>"));

	// Inner Class Flags
	index = bufferNextShort(buffer);
	entry->inner_class_access_flags = index;

	return entry;
}

AttributeInfo *visitInnerClassesAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	InnerClassesAttribute *inner = zalloc(sizeof(InnerClassesAttribute));

	// Inner Classes Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Inner classes count : %d.\n", length);
	inner->classes = createList();

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Inner class %d :\n", idx);
		listAdd(inner->classes, visitInnerClassEntry(classFile, buffer));
	}

	return (AttributeInfo *)inner;
}

AttributeInfo *visitEnclosingMethodAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	EnclosingMethodAttribute *enclose = zalloc(sizeof(EnclosingMethodAttribute));

	// Enclosing Class
	index = bufferNextShort(buffer);
	enclose->enclosing_class = getConstant(classFile, index);

	//Enclosing Method
	index = bufferNextShort(buffer);
	enclose->enclosing_method = getConstant(classFile, index);

	return (AttributeInfo *)enclose;
}

AttributeInfo *visitSyntheticAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ignore_unused(buffer);
	ignore_unused(classFile);
	return (AttributeInfo *)zalloc(sizeof(SyntheticAttribute));
}

AttributeInfo *visitSignatureAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SignatureAttribute *signature = zalloc(sizeof(SignatureAttribute));

	// Signature
	index = bufferNextShort(buffer);
	signature->signature = getConstant(classFile, index);

	return (AttributeInfo *)signature;
}

AttributeInfo *visitSourceFileAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	SourceFileAttribute *file = zalloc(sizeof(SourceFileAttribute));

	// Source File
	index = bufferNextShort(buffer);
	file->source_file = getConstant(classFile, index);

	debug_printf(level2, "Source file name : %s.\n", file->source_file->bytes);

	return (AttributeInfo *)file;
}

AttributeInfo *visitSourceDebugExtensionAttribute(
		ClassFile *classFile, ClassBuffer *buffer, uint32_t length) {
	unsigned int idx;
	SourceDebugExtensionAttribute *source = zalloc(
			sizeof(SourceDebugExtensionAttribute));

	ignore_unused(classFile);

	// Debug Extension
	source->debug_extension = zalloc(sizeof(uint8_t) * length);

	for(idx = 0; idx < length; idx++) {
		// TODO : Added a mass read operation
		source->debug_extension[idx] = bufferNextByte(buffer);
	}

	return (AttributeInfo *)source;
}

LineNumberTableEntry *visitLineNumberTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	LineNumberTableEntry *entry = zalloc(sizeof(LineNumberTableEntry));

	ignore_unused(classFile);
	entry->start_pc = bufferNextShort(buffer);
	entry->line_number = bufferNextShort(buffer);

	return entry;
}

AttributeInfo *visitLineNumberTableAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LineNumberTableAttribute *table = zalloc(sizeof(LineNumberTableAttribute));

	// Line Number Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Line Number Table length : %d.\n", length);
	table->line_number_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(table->line_number_table,
				visitLineNumberTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)table;
}

LocalVariableTableEntry *visitLocalVariableTableEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	LocalVariableTableEntry *entry = zalloc(sizeof(LocalVariableTableEntry));

	entry->start_pc = bufferNextShort(buffer);
	entry->length = bufferNextShort(buffer);

	// Variable Name
	index = bufferNextShort(buffer);
	entry->name = getConstant(classFile, index);
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Descriptor
	index = bufferNextShort(buffer);
	entry->descriptor = getConstant(classFile, index);
	debug_printf(level3, "Local variable descriptor : %s.\n", entry->descriptor->bytes);

	index = bufferNextShort(buffer);
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

AttributeInfo *visitLocalVariableTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTableAttribute *local = zalloc(
			sizeof(LocalVariableTableAttribute));

	// Local Variable Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Local Variable Table length : %d.\n", length);

	local->local_variable_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(local->local_variable_table,
				visitLocalVariableTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)local;
}

LocalVariableTypeTableEntry *visitLocalVariableTypeTableEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	LocalVariableTypeTableEntry *entry = zalloc(sizeof(LocalVariableTypeTableEntry));

	entry->start_pc = bufferNextShort(buffer);
	entry->length = bufferNextShort(buffer);

	// Variable Type Name
	index = bufferNextShort(buffer);
	entry->name = getConstant(classFile, index);
	debug_printf(level3, "Local variable name : %s.\n", entry->name->bytes);

	// Variable Type Signature
	index = bufferNextShort(buffer);
	entry->signature = getConstant(classFile, index);
	debug_printf(level3, "Local variable signature : %s.\n", entry->signature->bytes);

	index = bufferNextShort(buffer);
	entry->index = index;
	debug_printf(level3, "Index : %d.\n", index);

	return entry;
}

AttributeInfo *visitLocalVariableTypeTableAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	LocalVariableTypeTableAttribute *local = zalloc(
			sizeof(LocalVariableTypeTableAttribute));

	// Local Variable Type Table
	length = bufferNextShort(buffer);
	debug_printf(level2, "Local Variable Type Table length : %d.\n", length);

	local->local_variable_type_table = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(local->local_variable_type_table,
				visitLocalVariableTypeTableEntry(classFile, buffer));
	}

	return (AttributeInfo *)local;
}

AttributeInfo *visitDeprecatedAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ignore_unused(buffer);
	ignore_unused(classFile);
	return (AttributeInfo *)zalloc(sizeof(DeprecatedAttribute));
}

ElementValue *visitConstElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = zalloc(sizeof(ElementValue));

	// Constant Value
	index = bufferNextShort(buffer);
	value->value.const_value = getConstant(classFile, index);

	return value;
}

ElementValue *visitEnumElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = zalloc(sizeof(ElementValue));

	// Type Name
	index = bufferNextShort(buffer);
	value->value.enum_const_value.type_name = getConstant(classFile, index);

	// Constant Name
	index = bufferNextShort(buffer);
	value->value.enum_const_value.const_name = getConstant(classFile, index);

	return value;
}

ElementValue *visitClassElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValue *value = zalloc(sizeof(ElementValue));

	// Class Info
	index = bufferNextShort(buffer);
	value->value.class_info = getConstant(classFile, index);

	return value;
}

ElementValue *visitAnnotationElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern AnnotationEntry *visitAnnotationEntry(ClassFile *, ClassBuffer *);

	ElementValue *value = zalloc(sizeof(ElementValue));

	// Annotation Value
	value->value.annotation_value = visitAnnotationEntry(classFile, buffer);

	return value;
}

ElementValue *visitArrayElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	extern ElementValue *visitElementValue(ClassFile *, ClassBuffer *);

	unsigned int idx, length;
	ElementValue *value = zalloc(sizeof(ElementValue));

	// Array Value Table
	length = bufferNextShort(buffer);
	value->value.array_value.values = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(value->value.array_value.values,
				visitElementValue(classFile, buffer));
	}

	return value;
}

ElementValue *visitElementValue(ClassFile *classFile, ClassBuffer *buffer) {
	ElementValue *value;
	uint8_t tag = bufferNextByte(buffer);

	switch(tag) {
		case 'B': case 'C': case 'D':
		case 'F': case 'I': case 'J':
		case 'S': case 'Z': case 's':
			debug_printf(level3, "Constant Element Value.\n");
			value = visitConstElementValue(classFile, buffer);
			break;
		case 'e':
			debug_printf(level3, "Enum Constant Element Value.\n");
			value = visitEnumElementValue(classFile, buffer);
			break;
		case 'c':
			debug_printf(level3, "Class Element Value.\n");
			value = visitClassElementValue(classFile, buffer);
			break;
		case '@':
			debug_printf(level3, "Annotation Element Value.\n");
			value = visitAnnotationElementValue(classFile, buffer);
			break;
		case '[':
			debug_printf(level3, "Array Element Value.\n");
			value = visitArrayElementValue(classFile, buffer);
			break;
	}

	value->tag = tag;
	return value;
}

ElementValuePairsEntry *visitElementValuePairsEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	ElementValuePairsEntry *entry = zalloc(sizeof(ElementValuePairsEntry));

	// Element Name
	index = bufferNextShort(buffer);
	entry->element_name = getConstant(classFile, index);

	// Element Value
	entry->value = visitElementValue(classFile, buffer);

	return entry;
}

AnnotationEntry *visitAnnotationEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	unsigned int idx, length;
	AnnotationEntry *entry = zalloc(sizeof(AnnotationEntry));

	// Annotation Entry Type
	index = bufferNextShort(buffer);
	entry->type = getConstant(classFile, index);

	// Element Value Pairs Table
	length = bufferNextShort(buffer);
	entry->element_value_pairs = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(entry->element_value_pairs,
				visitElementValuePairsEntry(classFile, buffer));
	}

	return entry;
}

AttributeInfo *visitRuntimeAnnotationsAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	RuntimeAnnotationsAttribute *annot = zalloc(sizeof(
			RuntimeAnnotationsAttribute));

	// Annotations Table
	length = bufferNextShort(buffer);
	annot->annotations = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(annot->annotations, visitAnnotationEntry(classFile, buffer));
	}

	return (AttributeInfo *)annot;
}

ParameterAnnotationsEntry *visitParameterAnnotationsEntry(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	ParameterAnnotationsEntry *entry = zalloc(sizeof(ParameterAnnotationsEntry));

	// Annotations Table
	length = bufferNextShort(buffer);
	entry->annotations = createList();

	for(idx = 0; idx > length; idx++) {
		listAdd(entry->annotations, visitAnnotationEntry(classFile, buffer));
	}

	return entry;
}

AttributeInfo *visitRuntimeParameterAnnotationsAttribute(
		ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	RuntimeParameterAnnotationsAttribute *annot = zalloc(sizeof(
			RuntimeParameterAnnotationsAttribute));

	// Parameter Annotations Table
	length = bufferNextShort(buffer);
	annot->parameter_annotations = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(annot->parameter_annotations,
				visitParameterAnnotationsEntry(classFile, buffer));
	}

	return (AttributeInfo *)annot;
}

AttributeInfo *visitAnnotationDefaultAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AnnotationDefaultAttribute *annot = zalloc(sizeof(AnnotationDefaultAttribute));

	// Default Value
	annot->default_value = visitElementValue(classFile, buffer);

	return (AttributeInfo *)annot;
}

BootstrapMethodEntry *visitBootstrapMethodEntry(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodEntry *entry = zalloc(sizeof(BootstrapMethodEntry));

	// Bootstrap Method Parameters Table
	length = bufferNextShort(buffer);
	entry->bootstrap_arguments = createList();

	for(idx = 0; idx < length; idx++) {
		uint16_t index = bufferNextShort(buffer);
		listAdd(entry->bootstrap_arguments, getConstant(classFile, index));
	}

	return entry;
}

AttributeInfo *visitBootstrapMethodsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	unsigned int idx, length;
	BootstrapMethodsAttribute *bootstrap = zalloc(sizeof(BootstrapMethodsAttribute));

	// Bootstrap Method Table
	length = bufferNextShort(buffer);
	bootstrap->bootstrap_methods = createList();

	for(idx = 0; idx < length; idx++) {
		listAdd(bootstrap->bootstrap_methods,
				visitBootstrapMethodEntry(classFile, buffer));
	}

	return (AttributeInfo *)bootstrap;
}

AttributeInfo *visitAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AttributeInfo *info;

	uint16_t name_index = bufferNextShort(buffer);
	uint32_t attribute_length = bufferNextInt(buffer);
	ConstantUtf8Info *name = getConstant(classFile, name_index);
	unsigned initpos = bufferPos(buffer);

	if(name == NULL) {
		fprintf(stderr, "Error : Attribute with no name entry!\n");
		exit(EXIT_FAILURE);
	}

	debug_printf(level1, "Visiting Attribute type : %s.\n", name->bytes);

	// Constant Value Attribute
	if(!strcmp("ConstantValue", (char *)name->bytes)) {
		info = visitConstantValueAttribute(classFile, buffer);
	} else
	// Code Attribute
	if(!strcmp("Code", (char *)name->bytes)) {
		info = visitCodeAttribute(classFile, buffer);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", (char *)name->bytes)) {
		info = visitStackMapTableAttribute(classFile, buffer);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", (char *)name->bytes)) {
		info = visitExceptionsAttribute(classFile, buffer);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", (char *)name->bytes)) {
		info = visitInnerClassesAttribute(classFile, buffer);
	} else
	// Enclosing Method Attribute
	if(!strcmp("EnclosingMethod", (char *)name->bytes)) {
		info = visitEnclosingMethodAttribute(classFile, buffer);
	} else
	// Synthetic Attribute
	if(!strcmp("Synthetic", (char *)name->bytes)) {
		info = visitSyntheticAttribute(classFile, buffer);
	} else
	// Signature Attribute
	if(!strcmp("Signature", (char *)name->bytes)) {
		info = visitSignatureAttribute(classFile, buffer);
	} else
	// Source File Attribute
	if(!strcmp("SourceFile", (char *)name->bytes)) {
		info = visitSourceFileAttribute(classFile, buffer);
	} else
	// Source Debug Extension Attribute
	if(!strcmp("SourceDebugExtension", (char *)name->bytes)) {
		info = visitSourceDebugExtensionAttribute(
				classFile, buffer, attribute_length);
	} else
	// Line Number Table Attribute
	if(!strcmp("LineNumberTable", (char *)name->bytes)) {
		info = visitLineNumberTableAttribute(classFile, buffer);
	} else
	// Local Variable Table Attribute
	if(!strcmp("LocalVariableTable", (char *)name->bytes)) {
		info = visitLocalVariableTableAttribute(classFile, buffer);
	} else
	// Local Variable Type Table Attribute
	if(!strcmp("LocalVariableTypeTable", (char *)name->bytes)) {
		info = visitLocalVariableTypeTableAttribute(classFile, buffer);
	} else
	// Deprecated Attribute
	if(!strcmp("Deprecated", (char *)name->bytes)) {
		info = visitDeprecatedAttribute(classFile, buffer);
	} else
	// Runtime Visible Annotations Attribute
	if(!strcmp("RuntimeVisibleAnnotations", (char *)name->bytes)) {
		info = visitRuntimeAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Invisible Annotations Attribute
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)name->bytes)) {
		info = visitRuntimeAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)name->bytes)) {
		info = visitRuntimeParameterAnnotationsAttribute(classFile, buffer);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)name->bytes)) {
		info = visitRuntimeParameterAnnotationsAttribute(classFile, buffer);
	} else
	// Annotation Default Attribute
	if(!strcmp("AnnotationDefault", (char *)name->bytes)) {
		info = visitAnnotationDefaultAttribute(classFile, buffer);
	} else
	// Bootstrap Methods Attribute
	if(!strcmp("BootstrapMethods", (char *)name->bytes)) {
		info = visitBootstrapMethodsAttribute(classFile, buffer);
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
