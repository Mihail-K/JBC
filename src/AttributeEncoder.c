
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

int encodeConstantValueAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	ConstantValueAttribute *constant = static_cast(ConstantValueAttribute *, info);
	buildNextShort(builder, constant->constant_value->index);

	ignore_unused(classFile);
	return 0;
}

int encodeExceptionTableEntry(
		ClassFile *classFile, ClassBuilder *builder, ExceptionTableEntry *entry) {
	buildNextShort(builder, entry->start_pc);
	buildNextShort(builder, entry->end_pc);
	buildNextShort(builder, entry->handler_pc);
	buildNextShort(builder, entry->catch_type);

	ignore_unused(classFile);
	return 0;
}

int encodeCodeAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	CodeAttribute *code = static_cast(CodeAttribute *, info);

	// Maximums
	buildNextShort(builder, code->max_stack);
	buildNextShort(builder, code->max_locals);

	// Code
	buildNextInt(builder, code->code_length);
	for(idx = 0; idx < code->code_length; idx++) {
		buildNextByte(builder, code->code[idx]);
	}

	// Exception Table
	length = listSize(code->exception_table);
	buildNextShort(builder, length);
	for(idx = 0; idx < length; idx++) {
		encodeExceptionTableEntry(classFile, builder, listGet(
				code->exception_table, idx));
	}

	// Attribute Table
	length = listSize(code->attributes);
	buildNextShort(builder, length);
	for(idx = 0; idx < length; idx++) {
		encodeAttribute(classFile, builder, listGet(
				code->attributes, idx));
	}

	return 0;
}

int encodeVerificationTypeInfo(
		ClassFile *classFile, ClassBuilder *builder, VerificationTypeInfo *info) {
	buildNextByte(builder, info->tag);
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
			buildNextShort(builder, info->object_variable_info.object->index);
			break;
		case 8:
			debug_printf(level3, "Uninitialized variable info.\n");
			buildNextShort(builder, info->uninitialized_variable_info.offset);
			break;
		default:
			fprintf(stderr, "Unknown verification type (ID : %d)!\n", info->tag);
			exit(EXIT_FAILURE);
	}

	ignore_unused(classFile);
	return 0;
}

int encodeStackMapOffFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapOffFrame *offFrame = static_cast(StackMapOffFrame *, frame);
	buildNextShort(builder, offFrame->offset_delta);

	ignore_unused(classFile);
	return 0;
}

int encodeStackMapItemFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapItemFrame *itemFrame = static_cast(StackMapItemFrame *, frame);
	encodeVerificationTypeInfo(classFile, builder, itemFrame->stack);
	return 0;
}

int encodeStackMapExtFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapExtFrame *extFrame = static_cast(StackMapExtFrame *, frame);

	buildNextShort(builder, extFrame->offset_delta);
	encodeVerificationTypeInfo(classFile, builder, extFrame->stack);

	return 0;
}

int encodeStackMapListFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx;
	StackMapListFrame *listFrame = static_cast(StackMapListFrame *, frame);

	buildNextShort(builder, listFrame->offset_delta);
	for(idx = 0; idx < (listFrame->tag - 251u); idx++) {
		encodeVerificationTypeInfo(classFile, builder, listFrame->stack[idx]);
	}

	return 0;
}

int encodeStackMapFullFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx, length;
	StackMapFullFrame *fullFrame = static_cast(StackMapFullFrame *, frame);

	buildNextShort(builder, fullFrame->offset_delta);

	// Stack Frame Locals
	length = listSize(fullFrame->locals);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeVerificationTypeInfo(classFile, builder, listGet(
				fullFrame->locals, idx));
	}

	// Stack Frame Items
	length = listSize(fullFrame->stack);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeVerificationTypeInfo(classFile, builder, listGet(
				fullFrame->stack, idx));
	}

	return 0;
}

int encodeStackMapFrame(
		ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	buildNextByte(builder, frame->tag);

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
	StackMapTableAttribute *table = static_cast(StackMapTableAttribute *, info);

	length = listSize(table->entries);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeStackMapFrame(classFile, builder, listGet(
				table->entries, idx));
	}

	return 0;
}

int encodeExceptionsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	ExceptionsAttribute *except = static_cast(ExceptionsAttribute *, info);

	ignore_unused(classFile);
	length = listSize(except->exception_table);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		ConstantInfo *constant = listGet(except->exception_table, idx);
		buildNextShort(builder, constant->index);
	}

	return 0;
}

int encodeInnerClassEntry(
		ClassFile *classFile, ClassBuilder *builder, InnerClassEntry *entry) {
	buildNextShort(builder, entry->inner_class_info->index);
	buildNextShort(builder, entry->outer_class_info->index);
	buildNextShort(builder, entry->inner_class_name->index);
	buildNextShort(builder, entry->inner_class_access_flags);

	ignore_unused(classFile);
	return 0;
}

int encodeInnerClassesAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	InnerClassesAttribute *inner = static_cast(InnerClassesAttribute *, info);

	length = listSize(inner->classes);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeInnerClassEntry(classFile, builder, listGet(
				inner->classes, idx));
	}

	return 0;
}

int encodeEnclosingMethodAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	EnclosingMethodAttribute *enclose = static_cast(EnclosingMethodAttribute *, info);

	buildNextShort(builder, enclose->enclosing_class->index);
	buildNextShort(builder, enclose->enclosing_method->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSignatureAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	SignatureAttribute *signature = static_cast(SignatureAttribute *, info);

	buildNextShort(builder, signature->signature->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSourceFileAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	SourceFileAttribute *source = static_cast(SourceFileAttribute *, info);

	buildNextShort(builder, source->source_file->index);

	ignore_unused(classFile);
	return 0;
}

int encodeSourceDebugExtensionAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx;
	SourceDebugExtensionAttribute *debug = static_cast(SourceDebugExtensionAttribute *, info);

	for(idx = 0; idx < debug->attribute_length; idx++) {
		buildNextByte(builder, debug->debug_extension[idx]);
	}

	ignore_unused(classFile);
	return 0;
}

int encodeLineNumberTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LineNumberTableEntry *entry) {
	buildNextShort(builder, entry->start_pc);
	buildNextShort(builder, entry->line_number);

	ignore_unused(classFile);
	return 0;
}

int encodeLineNumberTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LineNumberTableAttribute *table = static_cast(LineNumberTableAttribute *, info);

	length = listSize(table->line_number_table);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeLineNumberTableEntry(classFile, builder, listGet(
				table->line_number_table, idx));
	}

	return 0;
}

int encodeLocalVariableTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LocalVariableTableEntry *entry) {
	buildNextShort(builder, entry->start_pc);
	buildNextShort(builder, entry->length);

	buildNextShort(builder, entry->name->index);
	buildNextShort(builder, entry->descriptor->index);

	buildNextShort(builder, entry->index);

	ignore_unused(classFile);
	return 0;
}

int encodeLocalVariableTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LocalVariableTableAttribute *table = static_cast(
			LocalVariableTableAttribute *, info);

	length = listSize(table->local_variable_table);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeLocalVariableTableEntry(classFile, builder, listGet(
				table->local_variable_table, idx));
	}

	return 0;
}

int encodeLocalVariableTypeTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LocalVariableTypeTableEntry *entry) {
	buildNextShort(builder, entry->start_pc);
	buildNextShort(builder, entry->length);

	buildNextShort(builder, entry->name->index);
	buildNextShort(builder, entry->signature->index);

	buildNextShort(builder, entry->index);

	ignore_unused(classFile);
	return 0;
}

int encodeLocalVariableTypeTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	LocalVariableTypeTableAttribute *table = static_cast(
			LocalVariableTypeTableAttribute *, info);

	length = listSize(table->local_variable_type_table);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeLocalVariableTypeTableEntry(classFile, builder, listGet(
				table->local_variable_type_table, idx));
	}

	return 0;
}

int encodeConstElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	buildNextShort(builder, value->value.const_value->index);

	ignore_unused(classFile);
	return 0;
}

int encodeEnumElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	buildNextShort(builder, value->value.enum_const_value.type_name->index);
	buildNextShort(builder, value->value.enum_const_value.const_name->index);

	ignore_unused(classFile);
	return 0;
}

int encodeClassElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	buildNextShort(builder, value->value.class_info->index);

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
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeElementValue(classFile, builder, listGet(
				value->value.array_values, idx));
	}

	ignore_unused(classFile);
	return 0;
}

int encodeElementValue(
		ClassFile *classFile, ClassBuilder *builder, ElementValue *value) {
	buildNextByte(builder, value->tag);

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
	buildNextShort(builder, entry->element_name->index);
	encodeElementValue(classFile, builder, entry->value);

	return 0;
}

int encodeAnnotationEntry(
		ClassFile *classFile, ClassBuilder *builder, AnnotationEntry *entry) {
	unsigned int idx, length;

	buildNextShort(builder, entry->type->index);
	length = listSize(entry->element_value_pairs);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeElementValuePairsEntry(classFile, builder, listGet(
				entry->element_value_pairs, idx));
	}

	return 0;
}

int encodeRuntimeAnnotationsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	RuntimeAnnotationsAttribute *annot = static_cast(RuntimeAnnotationsAttribute *, info);

	length = listSize(annot->annotations);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeAnnotationEntry(classFile, builder, listGet(
				annot->annotations, idx));
	}

	return 0;
}

int encodeParameterAnnotationsEntry(
		ClassFile *classFile, ClassBuilder *builder, ParameterAnnotationsEntry *entry) {
	unsigned int idx, length;

	length = listSize(entry->annotations);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeAnnotationEntry(classFile, builder, listGet(
				entry->annotations, idx));
	}

	return 0;
}

int encodeRuntimeParameterAnnotationsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	RuntimeParameterAnnotationsAttribute *annot = static_cast(
			RuntimeParameterAnnotationsAttribute *, info);

	length = listSize(annot->parameter_annotations);
	buildNextByte(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeParameterAnnotationsEntry(classFile, builder, listGet(
				annot->parameter_annotations, idx));
	}

	return 0;
}

int encodeAnnotationDefaultAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	AnnotationDefaultAttribute *annot = static_cast(AnnotationDefaultAttribute *, info);
	encodeElementValue(classFile, builder, annot->default_value);

	return 0;
}

int encodeBootstrapMethodEntry(
		ClassFile *classFile, ClassBuilder *builder, BootstrapMethodEntry *entry) {
	unsigned int idx, length;

	buildNextShort(builder, entry->bootstrap_method_ref->index);
	length = listSize(entry->bootstrap_arguments);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		ConstantInfo *info = listGet(entry->bootstrap_arguments, idx);
		buildNextShort(builder, info->index);
	}

	ignore_unused(classFile);
	return 0;
}

int encodeBootstrapMethodsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	BootstrapMethodsAttribute *methods = static_cast(BootstrapMethodsAttribute *, info);

	length = listSize(methods->bootstrap_methods);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeBootstrapMethodEntry(classFile, builder, listGet(
				methods->bootstrap_methods, idx));
	}

	return 0;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	int result;
	unsigned long int initpos;

	buildNextShort(builder, info->name->index);
	buildNextInt(builder, info->attribute_length);

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

