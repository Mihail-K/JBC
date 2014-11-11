
# include <string.h>

# include "Debug.h"
# include "Defines.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

ConstantValueAttribute *ConstantValueAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Constant Value Attribute.\n");

	builder->NextShort(constant_value->index);
	return this;
}

ExceptionTableEntry *ExceptionTableEntry
		::EncodeEntry(ClassBuilder *builder) {
	debug_printf(level3, "Encoding Exception Table Entry.\n");

	builder->NextShort(start_pc);
	builder->NextShort(end_pc);
	builder->NextShort(handler_pc);
	builder->NextShort(catch_type);

	return this;
}

CodeAttribute *CodeAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Code Attribute.\n");

	// Maximums
	builder->NextShort(max_stack);
	builder->NextShort(max_locals);

	// Code
	builder->NextInt(code_length);
	debug_printf(level3, "Code length : %u.\n", code_length);
	for(unsigned idx = 0; idx < code_length; idx++) {
		builder->NextByte(code[idx]);
	}

	// Exception Table
	length = exception_table.size();
	builder->NextShort(length);
	debug_printf(level3, "Code Exception table length : %hu.\n", length);
	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Exception table entry %u :\n", idx);
		exception_table[idx]->EncodeEntry(builder);
	}

	// Attribute Table
	length = attributes.size();
	builder->NextShort(length);
	debug_printf(level3, "Code Attributes count : %hu.\n", length);
	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %u :\n", idx);
		encodeAttribute(classFile, builder, attributes[idx]);
	}

	return this;
}

StackMapTableAttribute *StackMapTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Stack Map Table Attribute.\n");

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

	debug_printf(level3, "Encoding Exceptions Attribute.\n");

	length = exception_table.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		ConstantInfo *constant = exception_table[idx];
		builder->NextShort(constant->index);
	}

	return this;
}

InnerClassEntry *InnerClassEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Inner Class Entry.\n");

	builder->NextShort(inner_class_info == NULL ? 0 : inner_class_info->index);
	builder->NextShort(outer_class_info == NULL ? 0 : outer_class_info->index);
	builder->NextShort(inner_class_name == NULL ? 0 : inner_class_name->index);
	builder->NextShort(inner_class_access_flags);

	return this;
}

InnerClassesAttribute *InnerClassesAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Inner Classes Attribute.\n");

	length = classes.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		classes[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

EnclosingMethodAttribute *EnclosingMethodAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Enclosing Method Attribute.\n");

	builder->NextShort(enclosing_class->index);
	builder->NextShort(enclosing_method->index);

	return this;
}

SignatureAttribute *SignatureAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Signature Attribute.\n");

	builder->NextShort(signature->index);

	return this;
}

SourceFileAttribute *SourceFileAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Souce File Attribute.\n");

	if(source_file != NULL) {
		builder->NextShort(source_file->index);
		debug_printf(level3, "Source file name : %s.\n", source_file->bytes);
	} else {
		builder->NextShort(0);
	}

	return this;
}

SourceDebugExtensionAttribute *SourceDebugExtensionAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Source Debug Extension Attribute.\n");

	for(unsigned idx = 0; idx < attribute_length; idx++) {
		builder->NextByte(debug_extension[idx]);
	}

	return this;
}

int encodeLineNumberTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LineNumberTableEntry *entry) {
	debug_printf(level3, "Encoding Line Number Table Entry.\n");

	builder->NextShort(entry->start_pc);
	builder->NextShort(entry->line_number);

	ignore_unused(classFile);
	return 0;
}

LineNumberTableAttribute *LineNumberTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Line Number Table Attribute.\n");

	length = listSize(line_number_table);
	debug_printf(level2, "Line Number Table length : %hu.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeLineNumberTableEntry(classFile, builder, static_cast<
				LineNumberTableEntry *>(listGet(line_number_table, idx)));
	}

	return this;
}

int encodeLocalVariableTableEntry(
		ClassFile *classFile, ClassBuilder *builder, LocalVariableTableEntry *entry) {
	debug_printf(level3, "Encoding Local Variable Table Entry.\n");

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
	debug_printf(level3, "Encoding Local Variable Table Attribute.\n");

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
	debug_printf(level3, "Encoding Local Variable Type Table Entry.\n");

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

	debug_printf(level3, "Encoding Local Variable Type Table Attribute.\n");

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
		ClassFile *, ClassBuilder *builder, ElementValue *value) {
	debug_printf(level3, "Encoding Const Element Value.\n");

	builder->NextShort(value->value.const_value->index);

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

	debug_printf(level3, "Encoding Runtime Annotations Attribute.\n");

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

	debug_printf(level3, "Encoding Parameter Annotations Entry.\n");

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

	debug_printf(level3, "Encoding Parameter Annotations Attribute.\n");

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
	debug_printf(level3, "Encoding Annotation Default Attribute.\n");

	encodeElementValue(classFile, builder, default_value);

	return this;
}

int encodeBootstrapMethodEntry(
		ClassFile *classFile, ClassBuilder *builder, BootstrapMethodEntry *entry) {
	unsigned int idx, length;

	debug_printf(level3, "Encoding Bootstrap Method Entry.\n");

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

	debug_printf(level3, "Encoding Bootstrap Methods Attribute.\n");

	length = bootstrap_methods.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		encodeBootstrapMethodEntry(classFile, builder, bootstrap_methods[idx]);
	}

	return 0;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	debug_printf(level3, "Encoding Attribute.\n");

	builder->NextShort(info->name->index);
	builder->NextInt(info->attribute_length);
	info->EncodeAttribute(builder, classFile);

	return 0;
}

