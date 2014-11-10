
# include <stdlib.h>
# include <string.h>

# include "Debug.h"
# include "Defines.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

void deleteCodeAttribute(CodeAttribute *code) {
	if(code->code != NULL)
		delete code->code;
	if(code->exception_table != NULL) {
		debug_printf(level3, "Deleting exception table.\n");
		deleteList(code->exception_table, free);
	}
	if(code->attributes != NULL) {
		debug_printf(level3, "Deleting code attributes.\n");
		deleteList(code->attributes, (void(*)(void *))
				deleteAttribute);
	}
	delete code;
}

void deleteStackMapTableAttribute(StackMapTableAttribute *table) {
	if(!table->entries.empty()) {
		for(std::vector<StackMapFrame *>::iterator itr = table->entries.begin();
				itr != table->entries.end(); itr++) {
			delete *itr;
		}
	}

	delete table;
}

void deleteExceptionsAttribute(ExceptionsAttribute *except) {
	if(except->exception_table != NULL)
		deleteList(except->exception_table, NULL);
	delete except;
}

void deleteInnerClassesAttribute(InnerClassesAttribute *inner) {
	if(inner->classes != NULL)
		deleteList(inner->classes, free);
	delete inner;
}

void deleteLineNumberTableAttribute(LineNumberTableAttribute *table) {
	if(table->line_number_table != NULL)
		deleteList(table->line_number_table, free);
	delete table;
}

void deleteLocalVariableTableAttribute(LocalVariableTableAttribute *table) {
	if(table->local_variable_table != NULL)
		deleteList(table->local_variable_table, free);
	delete table;
}

void deleteLocalVariableTypeTableAttribute(LocalVariableTypeTableAttribute *table) {
	if(table->local_variable_type_table != NULL)
		deleteList(table->local_variable_type_table, free);
	delete table;
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

void deleteRuntimeAnnotationsAttribute(RuntimeAnnotationsAttribute *annot) {
	if(annot->annotations != NULL)
		deleteList(annot->annotations, (void(*)(void *))
				deleteAnnotationEntry);
	delete annot;
}

void deleteParameterAnnotationsEntry(ParameterAnnotationsEntry *entry) {
	if(entry->annotations != NULL)
		deleteList(entry->annotations, (void(*)(void *))
				deleteAnnotationEntry);
	delete entry;
}

void deleteRuntimeParameterAnnotationsAttribute(RuntimeParameterAnnotationsAttribute *annot) {
	if(annot->parameter_annotations != NULL)
		deleteList(annot->parameter_annotations, (void(*)(void *))
				deleteParameterAnnotationsEntry);
	delete annot;
}

void deleteAnnotationDefaultAttribute(AnnotationDefaultAttribute *value) {
	if(value->default_value != NULL)
		deleteElementValue(value->default_value);
	delete value;
}

void deleteBootstrapMethodEntry(BootstrapMethodEntry *entry) {
	if(entry->bootstrap_arguments != NULL)
		deleteList(entry->bootstrap_arguments, NULL);
	delete entry;
}

void deleteBootstrapMethodsAttribute(BootstrapMethodsAttribute *bootstrap) {
	if(bootstrap->bootstrap_methods != NULL)
		deleteList(bootstrap->bootstrap_methods, (void(*)(void *))
				deleteBootstrapMethodEntry);
	delete bootstrap;
}

void deleteAttribute(AttributeInfo *info) {
	ConstantUtf8Info *name;
	if(info == NULL) return;

	name = info->name;
	if(name == NULL || name->bytes == NULL) {
		delete info;
		return;
	}

	debug_printf(level2, "Deleting attribute : %s.\n", name->bytes);

	// Code Attribute
	if(!strcmp("Code", (char *)name->bytes)) {
		deleteCodeAttribute((CodeAttribute *)info);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", (char *)name->bytes)) {
		deleteStackMapTableAttribute((StackMapTableAttribute *)info);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", (char *)name->bytes)) {
		deleteExceptionsAttribute((ExceptionsAttribute *)info);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", (char *)name->bytes)) {
		deleteInnerClassesAttribute((InnerClassesAttribute *)info);
	} else
	// Source Debug Extension Attribute
	if(!strcmp("SourceDebugExtension", (char *)name->bytes)) {
		delete ((SourceDebugExtensionAttribute *)info)->debug_extension;
		delete info;
	} else
	// Line Number Table Attribute
	if(!strcmp("LineNumberTable", (char *)name->bytes)) {
		deleteLineNumberTableAttribute((LineNumberTableAttribute *)info);
	} else
	// Local Variable Table Attribute
	if(!strcmp("LocalVariableTable", (char *)name->bytes)) {
		deleteLocalVariableTableAttribute((LocalVariableTableAttribute *)info);
	} else
	// Local Variable Type Table Attribute
	if(!strcmp("LocalVariableTypeTable", (char *)name->bytes)) {
		deleteLocalVariableTypeTableAttribute((LocalVariableTypeTableAttribute *)info);
	} else
	// Runtime Visible Annotations Attribute
	if(!strcmp("RuntimeVisibleAnnotations", (char *)name->bytes)) {
		deleteRuntimeAnnotationsAttribute((RuntimeAnnotationsAttribute *)info);
	} else
	// Runtime Invisible Annotations Attribute
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)name->bytes)) {
		deleteRuntimeAnnotationsAttribute((RuntimeAnnotationsAttribute *)info);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)name->bytes)) {
		deleteRuntimeParameterAnnotationsAttribute((RuntimeParameterAnnotationsAttribute *)info);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)name->bytes)) {
		deleteRuntimeParameterAnnotationsAttribute((RuntimeParameterAnnotationsAttribute *)info);
	} else
	// Annotation Default Attribute
	if(!strcmp("AnnotationDefault", (char *)name->bytes)) {
		deleteAnnotationDefaultAttribute((AnnotationDefaultAttribute *)info);
	} else
	// Bootstrap Methods Attribute
	if(!strcmp("BootstrapMethods", (char *)name->bytes)) {
		deleteBootstrapMethodsAttribute((BootstrapMethodsAttribute *)info);
	} else {
		delete info;
	}
}
