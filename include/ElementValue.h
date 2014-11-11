# ifndef __ELEMENTVALUE_H__
# define __ELEMENTVALUE_H__

# include <vector>

# include "ClassFile.h"
# include "ClassBuffer.h"
# include "ClassBuilder.h"
# include "ConstantInfo.h"

struct AnnotationEntry;

struct ElementValue {
	uint8_t		tag;

	ElementValue() {
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

	ConstantElementValue() {
	}

	ConstantElementValue(uint8_t tag)
		: ElementValue(tag) {
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

	EnumConstantElementValue() {
	}

	EnumConstantElementValue(uint8_t tag)
		: ElementValue(tag) {
	}

	EnumConstantElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	EnumConstantElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct ClassElementValue
		: public ElementValue {
	// Class Info Value
	ConstantClassInfo *class_info;

	ClassElementValue() {
	}

	ClassElementValue(uint8_t tag)
		: ElementValue(tag) {
	}

	ClassElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	ClassElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct AnnotationElementValue
		: public ElementValue {
	// Annotation Value
	AnnotationEntry *annotation_value;

	AnnotationElementValue() {
	}

	AnnotationElementValue(uint8_t tag)
		: ElementValue(tag) {
	}

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

	ArrayElementValue *DecodeValue(ClassBuffer *buffer, ClassFile *classFile);

	ArrayElementValue *EncodeValue(ClassBuilder *builder, ClassFile *classFile);
};

struct ElementValuePairsEntry {
	// Name
	ConstantUtf8Info *element_name;

	// Element Value
	ElementValue *value;

	ElementValuePairsEntry() {
	}

	ElementValuePairsEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	ElementValuePairsEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

struct AnnotationEntry {
	// Type
	ConstantUtf8Info *type;

	// Element-Value Pairs Table
	std::vector<ElementValuePairsEntry *> element_value_pairs;

	AnnotationEntry() {
	}

	AnnotationEntry *DecodeEntry(ClassBuffer *buffer, ClassFile *classFile);

	AnnotationEntry *EncodeEntry(ClassBuilder *builder, ClassFile *classFile);
};

ElementValue *DecodeElementValue(ClassBuffer *buffer, ClassFile *classFile);

void EncodeElementValue(ClassBuilder *builder, ClassFile *classFile, ElementValue *value);

# endif /* ElementValue.h */
