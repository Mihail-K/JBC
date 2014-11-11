
# include <string.h>

# include "Debug.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

namespace JBC {

ConstantValueAttribute *ConstantValueAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Constant Value Attribute.\n");

	// Constant Value
	index = buffer->NextShort();
	constant_value = classFile->constant_pool[index];

	return this;
}

ExceptionTableEntry *ExceptionTableEntry
		::DecodeEntry(ClassBuffer *buffer) {
	debug_printf(level3, "Decoding Exception Table Entry.\n");

	start_pc = buffer->NextShort();
	end_pc = buffer->NextShort();
	handler_pc = buffer->NextShort();
	catch_type = buffer->NextShort();

	return this;
}

CodeAttribute *CodeAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Code Attribute.\n");

	// Maximums
	max_stack = buffer->NextShort();
	max_locals = buffer->NextShort();

	// Code
	code_length = buffer->NextInt();
	debug_printf(level2, "Code length : %u.\n", code_length);

	code = new uint8_t[code_length];
	buffer->Next(code, code_length);

	// Exception Table
	length = buffer->NextShort();
	debug_printf(level2, "Code Exception table length : %hu.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Exception table entry %u :\n", idx);
		exception_table.push_back((new ExceptionTableEntry)->DecodeEntry(buffer));
	}

	// Attributes Table
	length = buffer->NextShort();
	debug_printf(level2, "Code Attributes count : %hu.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Code Attribute %u :\n", idx);
		attributes.push_back(JBC::DecodeAttribute(buffer, classFile));
	}

	return this;
}

StackMapTableAttribute *StackMapTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level2, "Decoding Stack Map Table Attribute.\n");

	// Stack Map Table
	length = buffer->NextShort();
	debug_printf(level2, "Stack Frame count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Stack Map Frame %d :\n", idx);
		entries.push_back(DecodeStackMapFrame(buffer, classFile));
	}

	debug_printf(level2, "Finished StackMapTable.\n");

	return this;
}

ExceptionsAttribute *ExceptionsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Exceptions Attribute.\n");

	// Exceptions Table
	length = buffer->NextShort();
	debug_printf(level2, "Exceptions count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		debug_printf(level2, "Exception entry %d :\n", idx);
		exception_table.push_back(classFile->constant_pool[index]);
	}

	return this;
}

InnerClassEntry *InnerClassEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Inner Class Entry.\n");

	// Inner Class Info
	index = buffer->NextShort();
	inner_class_info = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	// Outer Class Info
	index = buffer->NextShort();
	outer_class_info = dynamic_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	// Inner Class Name
	index = buffer->NextShort();
	inner_class_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	debug_printf(level2, "Inner class name : %s.\n",
			(index != 0 ? (char *)inner_class_name->bytes
			: "<anonymous class>"));

	// Inner Class Flags
	index = buffer->NextShort();
	inner_class_access_flags = index;

	return this;
}

InnerClassesAttribute *InnerClassesAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Inner Classes Attribute.\n");

	// Inner Classes Table
	length = buffer->NextShort();
	debug_printf(level2, "Inner classes count : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Inner class %d :\n", idx);
		classes.push_back((new InnerClassEntry)->DecodeEntry(buffer, classFile));
	}

	return this;
}

EnclosingMethodAttribute *EnclosingMethodAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Enclosing Method Attribute.\n");

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

	debug_printf(level3, "Decoding Signature Attribute.\n");

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
	debug_printf(level3, "Decoding Source Debug Extension Attribute.\n");

	// Debug Extension
	debug_extension = new uint8_t[attribute_length];
	buffer->Next(debug_extension, attribute_length);

	return this;
}

LineNumberTableEntry *LineNumberTableEntry
		::DecodeEntry(ClassBuffer *buffer) {
	debug_printf(level3, "Decoding Line Number Table Entry.\n");

	start_pc = buffer->NextShort();
	line_number = buffer->NextShort();

	return this;
}

LineNumberTableAttribute *LineNumberTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *) {
	uint16_t length;

	debug_printf(level3, "Decoding Line Number Table Attribute.\n");

	// Line Number Table
	length = buffer->NextShort();
	debug_printf(level2, "Line Number Table length : %hu.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		line_number_table.push_back((new LineNumberTableEntry)
				->DecodeEntry(buffer));
	}

	return this;
}

LocalVariableTableEntry *LocalVariableTableEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Local Variable Table Entry.\n");

	start_pc = buffer->NextShort();
	length = buffer->NextShort();

	// Variable Name
	index = buffer->NextShort();
	name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable name : %s.\n", name->bytes);

	// Variable Descriptor
	index = buffer->NextShort();
	descriptor = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable descriptor : %s.\n", descriptor->bytes);

	this->index = buffer->NextShort();
	debug_printf(level3, "Index : %d.\n", this->index);

	return this;
}

LocalVariableTableAttribute *LocalVariableTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Variable Table Attribute.\n");

	// Local Variable Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Table length : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		local_variable_table.push_back((new LocalVariableTableEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

LocalVariableTypeTableEntry *LocalVariableTypeTableEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Local Variable Type Table Entry.\n");

	start_pc = buffer->NextShort();
	length = buffer->NextShort();

	// Variable Type Name
	index = buffer->NextShort();
	name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable name : %s.\n", name->bytes);

	// Variable Type Signature
	index = buffer->NextShort();
	signature = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Local variable signature : %s.\n", signature->bytes);

	this->index = buffer->NextShort();
	debug_printf(level3, "Index : %d.\n", this->index);

	return this;
}

LocalVariableTypeTableAttribute *LocalVariableTypeTableAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Local Variable Type Table Attribute.\n");

	// Local Variable Type Table
	length = buffer->NextShort();
	debug_printf(level2, "Local Variable Type Table length : %d.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		local_variable_type_table.push_back((new LocalVariableTypeTableEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

RuntimeAnnotationsAttribute *RuntimeAnnotationsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Annotations Table
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		annotations.push_back((new AnnotationEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

ParameterAnnotationsEntry *ParameterAnnotationsEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Annotations Table
	length = buffer->NextShort();
	debug_printf(level2, "Parameter Annotations entry count : %u.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Parameter Annotation entry %u :\n", idx);
		annotations.push_back((new AnnotationEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

RuntimeParameterAnnotationsAttribute *RuntimeParameterAnnotationsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	// Parameter Annotations Table
	length = buffer->NextByte();
	debug_printf(level2, "Parameter Annotation count : %u.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Parameter Annotation %u :\n", idx);
		parameter_annotations.push_back((new ParameterAnnotationsEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

AnnotationDefaultAttribute *AnnotationDefaultAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	// Default Value
	default_value = DecodeElementValue(buffer, classFile);

	return this;
}

BootstrapMethodEntry *BootstrapMethodEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index, length;

	debug_printf(level3, "Decoding Bootstrap Method Entry.\n");

	index = buffer->NextShort();
	bootstrap_method_ref = static_cast<ConstantMethodHandleInfo *>(
			classFile->constant_pool[index]);

	// Bootstrap Method Parameters Table
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		uint16_t index = buffer->NextShort();
		bootstrap_arguments.push_back(classFile->constant_pool[index]);
	}

	return this;
}

BootstrapMethodsAttribute *BootstrapMethodsAttribute
		::DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Bootstrap Methods Attribute.\n");

	// Bootstrap Method Table
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		bootstrap_methods.push_back((new BootstrapMethodEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

AttributeInfo *DecodeAttribute(ClassBuffer *buffer, ClassFile *classFile) {
	size_t initpos = buffer->Position();
	uint16_t name_index = buffer->NextShort();
	uint32_t attribute_length = buffer->NextInt();
	ConstantUtf8Info *name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[name_index]);

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
		size_t difference;

		debug_printf(level2, "Unknown Attribute type : %s; Skipping.\n", name->bytes);
		if((difference = buffer->Position() - initpos) < attribute_length) {
			buffer->Skip(difference);
		}
	}

	return NULL;
}

} /* JBC */
