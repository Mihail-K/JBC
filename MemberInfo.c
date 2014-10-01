
# include "Debug.h"
# include "Zalloc.h"
# include "MemberInfo.h"
# include "AttributeInfo.h"

MemberInfo *visitField(ClassFile *classFile, ClassBuffer *buffer) {
	int idx;
	FieldInfo *field = zalloc(sizeof(FieldInfo));

	field->member_type = MT_FIELD;
	field->access_flags = bufferNextShort(buffer);

	// Field Name
	field->name_index = bufferNextShort(buffer);
	field->name = (void *)getConstant(classFile, field->name_index);

	// Field Descriptor
	field->descriptor_index = bufferNextShort(buffer);
	field->descriptor = (void *)getConstant(classFile, field->descriptor_index);

	// Field Attributes Table
	field->attributes_count = bufferNextShort(buffer);
	field->attributes = zalloc(sizeof(AttributeInfo *) * field->attributes_count);

	for(idx = 0; idx < field->attributes_count; idx++) {
		field->attributes[idx] = visitAttribute(classFile, buffer);
	}

	return field;
}

MemberInfo *visitMethod(ClassFile *classFile, ClassBuffer *buffer) {
	return NULL;
}
