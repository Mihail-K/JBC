
# include <stdlib.h>
# include <string.h>

# include "ClassFile.h"
# include "AttributeInfo.h"

void deleteCodeAttribute(CodeAttribute *code) {
	unsigned int idx;
	if(code->code != NULL) free(code);
	if(code->exception_table) {
		for(idx = 0; idx < code->exception_table_length; idx++)
			free(code->exception_table[idx]);
		free(code->exception_table);
	}
	free(code);
}

void deleteStackMapOffFrame(StackMapOffFrame *frame) {
	free(frame);
}

void deleteStackMapItemFrame(StackMapItemFrame *frame) {
	if(frame->stack != NULL)
		free(frame->stack);
	free(frame);
}

void deleteStackMapExtFrame(StackMapExtFrame *frame) {
	if(frame->stack != NULL)
		free(frame->stack);
	free(frame);
}

void deleteStackMapListFrame(StackMapListFrame *frame) {
	int idx, count = frame->tag - 251;
	if(frame->stack != NULL) {
		for(idx = 0; idx < count; idx++)
			free(frame->stack[idx]);
		free(frame->stack);
	}
	free(frame);
}

void deleteStackMapFullFrame(StackMapFullFrame *frame) {
	unsigned int idx;
	if(frame->locals != NULL) {
		for(idx = 0; idx < frame->number_of_locals; idx++)
			free(frame->locals[idx]);
		free(frame->locals);
	}
	if(frame->stack != NULL) {
		for(idx = 0; idx < frame->number_of_stack_items; idx++)
			free(frame->stack[idx]);
		free(frame->stack);
	}
	free(frame);
}

void deleteStackMapFrame(StackMapFrame *frame) {
	if(frame->tag <= 63) {
		free(frame);
	} else
	// Stack Map Same Locals 1
	if(frame->tag <= 127) {
		deleteStackMapItemFrame((void *)frame);
	} else
	// Reserved Values
	if(frame->tag <= 246) {
		free(frame);
	} else
	// Stack Map Same Locals 1 Extended
	if(frame->tag == 247) {
		deleteStackMapExtFrame((void *)frame);
	} else
	// Stack Map Chop Frame
	if(frame->tag <= 250) {
		deleteStackMapOffFrame((void *)frame);
	} else
	// Stack Map Same Frame Extended
	if(frame->tag == 251) {
		deleteStackMapOffFrame((void *)frame);
	} else
	// Stack Map Append Frame
	if(frame->tag <= 254) {
		deleteStackMapListFrame((void *)frame);
	}
	// Stack Map Full Frame
	else {
		deleteStackMapFullFrame((void *)frame);
	}
}

void deleteStackMapTableAttribute(StackMapTableAttribute *table) {
	unsigned int idx;
	if(table->entries != NULL) {
		for(idx = 0; idx < table->number_of_entries; idx++)
			deleteStackMapFrame(table->entries[idx]);
		free(table->entries);
	}
	free(table);
}

void deleteExceptionsAttribute(ExceptionsAttribute *except) {
	if(except->exception_table != NULL)
		free(except->exception_table);
	free(except);
}

void deleteInnerClassesAttribute(InnerClassesAttribute *inner) {
	unsigned int idx;
	if(inner->classes != NULL) {
		for(idx = 0; idx < inner->number_of_classes; idx++)
			free(inner->classes[idx]);
		free(inner->classes);
	}
	free(inner);
}

void deleteLineNumberTableAttribute(LineNumberTableAttribute *table) {
	unsigned int idx;
	if(table->line_number_table != NULL) {
		for(idx = 0; idx < table->line_number_table_length; idx++)
			free(table->line_number_table[idx]);
		free(table->line_number_table);
	}
	free(table);
}

void deleteLocalVariableTableAttribute(LocalVariableTableAttribute *table) {
	unsigned int idx;
	if(table->local_variable_table != NULL) {
		for(idx = 0; idx < table->local_variable_table_length; idx++)
			free(table->local_variable_table[idx]);
		free(table->local_variable_table);
	}
	free(table);
}

void deleteLocalVariableTypeTableAttribute(LocalVariableTypeTableAttribute *table) {
	unsigned int idx;
	if(table->local_variable_type_table != NULL) {
		for(idx = 0; idx < table->local_variable_type_table_length; idx++)
			free(table->local_variable_type_table[idx]);
		free(table->local_variable_type_table);
	}
	free(table);
}

void deleteElementValue(ElementValue *value) {
	extern void deleteAnnotationEntry(AnnotationEntry *);
	unsigned int idx;

	switch(value->tag) {
		case '@':
			deleteAnnotationEntry(value->value.annotation_value);
			break;
		case '[':
			if(value->value.array_value.values != NULL) {
				for(idx = 0; idx < value->value.array_value.num_values; idx++)
					deleteElementValue(value->value.array_value.values[idx]);
				free(value->value.array_value.values);
			}
			break;
	}
	free(value);
}

void deleteElementValuePairsEntry(ElementValuePairsEntry *entry) {
	if(entry->value != NULL)
		deleteElementValue(entry->value);
	free(entry);
}

void deleteAnnotationEntry(AnnotationEntry *entry) {
	unsigned int idx;
	if(entry->element_value_pairs != NULL) {
		for(idx = 0; idx < entry->num_element_value_pairs; idx++)
			deleteElementValuePairsEntry(entry->element_value_pairs[idx]);
		free(entry->element_value_pairs);
	}
	free(entry);
}

void deleteRuntimeAnnotationsAttribute(RuntimeAnnotationsAttribute *annot) {
	unsigned int idx;
	if(annot->annotations != NULL) {
		for(idx = 0; idx < annot->num_annotations; idx++)
			deleteAnnotationEntry(annot->annotations[idx]);
		free(annot->annotations);
	}
	free(annot);
}

void deleteParameterAnnotationsEntry(ParameterAnnotationsEntry *entry) {
	unsigned int idx;
	if(entry->annotations != NULL) {
		for(idx = 0; idx < entry->num_annotations; idx++)
			deleteAnnotationEntry(entry->annotations[idx]);
		free(entry->annotations);
	}
	free(entry);
}

void deleteRuntimeParameterAnnotationsAttribute(RuntimeParameterAnnotationsAttribute *annot) {
	unsigned int idx;
	if(annot->parameter_annotations != NULL) {
		for(idx = 0; idx < annot->num_parameters; idx++)
			deleteParameterAnnotationsEntry(annot->parameter_annotations[idx]);
		free(annot->parameter_annotations);
	}
	free(annot);
}

void deleteAnnotationDefaultAttribute(AnnotationDefaultAttribute *value) {
	if(value->default_value != NULL)
		deleteElementValue(value->default_value);
	free(value);
}

void deleteBootstrapMethodEntry(BootstrapMethodEntry *entry) {
	if(entry->bootstrap_arguments != NULL)
		free(entry->bootstrap_arguments);
	free(entry);
}

void deleteBootstrapMethodsAttribute(BootstrapMethodsAttribute *bootstrap) {
	unsigned int idx;
	if(bootstrap->bootstrap_methods != NULL) {
		for(idx = 0; idx < bootstrap->num_bootstrap_methods; idx++)
			deleteBootstrapMethodEntry(bootstrap->bootstrap_methods[idx]);
		free(bootstrap->bootstrap_methods);
	}
	free(bootstrap);
}

void deleteAttribute(AttributeInfo *info) {
	ConstantUtf8Info *name;
	if(info == NULL) return;

	name = info->name;
	if(name == NULL || name->bytes == NULL) {
		free(info);
		return;
	}

	// Code Attribute
	if(!strcmp("Code", (char *)name->bytes)) {
		deleteCodeAttribute((void *)info);
	} else
	// Stack Map Table Attribute
	if(!strcmp("StackMapTable", (char *)name->bytes)) {
		deleteStackMapTableAttribute((void *)info);
	} else
	// Exceptions Attribute
	if(!strcmp("Exceptions", (char *)name->bytes)) {
		deleteExceptionsAttribute((void *)info);
	} else
	// Inner Classes Attribute
	if(!strcmp("InnerClasses", (char *)name->bytes)) {
		deleteInnerClassesAttribute((void *)info);
	} else
	// Source Debug Extension Attribute
	if(!strcmp("SourceDebugExtension", (char *)name->bytes)) {
		free(((SourceDebugExtensionAttribute *)info)->debug_extension);
		free(info);
	} else
	// Line Number Table Attribute
	if(!strcmp("LineNumberTable", (char *)name->bytes)) {
		deleteLineNumberTableAttribute((void *)info);
	} else
	// Local Variable Table Attribute
	if(!strcmp("LocalVariableTable", (char *)name->bytes)) {
		deleteLocalVariableTableAttribute((void *)info);
	} else
	// Local Variable Type Table Attribute
	if(!strcmp("LocalVariableTypeTable", (char *)name->bytes)) {
		deleteLocalVariableTypeTableAttribute((void *)info);
	} else
	// Runtime Visible Annotations Attribute
	if(!strcmp("RuntimeVisibleAnnotations", (char *)name->bytes)) {
		deleteRuntimeAnnotationsAttribute((void *)info);
	} else
	// Runtime Invisible Annotations Attribute
	if(!strcmp("RuntimeInvisibleAnnotations", (char *)name->bytes)) {
		deleteRuntimeAnnotationsAttribute((void *)info);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeVisibleParameterAnnotations", (char *)name->bytes)) {
		deleteRuntimeParameterAnnotationsAttribute((void *)info);
	} else
	// Runtime Visible Parameter Annotations Attribute
	if(!strcmp("RuntimeInvisibleParameterAnnotations", (char *)name->bytes)) {
		deleteRuntimeParameterAnnotationsAttribute((void *)info);
	} else
	// Annotation Default Attribute
	if(!strcmp("AnnotationDefault", (char *)name->bytes)) {
		deleteAnnotationDefaultAttribute((void *)info);
	} else
	// Bootstrap Methods Attribute
	if(!strcmp("BootstrapMethods", (char *)name->bytes)) {
		deleteBootstrapMethodsAttribute((void *)info);
	} else {
		free(info);
	}
}
