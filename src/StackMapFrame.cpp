
# include "Debug.h"
# include "ClassFile.h"
# include "StackMapFrame.h"

/* Stack Map Frame encode */

ObjectVariableInfo *ObjectVariableInfo
		::DecodeInfo(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t index = buffer->NextShort();
	object = static_cast<ConstantClassInfo *>(
			classFile->constant_pool[index]);
	return this;
}

UninitializedVariableInfo *UninitializedVariableInfo
		::DecodeInfo(ClassBuffer *buffer, ClassFile *) {
	offset = buffer->NextShort();
	return this;
}

VariableInfo *DecodeVariableInfo(ClassBuffer *buffer, ClassFile *classFile) {
	uint8_t tag = buffer->NextByte();

	switch(tag) {
		case 0:
			debug_printf(level3, "Top variable info.\n");
			return new VariableInfo(tag);
		case 1:
			debug_printf(level3, "Integer variable info.\n");
			return new VariableInfo(tag);
		case 2:
			debug_printf(level3, "Float variable info.\n");
			return new VariableInfo(tag);
		case 3:
			debug_printf(level3, "Double variable info.\n");
			return new VariableInfo(tag);
		case 4:
			debug_printf(level3, "Long variable info.\n");
			return new VariableInfo(tag);
		case 5:
			debug_printf(level3, "Null variable info.\n");
			return new VariableInfo(tag);
		case 6:
			debug_printf(level3, "Uninitialized this variable info.\n");
			return new VariableInfo(tag);
		case 7:
			debug_printf(level3, "Object variable info.\n");
			return (new ObjectVariableInfo(tag))->DecodeInfo(buffer, classFile);
		case 8:
			debug_printf(level3, "Uninitialized variable info.\n");
			return (new UninitializedVariableInfo(tag))->DecodeInfo(buffer, classFile);
		default:
			fprintf(stderr, "Unknown verification type (ID : %d)!\n", tag);
			exit(EXIT_FAILURE);
	}
}

/* Verification Type encode */

ObjectVariableInfo *ObjectVariableInfo
		::EncodeInfo(ClassBuffer *builder, ClassFile *) {
	if(object != NULL)
		bulder->NextShort(object->index);
	return this;
}

UninitializedVariableInfo *UninitializedVariableInfo
		::EncodeInfo(ClassBuffer *builder, ClassFile *) {
	builder->NextShort(offset);
	return this;
}

void EncodeVariableInfo(
		ClassFile *classFile, ClassBuilder *builder, VariableInfo *info) {
	builder->NextByte(tag);
	info->EncodeInfo(builder, classFile);
}

/* Stack Map Frame decode */

StackMapOffFrame *StackMapOffFrame
		::DecodeFrame(ClassBuffer *buffer, ClassFile *classFile) {
	offset_delta = buffer->NextShort();
	((void)classFile);
	return this;
}

StackMapItemFrame *StackMapItemFrame
		::DecodeFrame(ClassBuffer *buffer, ClassFile *classFile) {
	stack = DecodeVariableInfo(buffer, classFile);
	return this;
}

StackMapExtFrame *StackMapExtFrame
		::DecodeFrame(ClassBuffer *buffer, ClassFile *classFile) {
	offset_delta = buffer->NextShort();
	stack = DecodeVariableInfo(buffer, classFile);
	return this;
}

StackMapListFrame *StackMapListFrame
		::DecodeFrame(ClassBuffer *buffer, ClassFile *classFile) {
	offset_delta = buffer->NextShort();
	stack = new VariableInfo *[tag - 251u];

	for(unsigned idx = 0; idx < tag - 251u; idx++)
		stack[idx] = DecodeVariableInfo(buffer, classFile);
	return this;
}

StackMapFullFrame *StackMapFullFrame
		::DecodeFrame(ClassBuffer *buffer, ClassFile *classFile) {
	uint16_t length;

	offset_delta = buffer->NextShort();

	// Stack Frame Locals
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		locals.push_back(DecodeVariableInfo(buffer, classFile));
	}

	// Stack Frame Items
	length = buffer->NextShort();
	for(unsigned idx = 0; idx < length; idx++) {
		stack.push_back(DecodeVariableInfo(buffer, classFile));
	}

	return this;
}

/* Stack Map Frame encode */

int encodeStackMapOffFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapOffFrame *offFrame = (StackMapOffFrame *)frame;
	builder->NextShort(offFrame->offset_delta);

	ignore_unused(classFile);
	return 0;
}

int encodeStackMapItemFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapItemFrame *itemFrame = (StackMapItemFrame *)frame;
	EncodeVariableInfo(builder, classFile, itemFrame->stack);
	return 0;
}

int encodeStackMapExtFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapExtFrame *extFrame = (StackMapExtFrame *)frame;

	builder->NextShort(extFrame->offset_delta);
	EncodeVariableInfo(builder, classFile, extFrame->stack);

	return 0;
}

int encodeStackMapListFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx;
	StackMapListFrame *listFrame = (StackMapListFrame *)frame;

	builder->NextShort(listFrame->offset_delta);
	for(idx = 0; idx < (listFrame->tag - 251u); idx++) {
		EncodeVariableInfo(builder, EncodeVariableInfo, listFrame->stack[idx]);
	}

	return 0;
}

int encodeStackMapFullFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx, length;
	StackMapFullFrame *fullFrame = (StackMapFullFrame *)frame;

	builder->NextShort(fullFrame->offset_delta);

	// Stack Frame Locals
	length = listSize(fullFrame->locals);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		EncodeVariableInfo(builder, classFile, static_cast<
				VerificationTypeInfo *>(listGet(fullFrame->locals, idx)));
	}

	// Stack Frame Items
	length = listSize(fullFrame->stack);
	builder->NextShort(length);

	for(idx = 0; idx < length; idx++) {
		EncodeVariableInfo(builder, classFile, static_cast<
				VerificationTypeInfo *>(listGet(fullFrame->stack, idx)));
	}

	return 0;
}
