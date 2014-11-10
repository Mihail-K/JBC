
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

int encodeConstantValueAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	ConstantValueAttribute *constant = (ConstantValueAttribute *)info;
	builder->NextShort(constant->constant_value->index);

	ignore_unused(classFile);
	return 0;
}

int encodeExceptionTableEntry(
		ClassFile *classFile, ClassBuilder *builder, ExceptionTableEntry *entry) {
	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->end_pc);
	builder->NextShort(entry->handler_pc);
	builder->NextShort(entry->catch_type);

	ignore_unused(classFile);
	return 0;
}

int encodeCodeAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	CodeAttribute *code = (CodeAttribute *)info;

	// Maximums
	builder->NextShort(code->max_stack);
	builder->NextShort(code->max_locals);

	// Code
	builder->NextInt(code->code_length);
	for(idx = 0; idx < code->code_length; idx++) {
		builder->NextByte(code->code[idx]);
	}

	// Exception Table
	length = listSize(code->exception_table);
	builder->NextShort(length);
	for(idx = 0; idx < length; idx++) {
		encodeExceptionTableEntry(classFile, builder, static_cast(
				ExceptionTableEntry *, listGet(code->exception_table, idx)));
	}

	// Attribute Table
	length = listSize(code->attributes);
	builder->NextShort(length);
	for(idx = 0; idx < length; idx++) {
		encodeAttribute(classFile, builder, static_cast(
				AttributeInfo *, listGet(code->attributes, idx)));
	}

	return 0;
}

int encodeStackMapFrame(
		ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	builder->NextByte(frame->tag);

	// Stack Map Same Frame
	if(frame->tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		return 0;
	} else
	// Stack Map Same Locals 1
	if(frame->tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		return encodeStackMapItemFrame(classFile, builder, frame);
	} else
	// Reserved Values
	if(frame->tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", frame->tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(frame->tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		return encodeStackMapExtFrame(classFile, builder, frame);
	} else
	// Stack Map Chop Frame
	if(frame->tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		return encodeStackMapOffFrame(classFile, builder, frame);
	} else
	// Stack Map Same Frame Extended
	if(frame->tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		return encodeStackMapOffFrame(classFile, builder, frame);
	} else
	// Stack Map Append Frame
	if(frame->tag <= 254) {
		debug_printf(level3, "Stack Map append frame.\n");
		return encodeStackMapListFrame(classFile, builder, frame);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		return encodeStackMapFullFrame(classFile, builder, frame);
	}
}

int encodeStackMapTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	StackMapTableAttribute *table = (StackMapTableAttribute *)info;

	length = listSize(table->entries);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeStackMapFrame(classFile, builder, static_cast<
				StackMapFrame *>(listGet(table->entries, idx)));
	}

	return 0;
}

int encodeExceptionsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	ExceptionsAttribute *except = (ExceptionsAttribute *)info;

	ignore_unused(classFile);
	length = listSize(except->exception_table);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		ConstantInfo *constant = static_cast<ConstantInfo *>(listGet(
				except->exception_table, idx));
		builder->NextShort(constant->index);
	}

	return 0;
}

int encodeInnerClassEntry(
		ClassFile *classFile, ClassBuilder *builder, InnerClassEntry *entry) {
	builder->NextShort(entry->inner_class_info->index);
	builder->NextShort(entry->outer_class_info->index);
	builder->NextShort(entry->inner_class_name->index);
	builder->NextShort(entry->inner_class_access_flags);

	ignore_unused(classFile);
	return 0;
}

int encodeInnerClassesAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	InnerClassesAttribute *inner = (InnerClassesAttribute *)info;

	length = listSize(inner->classes);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeInnerClassEntry(classFile, builder, static_cast<
				InnerClassEntry *>(listGet(inner->classes, idx)));
	}

	return 0;
}

int encodeEnclosingMethodAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	EnclosingMethodAttribute *enclose = (EnclosingMethodAttribute *)info;

	builder->NextShort(enclose->enclosing_class->index);
	builder->NextShort(enclose->enclosing_method->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSignatureAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	SignatureAttribute *signature = (SignatureAttribute *)info;

	builder->NextShort(signature->signature->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSourceFileAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	SourceFileAttribute *source = (SourceFileAttribute *)info;

	builder->NextShort(source->source_file->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSourceDebugExtensionAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx;
	SourceDebugExtensionAttribute *debug = (SourceDebugExtensionAttribute *)info;

	for(idx = 0; idx < debug->attribute_length; idx++) {
		builder->NextByte(debug->debug_extension[idx]);
	}

	ignore_unused(classFile);
	return 0;
}

int encodeLineNumberTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LineNumberTableEntry *entry) {
	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->line_number);

	ignore_unused(classFile);
	return 0;
}

int encodeLineNumberTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LineNumberTableAttribute *table = (LineNumberTableAttribute *)info;

	length = listSize(table->line_number_table);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeLineNumberTableEntry(classFile, builder, static_cast<
				LineNumberTableEntry *>(listGet(table->line_number_table, idx)));
	}

	return 0;
}

int encodeLocalVariableTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LocalVariableTableEntry *entry) {
	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->length);

	builder->NextShort(entry->name->index);
	builder->NextShort(entry->descriptor->index);

	builder->NextShort(entry->index);

	ignore_unused(classFile);
	return 0;
}

int encodeLocalVariableTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LocalVariableTableAttribute *table =
			(LocalVariableTableAttribute *)info;

	length = listSize(table->local_variable_table);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeLocalVariableTableEntry(classFile, builder, static_cast<
				LocalVariableTableEntry *>(listGet(table->local_variable_table, idx)));
	}

	return 0;
}

int encodeLocalVariableTypeTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LocalVariableTypeTableEntry *entry) {
	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->length);

	builder->NextShort(entry->name->index);
	builder->NextShort(entry->signature->index);

	builder->NextShort(entry->index);

	ignore_unused(classFile);
	return 0;
}

int encodeLocalVariableTypeTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LocalVariableTypeTableAttribute *table =
			(LocalVariableTypeTableAttribute *)info;

	length = listSize(table->local_variable_type_table);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeLocalVariableTypeTableEntry(classFile, builder, static_cast<
				LocalVariableTypeTableEntry *>(listGet(
						table->local_variable_type_table, idx)));
	}

	return 0;
}

int encodeConstElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	builder->NextShort(value->value.const_value->index);

	ignore_unused(classFile);
	return 0;
}

int encodeEnumElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	builder->NextShort(value->value.enum_const_value.type_name->index);
	builder->NextShort(value->value.enum_const_value.const_name->index);

	ignore_unused(classFile);
	return 0;
}

int encodeClassElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	builder->NextShort(value->value.class_info->index);

	ignore_unused(classFile);
	return 0;
}

int encodeAnnotationElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	extern int encodeAnnotationEntry(ClassFile *, ClassBuilder *, AnnotationEntry *);
	encodeAnnotationEntry(classFile, builder, value->value.annotation_value);

	ignore_unused(classFile);
	return 0;
}

int encodeArrayElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	extern int encodeElementValue(ClassFile *, ClassBuilder *, ElementValue *);
	unsigned int idx, length;

	length = listSize(value->value.array_values);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeElementValue(classFile, builder, static_cast<
				ElementValue *>(listGet(value->value.array_values, idx)));
	}

	ignore_unused(classFile);
	return 0;
}

int encodeElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	builder->NextByte(value->tag);

	switch(value->tag) {
		case 'B': case 'C': case 'D':
		case 'F': case 'I': case 'J':
		case 'S': case 'Z': case 's':
			debug_printf(level3, "Constant Element Value.\n");
			return encodeConstElementValue(classFile, builder, value);
		case 'e':
			debug_printf(level3, "Enum Constant Element Value.\n");
			return encodeEnumElementValue(classFile, builder, value);
		case 'c':
			debug_printf(level3, "Class Element Value.\n");
			return encodeClassElementValue(classFile, builder, value);
		case '@':
			debug_printf(level3, "Annotation Element Value.\n");
			return encodeAnnotationElementValue(classFile, builder, value);
		case '[':
			debug_printf(level3, "Array Element Value.\n");
			return encodeArrayElementValue(classFile, builder, value);
		default:
			fprintf(stderr, "Invalid Element Value Type : %c.\n", value->tag);
			return -1;
	}

	return 0;
}

int encodeElementValuePairsEntry(
		ClassFile *classFile, ClassBuilder *builder, ElementValuePairsEntry *entry) {
	builder->NextShort(entry->element_name->index);
	encodeElementValue(classFile, builder, entry->value);

	return 0;
}

int encodeAnnotationEntry(
		ClassFile *classFile, ClassBuilder *builder, AnnotationEntry *entry) {
	unsigned int idx, length;

	builder->NextShort(entry->type->index);
	length = listSize(entry->element_value_pairs);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeElementValuePairsEntry(classFile, builder, static_cast<
				ElementValuePairsEntry *>(listGet(entry->element_value_pairs, idx)));
	}

	return 0;
}

int encodeRuntimeAnnotationsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	RuntimeAnnotationsAttribute *annot = (RuntimeAnnotationsAttribute *)info;

	length = listSize(annot->annotations);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeAnnotationEntry(classFile, builder, static_cast<
				AnnotationEntry *>(listGet(annot->annotations, idx)));
	}

	return 0;
}

int encodeParameterAnnotationsEntry(
		ClassFile *classFile, ClassBuilder *builder, ParameterAnnotationsEntry *entry) {
	unsigned int idx, length;

	length = listSize(entry->annotations);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeAnnotationEntry(classFile, builder, static_cast<
				AnnotationEntry *>(listGet(entry->annotations, idx)));
	}

	return 0;
}

int encodeRuntimeParameterAnnotationsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	RuntimeParameterAnnotationsAttribute *annot =
			(RuntimeParameterAnnotationsAttribute *)info;

	length = listSize(annot->parameter_annotations);
	builder->NextByte(length);

	for(idx = 0; idx < length; idx++) {
		encodeParameterAnnotationsEntry(classFile, builder, static_cast<
				ParameterAnnotationsEntry *>(listGet(annot->parameter_annotations, idx)));
	}

	return 0;
}

int encodeAnnotationDefaultAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	AnnotationDefaultAttribute *annot = (AnnotationDefaultAttribute *)info;
	encodeElementValue(classFile, builder, annot->default_value);

	return 0;
}

int encodeBootstrapMethodEntry(
		ClassFile *classFile, ClassBuilder *builder, BootstrapMethodEntry *entry) {
	unsigned int idx, length;

	builder->NextShort(entry->bootstrap_method_ref->index);
	length = listSize(entry->bootstrap_arguments);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		ConstantInfo *info = static_cast<ConstantInfo *>(
				listGet(entry->bootstrap_arguments, idx));
		builder->NextShort(info->index);
	}

	ignore_unused(classFile);
	return 0;
}

int encodeBootstrapMethodsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	BootstrapMethodsAttribute *methods = (BootstrapMethodsAttribute *)info;

	length = listSize(methods->bootstrap_methods);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		encodeBootstrapMethodEntry(classFile, builder, static_cast<
				BootstrapMethodEntry *>(listGet(methods->bootstrap_methods, idx)));
	}

	return 0;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	int result;
	unsigned long int initpos;

	builder->NextShort(info->name->index);
	builder->NextInt(info->attribute_length);

	initpos = builderPos(builder);

	if(!strcmp("ConstantValue", (char *)info->name->bytes)) {
		result = encodeConstantValueAttribute(classFile, builder, info);
	} else
	if(!strcmp("Code", (char *)info->name->bytes)) {
		result = encodeCodeAttribute(classFile, builder, info);
	} else
	if(!strcmp("StackMapTable", (char *)info->name->bytes)) {
		result = encodeStackMapTableAttribute(classFile, builder, info);
	} else
	if(!strcmp("Exceptions", (char *)info->name->bytes)) {
		result = encodeExceptionsAttribute(classFile, builder, info);
	} else
	if(!strcmp("InnerClasses", (char *)info->name->bytes)) {
		result = encodeInnerClassesAttribute(classFile, builder, info);
	} else
	if(!strcmp("EnclosingMethod", (char *)info->name->bytes)) {
		result = encodeEnclosingMethodAttribute(classFile, builder, info);
	} else
	if(!strcmp("Synthetic", (char *)info->name->bytes)) {
		result = 0;
	} else
	if(!strcmp("Signature", (char *)info->name->bytes)) {
		result = encodeSignatureAttribute(classFile, builder, info);
	} else
	if(!strcmp("SourceFile", (char *)info->name->bytes)) {
		result = encodeSourceFileAttribute(classFile, builder, info);
	} else
	if(!strcmp("SourceDebugExtension", (char *)info->name->bytes)) {
		result = encodeSourceDebugExtensionAttribute(classFile, builder, info);
	} else
	if(!strcmp("LineNumberTable", (char *)info->name->bytes)) {
		result = encodeLineNumberTableAttribute(classFile, builder, info);
	} else
	if(!strcmp("LocalVariableTable", (char *)info->name->bytes)) {
		result = encodeLocalVariableTableAttribute(classFile, builder, info);
	} else
	if(!strcmp("LocalVariableTypeTable", (char *)info->name->bytes)) {
		result = encodeLocalVariableTypeTableAttribute(classFile, builder, info);
	} else
	if(!strcmp("Deprecated", (char *)info->name->bytes)) {
		result = 0;
	} else
	if(!strcmp("RuntimeVisibleAnnotations", (char *)info->name->bytes)) {
		result = encodeRuntimeAnnotationsAttribute(classFile, builder, info);
	} else
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)info->name->bytes)) {
		result = encodeRuntimeAnnotationsAttribute(classFile, builder, info);
	} else
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)info->name->bytes)) {
		result = encodeRuntimeParameterAnnotationsAttribute(classFile, builder, info);
	} else
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)info->name->bytes)) {
		result = encodeRuntimeParameterAnnotationsAttribute(classFile, builder, info);
	} else
	if(!strcmp("AnnotationDefault", (char *)info->name->bytes)) {
		result = encodeAnnotationDefaultAttribute(classFile, builder, info);
	} else
	if(!strcmp("BootstrapMethods", (char *)info->name->bytes)) {
		result = encodeBootstrapMethodsAttribute(classFile, builder, info);
	} else {
		return -1;
	}

	if(builderPos(builder) - initpos != info->attribute_length) {
		fprintf(stderr, "Attribute length mismatch!\n");
		exit(EXIT_FAILURE);
	}

	debug_printf(level1, "Finished Attribute : %s.\n", info->name->bytes);
	return result;
}

