
# include <stdlib.h>
# include <string.h>

# include "Debug.h"
# include "Defines.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

CodeAttribute::~CodeAttribute() {
	if(code != NULL)
		delete code;
	if(!exception_table.empty()) {
		debug_printf(level3, "Deleting exception table.\n");
		for(std::vector<ExceptionTableEntry *>::iterator itr = exception_table.begin();
				itr != exception_table.end(); itr++) {
			delete *itr;
		}
	}
	if(!attributes.empty()) {
		debug_printf(level3, "Deleting code attributes.\n");
		for(std::vector<AttributeInfo *>::iterator itr = attributes.begin();
				itr != attributes.end(); itr++) {
			delete *itr;
		}
	}
}

StackMapTableAttribute::~StackMapTableAttribute() {
	if(!entries.empty()) {
		for(std::vector<StackMapFrame *>::iterator itr = entries.begin();
				itr != entries.end(); itr++) {
			delete *itr;
		}
	}
}

ExceptionsAttribute::~ExceptionsAttribute() {
	if(exception_table != NULL) {
		deleteList(exception_table, NULL);
	}
}

InnerClassesAttribute::~InnerClassesAttribute() {
	if(classes != NULL) {
		deleteList(classes, free);
	}
}

SourceDebugExtensionAttribute::~SourceDebugExtensionAttribute() {
	if(debug_extension != NULL) {
		delete debug_extension;
	}
}

LineNumberTableAttribute::~LineNumberTableAttribute() {
	if(line_number_table != NULL) {
		deleteList(line_number_table, free);
	}
}

LocalVariableTableAttribute::~LocalVariableTableAttribute() {
	if(local_variable_table != NULL) {
		deleteList(local_variable_table, free);
	}
}

LocalVariableTypeTableAttribute::~LocalVariableTypeTableAttribute() {
	if(local_variable_type_table != NULL) {
		deleteList(local_variable_type_table, free);
	}
}

void deleteElementValue(ElementValue *value) {
	extern void deleteAnnotationEntry(AnnotationEntry *);

	switch(value->tag) {
		case '@':
			deleteAnnotationEntry(value->value.annotation_value);
			break;
		case '[':
			if(value->value.array_values != NULL)
				deleteList(value->value.array_values, (void(*)(void *))
						deleteElementValue);
			break;
	}
	delete value;
}

void deleteElementValuePairsEntry(ElementValuePairsEntry *entry) {
	if(entry->value != NULL)
		deleteElementValue(entry->value);
	delete entry;
}

void deleteAnnotationEntry(AnnotationEntry *entry) {
	if(entry->element_value_pairs != NULL)
		deleteList(entry->element_value_pairs, (void(*)(void *))
				deleteElementValuePairsEntry);
	delete entry;
}

RuntimeAnnotationsAttribute::~RuntimeAnnotationsAttribute() {
	if(annotations != NULL) {
		deleteList(annotations, (void(*)(void *))deleteAnnotationEntry);
	}
}

ParameterAnnotationsEntry::~ParameterAnnotationsEntry() {
	if(annotations != NULL) {
		deleteList(annotations, (void(*)(void *))deleteAnnotationEntry);
	}
}

RuntimeParameterAnnotationsAttribute::~RuntimeParameterAnnotationsAttribute() {
	if(parameter_annotations != NULL) {
		deleteList(parameter_annotations, operator delete);
	}
}

AnnotationDefaultAttribute::~AnnotationDefaultAttribute() {
	if(default_value != NULL) {
		deleteElementValue(default_value);
	}
}

BootstrapMethodEntry::~BootstrapMethodEntry() {
	if(bootstrap_arguments != NULL) {
		deleteList(bootstrap_arguments, NULL);
	}
}

BootstrapMethodsAttribute::~BootstrapMethodsAttribute() {
	if(bootstrap_methods != NULL) {
		deleteList(bootstrap_methods, operator delete);
	}
}
