
# include <stdio.h>
# include <string.h>

# include "Debug.h"
# include "Zalloc.h"
# include "ClassBuffer.h"

# include "ClassFile.h"
# include "MemberInfo.h"
# include "ConstantInfo.h"
# include "AttributeInfo.h"

ClassFile *createClassFile() {
	ClassFile *classFile = NEW(ClassFile);
	return static_cast(ClassFile *, memset(classFile, 0, sizeof(ClassFile)));
}

void deleteClassFile(ClassFile *classFile) {
	if(classFile == NULL) return;
	if(classFile->interfaces != NULL) {
		deleteList(classFile->interfaces, NULL);
	}
	if(classFile->fields != NULL) {
		deleteList(classFile->fields, (void(*)(void *))deleteMember);
	}
	if(classFile->methods != NULL) {
		deleteList(classFile->methods, (void(*)(void *))deleteMember);
	}
	if(classFile->attributes != NULL) {
		deleteList(classFile->attributes, (void(*)(void *))deleteAttribute);
	}
	if(classFile->constant_pool != NULL) {
		deleteList(classFile->constant_pool, (void(*)(void *))deleteConstant);
	}
	DELETE(classFile);
}
