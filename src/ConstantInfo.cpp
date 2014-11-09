
# include <stdio.h>
# include <string.h>

# include "Zalloc.h"
# include "ConstantInfo.h"

ConstantInfo *createConstant(uint8_t type) {
	ConstantInfo *info;

	switch(type) {
		case CONSTANT_UTF8:
			info = (ConstantInfo *)NEW(ConstantUtf8Info);
			memset(info, 0, sizeof(ConstantUtf8Info));
			break;
		case CONSTANT_INTEGER:
			info = (ConstantInfo *)NEW(ConstantIntegerInfo);
			memset(info, 0, sizeof(ConstantIntegerInfo));
			break;
		case CONSTANT_FLOAT:
			info = (ConstantInfo *)NEW(ConstantFloatInfo);
			memset(info, 0, sizeof(ConstantFloatInfo));
			break;
		case CONSTANT_LONG:
			info = (ConstantInfo *)NEW(ConstantLongInfo);
			memset(info, 0, sizeof(ConstantLongInfo));
			break;
		case CONSTANT_DOUBLE:
			info = (ConstantInfo *)NEW(ConstantDoubleInfo);
			memset(info, 0, sizeof(ConstantDoubleInfo));
			break;
		case CONSTANT_CLASS:
			info = (ConstantInfo *)NEW(ConstantClassInfo);
			memset(info, 0, sizeof(ConstantClassInfo));
			break;
		case CONSTANT_STRING:
			info = (ConstantInfo *)NEW(ConstantStringInfo);
			memset(info, 0, sizeof(ConstantStringInfo));
			break;
		case CONSTANT_FIELD_REF:
		case CONSTANT_METHOD_REF:
		case CONSTANT_INTERFACE_METHOD_REF:
			info = (ConstantInfo *)NEW(ConstantFieldRefInfo);
			memset(info, 0, sizeof(ConstantFieldRefInfo));
			break;
		case CONSTANT_NAME_AND_TYPE:
			info = (ConstantInfo *)NEW(ConstantNameAndTypeInfo);
			memset(info, 0, sizeof(ConstantNameAndTypeInfo));
			break;
		case CONSTANT_METHOD_HANDLE:
			info = (ConstantInfo *)NEW(ConstantMethodHandleInfo);
			memset(info, 0, sizeof(ConstantMethodHandleInfo));
			break;
		case CONSTANT_METHOD_TYPE:
			info = (ConstantInfo *)NEW(ConstantMethodTypeInfo);
			memset(info, 0, sizeof(ConstantMethodTypeInfo));
			break;
		case CONSTANT_INVOKE_DYNAMIC:
			info = (ConstantInfo *)NEW(ConstantInvokeDynamicInfo);
			memset(info, 0, sizeof(ConstantInvokeDynamicInfo));
			break;
		default:
			return NULL;
	}

	info->tag = type;
	return info;
}

void deleteConstant(ConstantInfo *info) {
	if(info == NULL) return;
	if(info->tag == CONSTANT_UTF8)
		free(((ConstantUtf8Info *)info)->bytes);
	free(info);
}
