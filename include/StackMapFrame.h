/**
 * @file MemberInfo.h
 * @author Mihail K
 * @date November, 2014
 * @version 0.37
 *
 * @brief Defines stack map frame types.
 **/
# ifndef __STACKMAPFRAME_H__
# define __STACKMAPFRAME_H__

# include <stdint.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"
# include "ConstantInfo.h"

namespace JBC {

struct VariableInfo {
	uint8_t		tag;

	VariableInfo()
		: tag(0) {
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

	ObjectVariableInfo()
		: object(NULL) {
	}

	ObjectVariableInfo(uint8_t tag)
		: VariableInfo(tag), object(NULL) {
	}

	ObjectVariableInfo *DecodeInfo(ClassBuffer *buffer, ClassFile *classFile);

	ObjectVariableInfo *EncodeInfo(ClassBuilder *builder, ClassFile *classFile);
};

struct UninitializedVariableInfo
		: public VariableInfo {
	uint16_t offset;

	UninitializedVariableInfo()
		: offset(0) {
	}

	UninitializedVariableInfo(uint8_t tag)
		: VariableInfo(tag), offset(0) {
	}

	UninitializedVariableInfo *DecodeInfo(ClassBuffer *buffer, ClassFile *classFile);

	UninitializedVariableInfo *EncodeInfo(ClassBuilder *builder, ClassFile *classFile);
};

VariableInfo *DecodeVariableInfo(ClassBuffer *buffer, ClassFile *classFile);

void EncodeVariableInfo(ClassBuilder *builder, ClassFile *classFile, VariableInfo *info);

struct StackMapFrame {
	uint8_t		tag;

	StackMapFrame()
		: tag(0) {
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

	StackMapOffFrame()
		: offset_delta(0) {
	}

	StackMapOffFrame(uint8_t tag)
		: StackMapFrame(tag), offset_delta(0) {
	}

	StackMapOffFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapOffFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

struct StackMapItemFrame
		: public StackMapFrame {
	// Stack Items
	VariableInfo *stack;

	StackMapItemFrame()
		: stack(NULL) {
	}

	StackMapItemFrame(uint8_t tag)
		: StackMapFrame(tag), stack(NULL) {
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

	StackMapExtFrame()
		: offset_delta(0), stack(NULL) {
	}

	StackMapExtFrame(uint8_t tag)
		: StackMapFrame(tag), offset_delta(0), stack(NULL) {
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

	StackMapListFrame()
		: offset_delta(0), stack(NULL) {
	}

	StackMapListFrame(uint8_t tag)
		: StackMapFrame(tag), offset_delta(0), stack(NULL) {
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

	StackMapFullFrame()
		: offset_delta(0) {
	}

	StackMapFullFrame(uint8_t tag)
		: StackMapFrame(tag), offset_delta(0) {
	}

	~StackMapFullFrame();

	StackMapFullFrame *DecodeFrame(ClassBuffer *buffer, ClassFile *classFile);

	StackMapFullFrame *EncodeFrame(ClassBuilder *builder, ClassFile *classFile);
};

StackMapFrame *DecodeStackMapFrame(ClassBuffer *buffer, ClassFile *classFile);

void EncodeStackMapFrame(ClassBuilder *builder, ClassFile *classFile, StackMapFrame *frame);

} /* JBC */

# endif /* StackMapFrame.h */
