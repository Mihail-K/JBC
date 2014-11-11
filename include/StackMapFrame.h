# ifndef __STACKMAPFRAME_H__
# define __STACKMAPFRAME_H__

# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"
# include "ConstantInfo.h"

namespace JBC {

struct VariableInfo {
	uint8_t		tag;

	VariableInfo() {
	}

	VariableInfo(uint8_t tag)
		: tag(tag) {
	}

	virtual
	~VariableInfo() {
	}

	virtual inline
	VariableInfo *DecodeInfo(ClassBuffer *, ClassFile *) {
		// Placeholder prototype for children.
		return this;
	}

	virtual inline
	VariableInfo *EncodeInfo(ClassBuilder *, ClassFile *) {
		// Placeholder prototype for children.
		return this;
	}
};

struct ObjectVariableInfo
		: public VariableInfo {
	ConstantClassInfo *object;

	ObjectVariableInfo() {
	}

	ObjectVariableInfo(uint8_t tag)
		: VariableInfo(tag) {
	}

	ObjectVariableInfo *DecodeInfo(ClassBuffer *buffer, ClassFile *classFile);

	ObjectVariableInfo *EncodeInfo(ClassBuilder *builder, ClassFile *classFile);
};

struct UninitializedVariableInfo
		: public VariableInfo {
	uint16_t offset;

	UninitializedVariableInfo() {
	}

	UninitializedVariableInfo(uint8_t tag)
		: VariableInfo(tag) {
	}

	UninitializedVariableInfo *DecodeInfo(ClassBuffer *buffer, ClassFile *classFile);

	UninitializedVariableInfo *EncodeInfo(ClassBuilder *builder, ClassFile *classFile);
};

VariableInfo *DecodeVariableInfo(ClassBuffer *buffer, ClassFile *classFile);

void EncodeVariableInfo(ClassBuilder *builder, ClassFile *classFile, VariableInfo *info);

struct StackMapFrame {
	uint8_t		tag;

	StackMapFrame() {
	}

	StackMapFrame(uint8_t tag)
		: tag(tag) {
	}

	virtual
	~StackMapFrame();

	virtual inline
	StackMapFrame *DecodeFrame(ClassBuffer *, ClassFile *) {
		return this;
	}

	virtual inline
	StackMapFrame *EncodeFrame(ClassBuilder *, ClassFile *) {
		return this;
	}
};

struct StackMapOffFrame
		: public StackMapFrame {
	uint16_t	offset_delta;

	StackMapOffFrame() {
	}

	StackMapOffFrame(uint8_t tag)
		: StackMapFrame(tag) {
	}

	StackMapOffFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapOffFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapItemFrame
		: public StackMapFrame {
	// Stack Items
	VariableInfo *stack;

	StackMapItemFrame() {
	}

	StackMapItemFrame(uint8_t tag)
		: StackMapFrame(tag) {
	}

	~StackMapItemFrame();

	StackMapItemFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapItemFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapExtFrame
		: public StackMapFrame {
	uint16_t	offset_delta;

	// Stack Items
	VariableInfo *stack;

	StackMapExtFrame() {
	}

	StackMapExtFrame(uint8_t tag)
		: StackMapFrame(tag) {
	}

	~StackMapExtFrame();

	StackMapExtFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapExtFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapListFrame
		: public StackMapFrame {
	uint16_t	offset_delta;

	// Stack Item Table
	VariableInfo **stack;

	StackMapListFrame() {
	}

	StackMapListFrame(uint8_t tag)
		: StackMapFrame(tag) {
	}

	~StackMapListFrame();

	StackMapListFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapListFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapFullFrame
		: public StackMapFrame {
	uint16_t	offset_delta;

	// Local Items
	std::vector<VariableInfo *> locals;

	// Stack Items
	std::vector<VariableInfo *> stack;

	StackMapFullFrame() {
	}

	StackMapFullFrame(uint8_t tag)
		: StackMapFrame(tag) {
	}

	~StackMapFullFrame();

	StackMapFullFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapFullFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

StackMapFrame *decodeStackMapFrame(ClassBuffer *buffer, ClassFile *classFile);

int encodeStackMapFrame(ClassBuilder *builder, ClassFile *classFile, StackMapFrame *frame);

} /* JBC */

# endif /* StackMapFrame.h */
