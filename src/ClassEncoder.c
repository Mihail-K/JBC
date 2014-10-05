
# include "List.h"
# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"

# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

void encodeConstantPool(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->constant_pool != NULL) {
		unsigned int idx, length;

		buildNextShort(builder, length = listSize(classFile->constant_pool));
		debug_printf(level1, "Constant Pool Count : %d.\n", length);

		for(idx = 1; idx < length; idx++) {
			ConstantInfo *info;
			debug_printf(level2, "Constant %d :\n", idx);
			info = listGet(classFile->constant_pool, idx);

			encodeConstant(builder, info);
			if(isLongConstant(info)) {
				debug_printf(level2, "Long Constant; Skipping index.\n");
				idx++;
			}
		}
	}
}

void encodeThisClass(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->this_class != NULL) {
		uint16_t index =  classFile->this_class->index;
		debug_printf(level3, "This Class : %d.\n", index);
		buildNextShort(builder, index);
	}
}

void encodeSuperClass(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->super_class != NULL) {
		uint16_t index =  classFile->super_class->index;
		debug_printf(level3, "Super Class : %d.\n", index);
		buildNextShort(builder, index);
	}
}

void encodeInterfaces(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->interfaces != NULL) {
		unsigned int idx, length;

		length = listSize(classFile->interfaces);
		debug_printf(level1, "Interfaces Count : %d.\n", length);
		buildNextShort(builder, length);

		for(idx = 0; idx < length; idx++) {
			ConstantInfo *info = listGet(classFile->interfaces, idx);
			debug_printf(level2, "Interface %d : %d.\n", idx, info->index);
			buildNextShort(builder, info->index);
		}
	}
}

int encodeClassData(ClassFile *classFile, ClassBuilder *builder) {
	unsigned int idx, length;

	debug_printf(level0, "Magic : %#X.\n", classFile->magic);
	debug_printf(level0, "Major Version : %d.\n", classFile->major_version);
	debug_printf(level0, "Minor Version : %d.\n", classFile->minor_version);

	buildNextInt(builder, classFile->magic);
	buildNextInt(builder, classFile->major_version);
	buildNextInt(builder, classFile->minor_version);

	encodeConstantPool(classFile, builder);

	debug_printf(level3, "Access Flags : %#X.\n", classFile->access_flags);
	buildNextShort(builder, classFile->access_flags);

	encodeThisClass(classFile, builder);
	encodeSuperClass(classFile, builder);
	encodeInterfaces(classFile, builder);

	// Fields Table
	if(classFile->fields != NULL) {
		length = listSize(classFile->fields);
		debug_printf(level1, "Fields Count : %d.\n", length);

		for(idx = 0; idx < length; idx++) {
			debug_printf(level2, "Field %d :\n", idx);
			encodeField(classFile, builder, listGet(classFile->fields, idx));
		}
	}

	// Methods Table
	if(classFile->methods != NULL) {
		length = listSize(classFile->methods);
		debug_printf(level1, "Methods Count : %d.\n", length);

		for(idx = 0; idx < length; idx++) {
			debug_printf(level2, "Method %d :\n", idx);
			encodeMethod(classFile, builder, listGet(classFile->methods, idx));
		}
	}

	// Attributes Table
	if(classFile->attributes != NULL) {
		length = listSize(classFile->attributes);
		debug_printf(level1, "Attributes Count : %d.\n", length);

		for(idx = 0; idx < length; idx++) {
			debug_printf(level2, "Attribute %d :\n", idx);
			encodeAttribute(classFile, builder, listGet(classFile->attributes, idx));
		}
	}

	return 0;
}

int encodeClassFile(FILE *source, ClassFile *classFile) {
	int result;

	debug_printf(level0, "Creating Class builder.\n");
	ClassBuilder *builder = createBuilder(source);
	debug_printf(level0, "Encoding Class file :\n");
	result = encodeClassData(classFile, builder);
	debug_printf(level0, "Finished Class file.\n");
	return result;
}
