
# include "Debug.h"
# include "ElementValue.h"

namespace JBC {

/* Element Value Decoders */

ConstantElementValue *ConstantElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	debug_printf(level3, "Decoding Constant Element Value.\n");

	// Constant Value
	index = buffer->NextShort();
	const_value = classFile->constant_pool[index];

	return this;
}

EnumConstantElementValue *EnumConstantElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

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
	uint16_t index;

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
	annotation_value = (new AnnotationEntry)
			->DecodeEntry(buffer, classFile);

	return this;
}

ArrayElementValue *ArrayElementValue
		::DecodeValue(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Decoding Array Element Value.\n");

	// Array Value Table
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		array_values.push_back(DecodeElementValue(buffer, classFile));
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
		default:
			// TODO : Throw an exception
			return NULL;
	}
}

/* Element Value Encoders */

ConstantElementValue *ConstantElementValue
		::EncodeValue(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Constant Element Value.\n");

	builder->NextShort(const_value == NULL ? 0 : const_value->index);

	return this;
}

EnumConstantElementValue *EnumConstantElementValue
		::EncodeValue(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Enum Constant Element Value.\n");

	builder->NextShort(type_name == NULL ? 0 : type_name->index);
	builder->NextShort(const_name == NULL ? 0 : const_name->index);

	return this;
}

ClassElementValue *ClassElementValue
		::EncodeValue(ClassBuilder *builder, ClassFile *) {
	debug_printf(level3, "Encoding Constant Element Value.\n");

	builder->NextShort(class_info == NULL ? 0 : class_info->index);

	return this;
}

AnnotationElementValue *AnnotationElementValue
		::EncodeValue(ClassBuilder *builder, ClassFile *classFile) {
	debug_printf(level3, "Encoding Annotation Element Value.\n");

	annotation_value->EncodeEntry(builder, classFile);

	return this;
}

ArrayElementValue *ArrayElementValue
		::EncodeValue(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	debug_printf(level3, "Encoding Array Element Value.\n");

	length = array_values.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		EncodeElementValue(builder, classFile, array_values[idx]);
	}

	return this;
}

void EncodeElementValue(ClassBuilder *builder, ClassFile *classFile, ElementValue *value) {
	builder->NextByte(value->tag);
	value->EncodeValue(builder, classFile);
}

/* Element Value Destructors */

AnnotationElementValue::~AnnotationElementValue() {
	if(annotation_value != NULL) {
		delete annotation_value;
	}
}

ArrayElementValue::~ArrayElementValue() {
	if(!array_values.empty()) {
		for(std::vector<ElementValue *>::iterator itr = array_values.begin();
				itr != array_values.end(); itr++) {
			delete *itr;
		}
	}
}

/* Element Value Pairs Entry */

ElementValuePairsEntry *ElementValuePairsEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index;

	// Element Name
	index = buffer->NextShort();
	element_name = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);
	debug_printf(level3, "Element-Value Name : %s.\n",
			(element_name == NULL ? "<NULL>" :
			(char *)element_name->bytes));

	// Element Value
	value = DecodeElementValue(buffer, classFile);

	return this;
}

ElementValuePairsEntry *ElementValuePairsEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *classFile) {
	builder->NextShort(element_name == NULL ? 0 : element_name->index);
	EncodeElementValue(builder, classFile, value);

	return this;
}

ElementValuePairsEntry::~ElementValuePairsEntry() {
	if(value != NULL) {
		delete value;
	}
}

/* Annotation Entry */

AnnotationEntry *AnnotationEntry
		::DecodeEntry(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index, length;

	// Annotation Entry Type
	index = buffer->NextShort();
	type = static_cast<ConstantUtf8Info *>(classFile->constant_pool[index]);

	// Element Value Pairs Table
	length = buffer->NextShort();
	debug_printf(level2, "Element-Value Pairs count : %u.\n", length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Element-Value Pair %u :\n", idx);
		element_value_pairs.push_back((new ElementValuePairsEntry)
				->DecodeEntry(buffer, classFile));
	}

	return this;
}

AnnotationEntry *AnnotationEntry
		::EncodeEntry(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	builder->NextShort(type == NULL ? 0 : type->index);
	length = element_value_pairs.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		element_value_pairs[idx]->EncodeEntry(builder, classFile);
	}

	return this;
}

AnnotationEntry::~AnnotationEntry() {
	if(!element_value_pairs.empty()) {
		for(std::vector<ElementValuePairsEntry *>::iterator itr = element_value_pairs
				.begin(); itr != element_value_pairs.end(); itr++) {
			delete *itr;
		}
	}
}

} /* JBC */
