
# include "Debug.h"
# include "ElementValue.h"

/* Element Value Decoders */

ConstantElementValue *ConstantElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	debug_printf(level3, "Decoding Constant Element Value.\n");

	// Constant Value
	index = buffer->NextShort();
	const_value = classFile->constant_pool[index];

	return this;
}

EnumConstantElementValue *EnumConstantElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	debug_printf(level3, "Decoding Enum Constant Element Value.\n");

	// Type Name
	index = buffer->NextShort();
	type_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	// Constant Name
	index = buffer->NextShort();
	const_name = static_cast<ConstantUtf8Info *>(
			classFile->constant_pool[index]);

	return this;
}

ClassElementValue *ClassElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	debug_printf(level3, "Decoding Class Element Value.\n");

	// Class Info
	index = buffer->NextShort();
	class_info = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);

	return this;
}

AnnotationElementValue *AnnotationElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	debug_printf(level3, "Decoding Annotation Element Value.\n");

	// Annotation Value
	annotation_value = decodeAnnotationEntry(classFile, buffer);

	return this;
}

ArrayElementValue *ArrayElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Array Element Value.\n");

	// Array Value Table
	length = buffer->NextShort();
	for(idx = 0; idx < length; idx++) {
		array_values.push_back(decodeElementValue(classFile, buffer));
	}

	return this;
}

ElementValue *DecodeElementValue(ClassBuffer *buffer, ClassFile *classFile) {
	uint8_t tag = buffer->NextByte();

	switch(tag) {
		// Constant Value types
		case 'B': case 'C': case 'D':
		case 'F': case 'I': case 'J':
		case 'S': case 'Z': case 's':
			debug_printf(level3, "Constant Element Value.\n");
			return (new ConstantElementValue(tag))->DecodeValue(buffer, classFile);
		// Enum Constant
		case 'e':
			debug_printf(level3, "Enum Constant Element Value.\n");
			return (new EnumConstantElementValue(tag))->DecodeValue(buffer, classFile);
		// Class Constant
		case 'c':
			debug_printf(level3, "Class Element Value.\n");
			return (new ClassElementValue(tag))->DecodeValue(buffer, classFile);
		// Annotation
		case '@':
			debug_printf(level3, "Annotation Element Value.\n");
			return (new AnnotationElementValue(tag))->DecodeValue(buffer, classFile);
		// Array
		case '[':
			debug_printf(level3, "Array Element Value.\n");
			return (new ArrayElementValue(tag))->DecodeValue(buffer, classFile);
	}
}
