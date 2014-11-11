
# include <errno.h>
# include <stdlib.h>
# include <string.h>

# include "ClassBuilder.h"

ClassBuilder::ClassBuilder(FILE *output)
		: output(output), writes(0) {
	if(output == NULL) {
		throw BuilderError("Invalid input file.");
	}

	if(setvbuf(output, NULL, _IOFBF, 4096) == EOF) {
		throw BuilderError(strerror(errno));
	}
}

ClassBuilder::~ClassBuilder() {
	fflush(output);
	fclose(output);
}

size_t ClassBuilder::Position() {
	return ftell(output);
}

// TODO: Implement an endianness check.

static inline
void Swap(uint8_t &a, uint8_t &b) {
	uint8_t t = a;
	a = b;
	b = t;
}

static inline
uint16_t ToBigEndian(uint16_t l) {
	union {
		uint16_t l;
		uint8_t b[2];
	} u = { l };

	Swap(u.b[0], u.b[1]);
	return u.l;
}

static inline
uint32_t ToBigEndian(uint32_t l) {
	union {
		uint32_t l;
		uint8_t b[4];
	} u = { l };

	Swap(u.b[0], u.b[3]);
	Swap(u.b[1], u.b[2]);
	return u.l;
}

ClassBuilder *ClassBuilder::NextByte(uint8_t byte) {
	size_t wrote;

	writes++;
	if((wrote = fwrite(&byte, 1, sizeof(uint8_t), output))
			!= sizeof(uint8_t))
		throw BuilderError(strerror(errno));
	return this;
}

ClassBuilder *ClassBuilder::NextShort(uint16_t word) {
	size_t wrote;

	writes++;
	word = ToBigEndian(word);
	if((wrote = fwrite(&word, 1, sizeof(uint16_t), output))
			!= sizeof(uint16_t))
		throw BuilderError(strerror(errno));
	return this;
}

ClassBuilder *ClassBuilder::NextInt(uint32_t dword) {
	size_t wrote;

	writes++;
	dword = ToBigEndian(dword);
	if((wrote = fwrite(&dword, 1, sizeof(uint32_t), output))
			!= sizeof(uint32_t))
		throw BuilderError(strerror(errno));
	return this;
}
