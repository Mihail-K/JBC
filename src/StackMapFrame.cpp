
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
		::EncodeInfo(ClassBuilder *builder, ClassFile *) {
	if(object != NULL)
		builder->NextShort(object->index);
	return this;
}

UninitializedVariableInfo *UninitializedVariableInfo
		::EncodeInfo(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(offset);
	return this;
}

void EncodeVariableInfo(
		ClassBuilder *builder, ClassFile *classFile, VariableInfo *info) {
	builder->NextByte(info->tag);
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

StackMapOffFrame *StackMapOffFrame
		::EncodeFrame(ClassBuilder *builder, ClassFile *) {
	builder->NextShort(offset_delta);
	return this;
}

StackMapItemFrame *StackMapItemFrame
		::EncodeFrame(ClassBuilder *builder, ClassFile *classFile) {
	EncodeVariableInfo(builder, classFile, stack);
	return this;
}

StackMapExtFrame *StackMapExtFrame
		::EncodeFrame(ClassBuilder *builder, ClassFile *classFile) {
	builder->NextShort(offset_delta);
	EncodeVariableInfo(builder, classFile, stack);

	return this;
}

StackMapListFrame *StackMapListFrame
		::EncodeFrame(ClassBuilder *builder, ClassFile *classFile) {
	builder->NextShort(offset_delta);
	for(unsigned idx = 0; idx < (tag - 251u); idx++) {
		EncodeVariableInfo(builder, classFile, stack[idx]);
	}

	return this;
}

StackMapFullFrame *StackMapFullFrame
		::EncodeFrame(ClassBuilder *builder, ClassFile *classFile) {
	uint16_t length;

	builder->NextShort(offset_delta);

	// Stack Frame Locals
	length = locals.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		EncodeVariableInfo(builder, classFile, locals[idx]);
	}

	// Stack Frame Items
	length = stack.size();
	builder->NextShort(length);

	for(unsigned idx = 0; idx < length; idx++) {
		EncodeVariableInfo(builder, classFile, stack[idx]);
	}

	return this;
}

/* Stack Map Frame destructors */

StackMapItemFrame::~StackMapItemFrame() {
	if(stack != NULL) {
		delete stack;
	}
}

StackMapExtFrame::~StackMapExtFrame() {
	if(stack != NULL) {
		delete stack;
	}
}

StackMapListFrame::~StackMapListFrame() {
	if(stack != NULL) {
		for(unsigned idx = 0; idx < tag - 251u; idx++)
			delete stack[idx];
		delete stack;
	}
}

StackMapFullFrame::~StackMapFullFrame() {
	if(!locals.empty()) {
		for(std::vector<VariableInfo *>::iterator itr = locals.begin();
				itr != locals.end(); itr++) {
			delete *itr;
		}
	}

	if(!stack.empty()) {
		for(std::vector<VariableInfo *>::iterator itr = stack.begin();
				itr != stack.end(); itr++) {
			delete *itr;
		}
	}
}
