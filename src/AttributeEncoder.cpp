
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

ConstantValueAttribute *ConstantValueAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(constant_value->index);
	return this;
}

ExceptionTableEntry *ExceptionTableEntry
		::EncodeEntry(ClassBuilder *builder) {
	builder->NextShort(start_pc);
	builder->NextShort(end_pc);
	builder->NextShort(handler_pc);
	builder->NextShort(catch_type);

	return this;
}

CodeAttribute *CodeAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	// Maximums
	builder->NextShort(max_stack);
	builder->NextShort(max_locals);

	// Code
	builder->NextInt(code_length);
	for(unsigned idx = 0; idx < code_length; idx++) {
		builder->NextByte(code[idx]);
	}

	// Exception Table
	length = exception_table.size();
	builder->NextShort(length);
	for(unsigned idx = 0; idx < length; idx++) {
		exception_table[idx]->EncodeEntry(builder);
	}

	// Attribute Table
	length = attributes.size();
	builder->NextShort(length);
	for(unsigned idx = 0; idx < length; idx++) {
		attributes[idx]->EncodeAttribute(builder, classFile);
	}

	return this;
}

StackMapTableAttribute *StackMapTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = entries.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeStackMapFrame(builder, classFile, entries[idx]);
	}

	return this;
}

ExceptionsAttribute *ExceptionsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	uint16_t length;

	length = exception_table.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		ConstantInfo *constant = exception_table[idx];
		builder->NextShort(constant->index);
	}

	return this;
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

InnerClassesAttribute *InnerClassesAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(classes);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeInnerClassEntry(classFile, builder, static_cast<
				InnerClassEntry *>(listGet(classes, idx)));
	}

	return this;
}

EnclosingMethodAttribute *EnclosingMethodAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(enclosing_class->index);
	builder->NextShort(enclosing_method->index);

	return this;
}

SignatureAttribute *SignatureAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(signature->index);

	return this;
}

SourceFileAttribute *SourceFileAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(source_file->index);

	return this;
}

SourceDebugExtensionAttribute *SourceDebugExtensionAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	for(unsigned idx = 0; idx < attribute_length; idx++) {
		builder->NextByte(debug_extension[idx]);
	}

	return this;
}

int encodeLineNumberTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LineNumberTableEntry *entry) {
	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->line_number);

	ignore_unused(classFile);
	return 0;
}

LineNumberTableAttribute *LineNumberTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(line_number_table);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeLineNumberTableEntry(classFile, builder, static_cast<
				LineNumberTableEntry *>(listGet(line_number_table, idx)));
	}

	return this;
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

LocalVariableTableAttribute *LocalVariableTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(local_variable_table);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeLocalVariableTableEntry(classFile, builder, static_cast<
				LocalVariableTableEntry *>(listGet(local_variable_table, idx)));
	}

	return this;
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

LocalVariableTypeTableAttribute *LocalVariableTypeTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(local_variable_type_table);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeLocalVariableTypeTableEntry(classFile, builder, static_cast<
				LocalVariableTypeTableEntry *>(listGet(
						local_variable_type_table, idx)));
	}

	return this;
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

RuntimeAnnotationsAttribute *RuntimeAnnotationsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(annotations);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeAnnotationEntry(classFile, builder, static_cast<
				AnnotationEntry *>(listGet(annotations, idx)));
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

RuntimeParameterAnnotationsAttribute *RuntimeParameterAnnotationsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = listSize(parameter_annotations);
	builder->NextByte(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeParameterAnnotationsEntry(classFile, builder, static_cast<
				ParameterAnnotationsEntry *>(listGet(parameter_annotations, idx)));
	}

	return this;
}

AnnotationDefaultAttribute *AnnotationDefaultAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	encodeElementValue(classFile, builder, default_value);

	return this;
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

BootstrapMethodsAttribute *BootstrapMethodsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	length = bootstrap_methods.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeBootstrapMethodEntry(classFile, builder, bootstrap_methods[idx]);
	}

	return 0;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	builder->NextShort(info->name->index);
	builder->NextInt(info->attribute_length);
	info->EncodeAttribute(builder, classFile);

	return 0;
}

