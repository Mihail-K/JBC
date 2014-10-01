
# include "Debug.h"
# include "Zalloc.h"
# include "AttributeInfo.h"

AttributeInfo *visitConstantValueAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	ConstantValueAttribute *info = zalloc(sizeof(ConstantValueAttribute));

	// Constant Value
	info->constant_value_index = bufferNextShort(buffer);
	info->constant_value = classFile->constant_pool[info->constant_value_index];

	return (AttributeInfo *)info;
}

ExceptionTableEntry *visitExceptionTableEntry(ClassFile *classFile, ClassBuffer *buffer) {
	ExceptionTableEntry *entry = zalloc(sizeof(ExceptionTableEntry));

	entry->start_pc = bufferNextShort(buffer);
	entry->end_pc = bufferNextShort(buffer);
	entry->handler_pc = bufferNextShort(buffer);
	entry->catch_type = bufferNextShort(buffer);

	return entry;
}

AttributeInfo *visitCodeAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	CodeAttribute *code = zalloc(sizeof(CodeAttribute));

	// Maximums
	code->max_stack = bufferNextShort(buffer);
	code->max_locals = bufferNextShort(buffer);

	// Code
	code->code_length = bufferNextInt(buffer);
	code->code = zalloc(sizeof(uint8_t) * code->code_length);

	for(idx = 0; idx < code->code_length; idx++) {
		// TODO : Added a mass read operation
		code->code[idx] = bufferNextByte(buffer);
	}

	// Exception Table
	code->exception_table_length = bufferNextShort(buffer);
	code->exception_table = zalloc(sizeof(ExceptionTableEntry *) *
			code->exception_table_length);

	for(idx = 0; idx < code->exception_table_length; idx++) {
		code->exception_table[idx] = visitExceptionTableEntry(classFile, buffer);
	}

	// Attributes Table
	code->attributes_count = bufferNextShort(buffer);
	code->attributes = zalloc(sizeof(AttributeInfo *) * code->attributes_count);

	for(idx = 0; idx < code->attributes_count; idx++) {
		code->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return (AttributeInfo *)code;
}

AttributeInfo *visitExceptionsAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	ExceptionsAttribute *except = zalloc(sizeof(ExceptionsAttribute));

	// Exceptions Table
	except->number_of_exceptions = bufferNextShort(buffer);
	except->exception_index_table = zalloc(sizeof(uint16_t) *
			except->number_of_exceptions);
	except->exception_table = zalloc(sizeof(ConstantClassInfo *) *
			except->number_of_exceptions);

	for(idx = 0; idx < except->number_of_exceptions; idx++) {
		uint16_t index = bufferNextShort(buffer);
		except->exception_index_table[idx] = index;
		except->exception_table[idx] = (void *)classFile->constant_pool[index];
	}

	return (AttributeInfo *)except;
}

InnerClassEntry *visitInnerClassEntry(ClassFile *classFile, ClassBuffer *buffer) {
	uint16_t index;
	InnerClassEntry *entry = zalloc(sizeof(InnerClassEntry));

	// Inner Class Info
	index = bufferNextShort(buffer);
	entry->inner_class_info_index = index;
	entry->inner_class_info = (void *)classFile->constant_pool[index];

	// Outer Class Info
	index = bufferNextShort(buffer);
	entry->outer_class_info_index = index;
	entry->outer_class_info = (void *)classFile->constant_pool[index];

	// Inner Class Name
	index = bufferNextShort(buffer);
	entry->inner_class_name_index = index;
	entry->inner_class_name = (void *)classFile->constant_pool[index];

	// Inner Class Flags
	index = bufferNextShort(buffer);
	entry->inner_class_access_flags = index;

	return entry;
}

AttributeInfo *visitInnerClassesAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	InnerClassesAttribute *inner = zalloc(sizeof(InnerClassesAttribute));

	// Inner Classes Table
	inner->number_of_classes = bufferNextShort(buffer);
	inner->classes = zalloc(sizeof(InnerClassEntry *) *
			inner->number_of_classes);

	for(idx = 0; idx < inner->number_of_classes; idx++) {
		inner->classes[idx] = visitInnerClassEntry(classFile, buffer);
	}

	return inner;
}

AttributeInfo *visitAttribute(ClassFile *classFile, ClassBuffer *buffer) {
	AttributeInfo *info;
	uint16_t name_index = bufferNextShort(buffer);
	uint32_t attribute_length = bufferNextInt(buffer);
	ConstantUtf8Info *name = (void *)classFile->constant_pool[name_index];

	if(name == NULL) {
		fprintf(stderr, "Error : Attribute with no name entry!\n");
		exit(EXIT_FAILURE);
	}

	// Constant Value Attribute
	if(!strcmp("ConstantValue", name->bytes)) {
		info = visitConstantValueAttribute(classFile, buffer);
	} else
	// Code Attribute
	if(!strcmp("Code", name->bytes)) {
		info = visitCodeAttribute(classFile, buffer);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", name->bytes)) {
		debug_printf("Not yet implemented!\n");
		exit(EXIT_FAILURE);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", name->bytes)) {
		info = visitExceptionsAttribute(classFile, buffer);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", name->bytes)) {
		info = visitInnerClassesAttribute(classFile, buffer);
	} else
	// Enclosing Method Attribute
	if(!strcmp("EnclosingMethod", name->bytes)) {
	
	}

	info->attribute_length = attribute_length;
	info->name_index = name_index;
	info->name = name;

	return info;
}
