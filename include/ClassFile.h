# ifndef __CLASSFILE_H__
# define __CLASSFILE_H__

# include <vector>
# include <stdio.h>
# include <stdlib.h>

# include "ClassBuffer.h"
# include "ClassBuilder.h"

struct ConstantInfo;
struct ConstantClassInfo;

struct MemberInfo;
struct AttributeInfo;

class ClassFile {
public:
	uint32_t	magic;

	// Version Info
	uint16_t	major_version;
	uint16_t	minor_version;

	// Constants Table
	std::vector<ConstantInfo *> constant_pool;

	uint16_t	access_flags;

	// This Class
	ConstantClassInfo *this_class;

	// Super Class
	ConstantClassInfo *super_class;

	// Interfaces Table
	std::vector<ConstantClassInfo *> interfaces;

	// Fields Table
	std::vector<MemberInfo *> fields;

	// Methods Table
	std::vector<MemberInfo *> methods;

	// Attributes Table
	std::vector<AttributeInfo *> attributes;

public:
	// Constructors
	ClassFile();
	ClassFile(ClassBuffer *buffer);

	~ClassFile();

public:
	void DecodeClassFile(ClassBuffer *buffer);
	void EncodeClassFile(ClassBuilder *builder);

private:
	void DecodeConstants(ClassBuffer *buffer);
	void EncodeConstants(ClassBuilder *builder);

	void DecodeClasses(ClassBuffer *buffer);
	void EncodeClasses(ClassBuilder *builder);

	void DecodeInterfaces(ClassBuffer *buffer);
	void EncodeInterfaces(ClassBuilder *builder);

	void DecodeFields(ClassBuffer *buffer);
	void EncodeFields(ClassBuilder *builder);

	void DecodeMethods(ClassBuffer *buffer);
	void EncodeMethods(ClassBuilder *builder);

	void DecodeAttributes(ClassBuffer *buffer);
	void EncodeAttributes(ClassBuilder *builder);
};

ClassFile *decodeClassFile(FILE *source);

int encodeClassFile(FILE *target, ClassFile *classFile);

static inline
void *getConstant(ClassFile *classFile, unsigned int idx) {
	if(idx > (unsigned int)classFile->constant_pool.size()) {
		fprintf(stderr, "Constant not found. (ID : %d/%zu)\n", idx,
				classFile->constant_pool.size());
		exit(EXIT_FAILURE);
	}

	return (void *)classFile->constant_pool[idx];
}

# endif /* ClassFile.h */
