
# include <stdlib.h>
# include <string.h>

# include "Debug.h"
# include "Defines.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

CodeAttribute::~CodeAttribute() {
	debug_printf(level3, "Deleting Code Attribute.\n");
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
	debug_printf(level3, "Deleting Stack Map Table Attribute.\n");

	if(!entries.empty()) {
		for(std::vector<StackMapFrame *>::iterator itr = entries.begin();
				itr != entries.end(); itr++) {
			delete *itr;
		}
	}
}

ExceptionsAttribute::~ExceptionsAttribute() {
	debug_printf(level3, "Deleting Exceptions Attribute.\n");

	// ConstantInfo entries are deallocated elsewhere.
	exception_table.clear();
}

InnerClassesAttribute::~InnerClassesAttribute() {
	debug_printf(level3, "Deleting Inner Classes Attribute.\n");

	if(!classes.empty()) {
		for(std::vector<InnerClassEntry *>::iterator itr = classes.begin();
				itr != classes.end(); itr++) {
			delete *itr;
		}
	}
}

SourceDebugExtensionAttribute::~SourceDebugExtensionAttribute() {
	debug_printf(level3, "Deleting Source Debug Extension Attribute.\n");

	if(debug_extension != NULL) {
		delete debug_extension;
	}
}

LineNumberTableAttribute::~LineNumberTableAttribute() {
	debug_printf(level3, "Deleting Line Number Table Attribute.\n");

	if(!line_number_table.empty()) {
		for(std::vector<LineNumberTableEntry *>::iterator itr = line_number_table.begin();
				itr != line_number_table.end(); itr++) {
			delete *itr;
		}
	}
}

LocalVariableTableAttribute::~LocalVariableTableAttribute() {
	debug_printf(level3, "Deleting Local Variable Table Attribute.\n");
	if(local_variable_table != NULL) {
		deleteList(local_variable_table, free);
	}
}

LocalVariableTypeTableAttribute::~LocalVariableTypeTableAttribute() {
	debug_printf(level3, "Deleting Local Variable Type Table Attribute.\n");
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
	debug_printf(level3, "Deleting Runtime Annotations Attribute.\n");
	if(annotations != NULL) {
		deleteList(annotations, (void(*)(void *))deleteAnnotationEntry);
	}
}

ParameterAnnotationsEntry::~ParameterAnnotationsEntry() {
	debug_printf(level3, "Deleting Parameter Annotations Entry.\n");
	if(annotations != NULL) {
		deleteList(annotations, (void(*)(void *))deleteAnnotationEntry);
	}
}

RuntimeParameterAnnotationsAttribute::~RuntimeParameterAnnotationsAttribute() {
	debug_printf(level3, "Deleting Runtime Parameter Annotations Attribute.\n");
	if(parameter_annotations != NULL) {
		deleteList(parameter_annotations, operator delete);
	}
}

AnnotationDefaultAttribute::~AnnotationDefaultAttribute() {
	debug_printf(level3, "Deleting Annotation Default Attribute.\n");
	if(default_value != NULL) {
		deleteElementValue(default_value);
	}
}

BootstrapMethodEntry::~BootstrapMethodEntry() {
	debug_printf(level3, "Deleting Bootstrap Method Entry.\n");
	if(bootstrap_arguments != NULL) {
		deleteList(bootstrap_arguments, NULL);
	}
}

BootstrapMethodsAttribute::~BootstrapMethodsAttribute() {
	debug_printf(level3, "Deleting Bootstrap Methods Attribute.\n");
	if(!bootstrap_methods.empty()) {
		for(std::vector<BootstrapMethodEntry *>::iterator itr = bootstrap_methods.begin();
				itr != bootstrap_methods.end(); itr++) {
			delete *itr;
		}
	}
}
