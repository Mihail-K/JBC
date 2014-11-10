
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

StackMapFrame *decodeStackMapFrame(ClassBuffer *buffer, ClassFile *classFile) {
	uint8_t tag = buffer->NextByte();

	debug_printf(level3, "Decoding Stack Frame type : %d.\n", tag);

	// Stack Map Same Frame
	if(tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		return new StackMapFrame(tag);
	} else
	// Stack Map Same Locals 1
	if(tag >= 64 && tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		return (new StackMapItemFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Reserved Values
	if(tag >= 128 && tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		return (new StackMapExtFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Chop Frame
	if(tag >= 248 && tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		return (new StackMapOffFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Same Frame Extended
	if(tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		return (new StackMapOffFrame(tag))->DecodeFrame(buffer, classFile);
	} else
	// Stack Map Append Frame
	if(tag >= 252 && tag <= 254) {
		debug_printf(level3, "Stack Map append frame.\n");
		return (new StackMapListFrame(tag))->DecodeFrame(buffer, classFile);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		return (new StackMapFullFrame(tag))->DecodeFrame(buffer, classFile);
	}
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

int encodeStackMapFrame(
		ClassBuilder *builder, ClassFile *classFile, StackMapFrame *frame) {
	builder->NextByte(frame->tag);

	// Stack Map Same Frame
	if(frame->tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
	} else
	// Stack Map Same Locals 1
	if(frame->tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		frame->EncodeFrame(builder, classFile);
	} else
	// Reserved Values
	if(frame->tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", frame->tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(frame->tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		frame->EncodeFrame(builder, classFile);
	} else
	// Stack Map Chop Frame
	if(frame->tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		frame->EncodeFrame(builder, classFile);
	} else
	// Stack Map Same Frame Extended
	if(frame->tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		frame->EncodeFrame(builder, classFile);
	} else
	// Stack Map Append Frame
	if(frame->tag <= 254) {
		debug_printf(level3, "Stack Map append frame.\n");
		frame->EncodeFrame(builder, classFile);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		frame->EncodeFrame(builder, classFile);
	}

	return 0;
}

/* Stack Map Frame destructors */

StackMapFrame::~StackMapFrame() {
	debug_printf(level3, "Deleting Stack Map Frame.\n");
}

StackMapItemFrame::~StackMapItemFrame() {
	debug_printf(level3, "Deleting Stack Map Item Frame.\n");
	if(stack != NULL) {
		delete stack;
	}
}

StackMapExtFrame::~StackMapExtFrame() {
	debug_printf(level3, "Deleting Stack Map Ext Frame.\n");
	if(stack != NULL) {
		delete stack;
	}
}

StackMapListFrame::~StackMapListFrame() {
	debug_printf(level3, "Deleting Stack Map List Frame.\n");
	if(stack != NULL) {
		for(unsigned idx = 0; idx < tag - 251u; idx++)
			delete stack[idx];
		delete stack;
	}
}

StackMapFullFrame::~StackMapFullFrame() {
	debug_printf(level3, "Deleting Stack Map Full Frame.\n");
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
