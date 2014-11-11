
# include <stdlib.h>
# include <string.h>

# include "Debug.h"
# include "ClassFile.h"
# include "AttributeInfo.h"

namespace JBC {

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

	if(!local_variable_table.empty()) {
		for(std::vector<LocalVariableTableEntry *>::iterator itr = local_variable_table.begin();
				itr != local_variable_table.end(); itr++) {
			delete *itr;
		}
	}
}

LocalVariableTypeTableAttribute::~LocalVariableTypeTableAttribute() {
	debug_printf(level3, "Deleting Local Variable Type Table Attribute.\n");

	if(!local_variable_type_table.size()) {
		for(std::vector<LocalVariableTypeTableEntry *>::iterator itr = local_variable_type_table
				.begin(); itr != local_variable_type_table.end(); itr++) {
			delete *itr;
		}
	}
}

RuntimeAnnotationsAttribute::~RuntimeAnnotationsAttribute() {
	debug_printf(level3, "Deleting Runtime Annotations Attribute.\n");

	if(!annotations.empty()) {
		for(std::vector<AnnotationEntry *>::iterator itr = annotations.begin();
				itr != annotations.end(); itr++) {
			delete *itr;
		}
	}
}

ParameterAnnotationsEntry::~ParameterAnnotationsEntry() {
	debug_printf(level3, "Deleting Parameter Annotations Entry.\n");

	if(!annotations.empty()) {
		for(std::vector<AnnotationEntry *>::iterator itr = annotations.begin();
				itr != annotations.end(); itr++) {
			delete *itr;
		}
	}
}

RuntimeParameterAnnotationsAttribute::~RuntimeParameterAnnotationsAttribute() {
	debug_printf(level3, "Deleting Runtime Parameter Annotations Attribute.\n");

	if(!parameter_annotations.empty()) {
		for(std::vector<ParameterAnnotationsEntry *>::iterator itr = parameter_annotations
				.begin(); itr != parameter_annotations.end(); itr++) {
			delete *itr;
		}
	}
}

AnnotationDefaultAttribute::~AnnotationDefaultAttribute() {
	debug_printf(level3, "Deleting Annotation Default Attribute.\n");

	if(default_value != NULL) {
		delete default_value;
	}
}

BootstrapMethodEntry::~BootstrapMethodEntry() {
	debug_printf(level3, "Deleting Bootstrap Method Entry.\n");

	if(!bootstrap_arguments.empty()) {
		for(std::vector<ConstantInfo *>::iterator itr = bootstrap_arguments.begin();
				itr != bootstrap_arguments.end(); itr++) {
			delete *itr;
		}
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

} /* JBC */
