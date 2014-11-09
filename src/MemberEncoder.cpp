
# include "List.h"
# include "Debug.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

int encodeMember(ClassFile *classFile, ClassBuilder *builder, MemberInfo *info) {
	unsigned int idx, length;

	buildNextShort(builder, info->access_flags);
	buildNextShort(builder, info->name->index);
	buildNextShort(builder, info->descriptor->index);

	buildNextShort(builder, length = listSize(info->attributes));
	debug_printf(level1, "Member Attributes Count : %d.\n", length);

	for(idx = 0; idx < length; idx++) {
		debug_printf(level2, "Member Attribute %d :\n", idx);
		encodeAttribute(classFile, builder, static_cast(AttributeInfo *,
				listGet(info->attributes, idx)));
	}

	return 0;
}
