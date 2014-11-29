/**
 * @file ElementValue.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.38
 *
 * @brief Defines all Element Value types.
 **/
# ifndef __ELEMENTVALUE_H__
# define __ELEMENTVALUE_H__

# include <vector>

# include "ClassFile.h"
# include "ClassBuffer.h"
# include "ClassBuilder.h"
# include "ConstantInfo.h"

namespace JBC {

struct AnnotationEntry;

struct ElementValue {
	uint8_t		tag;

	ElementValue()
		: tag(0) {
	}

	ElementValue(uint8_t tag)
		: tag(tag) {
	}

	virtual
	~ElementValue() {
	}

	virtual
	ElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile) = 0;

	virtual
	ElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile) = 0;
};

struct ConstantElementValue
		: public ElementValue {
	// Constant Value
	ConstantInfo *const_value;

	ConstantElementValue()
		: const_value(NULL) {
	}

	ConstantElementValue(uint8_t tag)
		: ElementValue(tag), const_value(NULL) {
	}

	ConstantElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	ConstantElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct EnumConstantElementValue
		: public ElementValue {
	// Type Name
	ConstantUtf8Info *type_name;

	// Constant Name
	ConstantUtf8Info *const_name;

	EnumConstantElementValue()
		: type_name(NULL), const_name(NULL) {
	}

	EnumConstantElementValue(uint8_t tag)
		: ElementValue(tag), type_name(NULL), const_name(NULL) {
	}

	EnumConstantElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	EnumConstantElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct ClassElementValue
		: public ElementValue {
	// Class Info Value
	ConstantClassInfo *class_info;

	ClassElementValue()
		: class_info(NULL) {
	}

	ClassElementValue(uint8_t tag)
		: ElementValue(tag), class_info(NULL) {
	}

	ClassElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	ClassElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct AnnotationElementValue
		: public ElementValue {
	// Annotation Value
	AnnotationEntry *annotation_value;

	AnnotationElementValue()
		: annotation_value(NULL) {
	}

	AnnotationElementValue(uint8_t tag)
		: ElementValue(tag), annotation_value(NULL) {
	}

	~AnnotationElementValue();

	AnnotationElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	AnnotationElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct ArrayElementValue
		: public ElementValue {
	// Array Value
	std::vector<ElementValue *> array_values;

	ArrayElementValue() {
	}

	ArrayElementValue(uint8_t tag)
		: ElementValue(tag) {
	}

	~ArrayElementValue();

	ArrayElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	ArrayElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct ElementValuePairsEntry {
	// Name
	ConstantUtf8Info *element_name;

	// Element Value
	ElementValue *value;

	ElementValuePairsEntry()
		: element_name(NULL), value(NULL) {
	}

	~ElementValuePairsEntry();

	ElementValuePairsEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	ElementValuePairsEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct AnnotationEntry {
	// Type
	ConstantUtf8Info *type;

	// Element-Value Pairs Table
	std::vector<ElementValuePairsEntry *> element_value_pairs;

	AnnotationEntry()
		: type(NULL) {
	}

	~AnnotationEntry();

	AnnotationEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	AnnotationEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

ElementValue *DecodeElementValue(ClassBuffer *buffer, ClassFile *classFile);

void EncodeElementValue(ClassBuilder *builder, ClassFile *classFile, ElementValue *value);

} /* JBC */

# endif /* ElementValue.h */
