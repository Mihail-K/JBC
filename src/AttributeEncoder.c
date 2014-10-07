
# include "Debug.h"

# include "ClassFile.h"
# include "AttributeInfo.h"

# define ignore_unused(x) ((void)x)

int encodeConstantValueAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	ConstantValueAttribute *constant = (void *)info;

	ignore_unused(classFile);
	buildNextShort(builder, constant->name->index);
	buildNextInt(builder, constant->attribute_length);
	buildNextShort(builder, constant->constant_value->index);

	return 0;
}

int encodeExceptionTableEntry(
		ClassFile *classFile, ClassBuilder *builder, ExceptionTableEntry *entry) {
	ignore_unused(classFile);
	buildNextShort(builder, entry->start_pc);
	buildNextShort(builder, entry->end_pc);
	buildNextShort(builder, entry->handler_pc);
	buildNextShort(builder, entry->catch_type);

	return 0;
}

int encodeCodeAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	CodeAttribute *code = (void *)info;

	buildNextShort(builder, code->name->index);
	buildNextInt(builder, code->attribute_length);
	
	// Maximums
	buildNextShort(builder, code->max_stack);
	buildNextShort(builder, code->max_locals);

	// Code
	buildNextInt(builder, code->code_length);
	for(idx = 0; idx < code->code_length; idx++) {
		buildNextByte(builder, code->code[idx]);
	}

	// Exception Table
	length = listSize(code->exception_table);
	buildNextShort(builder, length);
	for(idx = 0; idx < length; idx++) {
		encodeExceptionTableEntry(classFile, builder, listGet(
				code->exception_table, idx));
	}

	// Attribute Table
	length = listSize(code->attributes);
	buildNextShort(builder, length);
	for(idx = 0; idx < length; idx++) {
		encodeAttribute(classFile, builder, listGet(
				code->attributes, idx));
	}

	return 0;
}

int encodeVerificationTypeInfo(
		ClassFile *classFile, ClassBuilder *builder, VerificationTypeInfo *info) {
	ignore_unused(classFile);
	buildNextByte(builder, info->tag);
	switch(info->tag) {
		case 0:
			debug_printf(level3, "Top variable info.\n");
			break;
		case 1:
			debug_printf(level3, "Integer variable info.\n");
			break;
		case 2:
			debug_printf(level3, "Float variable info.\n");
			break;
		case 3:
			debug_printf(level3, "Double variable info.\n");
			break;
		case 4:
			debug_printf(level3, "Long variable info.\n");
			break;
		case 5:
			debug_printf(level3, "Null variable info.\n");
			break;
		case 6:
			debug_printf(level3, "Uninitialized this variable info.\n");
			break;
		case 7:
			debug_printf(level3, "Object variable info.\n");
			buildNextShort(builder, info->object_variable_info.object->index);
			break;
		case 8:
			debug_printf(level3, "Uninitialized variable info.\n");
			buildNextShort(builder, info->uninitialized_variable_info.offset);
			break;
		default:
			fprintf(stderr, "Unknown verification type (ID : %d)!\n", info->tag);
			exit(EXIT_FAILURE);
	}

	return 0;
}

int encodeStackMapOffFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapOffFrame *offFrame = (void *)frame;

	ignore_unused(classFile);
	buildNextShort(builder, offFrame->offset_delta);

	return 0;
}

int encodeStackMapItemFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapItemFrame *itemFrame = (void *)frame;

	encodeVerificationTypeInfo(classFile, builder, itemFrame->stack);

	return 0;
}

int encodeStackMapExtFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	StackMapExtFrame *extFrame = (void *)frame;

	buildNextShort(builder, extFrame->offset_delta);
	encodeVerificationTypeInfo(classFile, builder, extFrame->stack);

	return 0;
}

int encodeStackMapListFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx;
	StackMapListFrame *listFrame = (void *)frame;

	buildNextShort(builder, listFrame->offset_delta);

	for(idx = 0; idx < (listFrame->tag - 251u); idx++) {
		encodeVerificationTypeInfo(classFile, builder, listFrame->stack[idx]);
	}

	return 0;
}

int encodeStackMapFullFrame(ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	unsigned int idx, length;
	StackMapFullFrame *fullFrame = (void *)frame;

	buildNextShort(builder, fullFrame->offset_delta);

	// Stack Frame Locals
	length = listSize(fullFrame->locals);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeVerificationTypeInfo(classFile, builder, listGet(
				fullFrame->locals, idx));
	}

	// Stack Frame Items
	length = listSize(fullFrame->stack);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeVerificationTypeInfo(classFile, builder, listGet(
				fullFrame->stack, idx));
	}

	return 0;
}

int encodeStackMapFrame(
		ClassFile *classFile, ClassBuilder *builder, StackMapFrame *frame) {
	buildNextByte(builder, frame->tag);

	// Stack Map Same Frame
	if(frame->tag <= 63) {
		debug_printf(level3, "Stack Map same frame.\n");
		return 0;
	} else
	// Stack Map Same Locals 1
	if(frame->tag <= 127) {
		debug_printf(level3, "Stack Map same locals.\n");
		return encodeStackMapItemFrame(classFile, builder, frame);
	} else
	// Reserved Values
	if(frame->tag <= 246) {
		fprintf(stderr, "Stack Frame tag (ID : %d) is reveserved!\n", frame->tag);
		exit(EXIT_FAILURE);
	} else
	// Stack Map Same Locals 1 Extended
	if(frame->tag == 247) {
		debug_printf(level3, "Stack Map same locals extended.\n");
		return encodeStackMapExtFrame(classFile, builder, frame);
	} else
	// Stack Map Chop Frame
	if(frame->tag <= 250) {
		debug_printf(level3, "Stack Map chop frame.\n");
		return encodeStackMapOffFrame(classFile, builder, frame);
	} else
	// Stack Map Same Frame Extended
	if(frame->tag == 251) {
		debug_printf(level3, "Stack Map same frame extended.\n");
		return encodeStackMapOffFrame(classFile, builder, frame);
	} else
	// Stack Map Append Frame
	if(frame->tag <= 254) {
		debug_printf(level3, "Stack Map append frame.\n");
		return encodeStackMapListFrame(classFile, builder, frame);
	}
	// Stack Map Full Frame
	else {
		debug_printf(level3, "Stack Map full frame.\n");
		return encodeStackMapFullFrame(classFile, builder, frame);
	}
}

int encodeStackMapTableAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	StackMapTableAttribute *table = (void *)info;

	buildNextShort(builder, table->name->index);
	buildNextInt(builder, table->attribute_length);

	length = listSize(table->entries);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		encodeStackMapFrame(classFile, builder, listGet(
				table->entries, idx));
	}

	return 0;
}

int encodeExceptionsAttribute(
		ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	unsigned int idx, length;
	ExceptionsAttribute *except = (void *)info;

	ignore_unused(classFile);
	length = listSize(except->exception_table);
	buildNextShort(builder, length);

	for(idx = 0; idx < length; idx++) {
		ConstantInfo *constant = listGet(except->exception_table, idx);
		buildNextShort(builder, constant->index);
	}

	return 0;
}

int encodeAttribute(ClassFile *classFile, ClassBuilder *builder, AttributeInfo *info) {
	// TODO
	((void)classFile);
	((void)builder);
	((void)info);
	return 0;
}
