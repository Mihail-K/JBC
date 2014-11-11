
# include "Debug.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

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
	builder->Next(code, code_length);
	debug_printf(level3, "Code length : %u.\n", code_length);

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

	builder->NextShort(enclosing_class == NULL ? 0 : enclosing_class->index);
	builder->NextShort(enclosing_method == NULL ? 0 : enclosing_method->index);

	return this;
}

SignatureAttribute *SignatureAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Signature Attribute.\n");

	builder->NextShort(signature == NULL ? 0 : signature->index);

	return this;
}

SourceFileAttribute *SourceFileAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Souce File Attribute.\n");

	builder->NextShort(source_file == NULL ? 0 : source_file->index);

	return this;
}

SourceDebugExtensionAttribute *SourceDebugExtensionAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Source Debug Extension Attribute.\n");

	builder->Next(debug_extension, attribute_length);

	return this;
}

LineNumberTableEntry *LineNumberTableEntry
		::EncodeEntry(ClassBuilder *builder) {
	debug_printf(level3, "Encoding Line Number Table Entry.\n");

	builder->NextShort(start_pc);
	builder->NextShort(line_number);

	return this;
}

LineNumberTableAttribute *LineNumberTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *) {
	uint16_t length;

	debug_printf(level3, "Encoding Line Number Table Attribute.\n");

	length = line_number_table.size();
	debug_printf(level2, "Line Number Table length : %hu.\n", length);
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		line_number_table[idx]->EncodeEntry(builder);
	}

	return this;
}

LocalVariableTableEntry *LocalVariableTableEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Local Variable Table Entry.\n");

	builder->NextShort(start_pc);
	builder->NextShort(length);

	builder->NextShort(name == NULL ? 0 : name->index);
	builder->NextShort(descriptor == NULL ? 0 : descriptor->index);

	builder->NextShort(index);

	return 0;
}

LocalVariableTableAttribute *LocalVariableTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	debug_printf(level3, "Encoding Local Variable Table Attribute.\n");

	uint16_t length;

	length = local_variable_table.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		local_variable_table[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

LocalVariableTypeTableEntry *LocalVariableTypeTableEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Local Variable Type Table Entry.\n");

	builder->NextShort(start_pc);
	builder->NextShort(length);

	builder->NextShort(name == NULL ? 0 : name->index);
	builder->NextShort(signature == NULL ? 0 : signature->index);

	builder->NextShort(index);

	return this;
}

LocalVariableTypeTableAttribute *LocalVariableTypeTableAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Local Variable Type Table Attribute.\n");

	length = local_variable_type_table.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		local_variable_type_table[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

RuntimeAnnotationsAttribute *RuntimeAnnotationsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Runtime Annotations Attribute.\n");

	length = annotations.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		annotations[idx]->EncodeEntry(builder, classFile);
	}

	return 0;
}

ParameterAnnotationsEntry *ParameterAnnotationsEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Parameter Annotations Entry.\n");

	length = annotations.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		annotations[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

RuntimeParameterAnnotationsAttribute *RuntimeParameterAnnotationsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Parameter Annotations Attribute.\n");

	length = parameter_annotations.size();
	builder->NextByte(length);

	for(unsigned idx = 0; idx < length; idx++) {
		parameter_annotations[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

AnnotationDefaultAttribute *AnnotationDefaultAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	debug_printf(level3, "Encoding Annotation Default Attribute.\n");

	EncodeElementValue(builder, classFile, default_value);

	return this;
}

BootstrapMethodEntry *BootstrapMethodEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *) {
	uint16_t length;

	debug_printf(level3, "Encoding Bootstrap Method Entry.\n");

	builder->NextShort(bootstrap_method_ref == NULL ? 0 :
			bootstrap_method_ref->index);
	length = bootstrap_arguments.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		ConstantInfo *info = bootstrap_arguments[idx];
		builder->NextShort(info == NULL ? 0 : info->index);
	}

	return this;
}

BootstrapMethodsAttribute *BootstrapMethodsAttribute
		::EncodeAttribute(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Bootstrap Methods Attribute.\n");

	length = bootstrap_methods.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		bootstrap_methods[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	debug_printf(level3, "Encoding Attribute.\n");

	builder->NextShort(info->name->index);
	builder->NextInt(info->attribute_length);
	info->EncodeAttribute(builder, classFile);

	return 0;
}

