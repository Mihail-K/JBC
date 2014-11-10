
# include "List.h"
# include "Debug.h"
# include "Zalloc.h"
# include "ClassFile.h"

# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

void encodeConstantPool(ClassFile *classFile, ClassBuilder *builder) {
	unsigned int length;

	buildNextShort(builder, length = classFile->constant_pool.size());
	debug_printf(level1, "Constant Pool Count : %d.\n", length);

	for(unsigned idx = 1; idx < length; idx++) {
		ConstantInfo *info;
		debug_printf(level2, "Constant %d :\n", idx);
		info = static_cast(ConstantInfo *, classFile->constant_pool[idx]);

		encodeConstant(builder, info);
		if(isLongConstant(info)) {
			debug_printf(level2, "Long Constant; Skipping index.\n");
			idx++;
		}
	}
}

void encodeThisClass(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->this_class != NULL) {
		uint16_t index =  classFile->this_class->index;
		debug_printf(level3, "This Class : %d.\n", index);
		buildNextShort(builder, index);
	} else {
		// No ThisClass Entry
		debug_printf(level3, "This Class : <NULL>.\n");
		buildNextShort(builder, 0);
	}
}

void encodeSuperClass(ClassFile *classFile, ClassBuilder *builder) {
	if(classFile->super_class != NULL) {
		uint16_t index =  classFile->super_class->index;
		debug_printf(level3, "Super Class : %d.\n", index);
		buildNextShort(builder, index);
	} else {
		// No SuperClass Entry
		debug_printf(level3, "Super Class : <NULL>.\n");
		buildNextShort(builder, 0);
	}
}

void encodeInterfaces(ClassFile *classFile, ClassBuilder *builder) {
	unsigned length;

	length = classFile->interfaces.size();
	debug_printf(level1, "Interfaces Count : %d.\n", length);
	buildNextShort(builder, length);

	for(unsigned idx = 0; idx < length; idx++) {
		ConstantInfo *info = (ConstantInfo *)classFile->interfaces[idx];
		debug_printf(level2, "Interface %d : %d.\n", idx, info->index);
		buildNextShort(builder, info->index);
	}
}

int encodeClassData(ClassFile *classFile, ClassBuilder *builder) {
	unsigned int length;

	debug_printf(level0, "Magic : %#X.\n", classFile->magic);
	debug_printf(level0, "Major Version : %d.\n", classFile->major_version);
	debug_printf(level0, "Minor Version : %d.\n", classFile->minor_version);

	buildNextInt(builder, classFile->magic);
	buildNextShort(builder, classFile->major_version);
	buildNextShort(builder, classFile->minor_version);

	encodeConstantPool(classFile, builder);

	debug_printf(level3, "Access Flags : %#X.\n", classFile->access_flags);
	buildNextShort(builder, classFile->access_flags);

	encodeThisClass(classFile, builder);
	encodeSuperClass(classFile, builder);
	encodeInterfaces(classFile, builder);

	// Fields Table
	length = classFile->fields.size();
	debug_printf(level1, "Fields Count : %d.\n", length);
	buildNextShort(builder, length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Field %d :\n", idx);
		encodeField(classFile, builder, static_cast(MemberInfo *,
				classFile->fields[idx]));
	}

	// Methods Table
	length = classFile->methods.size();
	debug_printf(level1, "Methods Count : %d.\n", length);
	buildNextShort(builder, length);

	for(unsigned idx = 0; idx < length; idx++) {
		debug_printf(level2, "Method %d :\n", idx);
		encodeMethod(classFile, builder, static_cast(MemberInfo *,
				classFile->methods[idx]));
	}

	// Attributes Table
	length = classFile->attributes.size();
	debug_printf(level1, "Attributes Count : %d.\n", length);
	buildNextShort(builder, length);

	for(unsigned  idx = 0; idx < length; idx++) {
		debug_printf(level2, "Attribute %d :\n", idx);
		encodeAttribute(classFile, builder, static_cast(AttributeInfo *,
				classFile->attributes[idx]));
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
