
# include <errno.h>
# include <stdlib.h>
# include <string.h>

# include "ClassBuffer.h"

namespace JBC {

ClassBuffer::ClassBuffer(FILE *input)
		: input(input), reads(0) {
	if(input == NULL) {
		throw BufferError("Invalid input file.");
	}

	if(setvbuf(input, NULL, _IOFBF, 4096) == EOF) {
		throw BufferError(strerror(errno));
	}
}

ClassBuffer::~ClassBuffer() {
	fclose(input);
}

size_t ClassBuffer::Position() {
	return ftell(input);
}

// TODO: Implement an endianness check.

static inline
void Swap(uint8_t &a, uint8_t &b) {
	uint8_t t = a;
	a = b;
	b = t;
}

static inline
uint16_t FromBigEndian(uint16_t l) {
	union {
		uint16_t l;
		uint8_t b[2];
	} u = { l };

	Swap(u.b[0], u.b[1]);
	return u.l;
}

static inline
uint32_t FromBigEndian(uint32_t l) {
	union {
		uint32_t l;
		uint8_t b[4];
	} u = { l };

	Swap(u.b[0], u.b[3]);
	Swap(u.b[1], u.b[2]);
	return u.l;
}

void ClassBuffer::Skip(size_t count) {
	reads += count;
	if(fseek(input, count, SEEK_CUR) == EOF) {
		throw BufferError(strerror(errno));
	}
}

uint8_t *ClassBuffer::Next(uint8_t *dst, size_t count) {
	size_t read;

	reads += count;
	if((read = fread(dst, sizeof(uint8_t), count, input))
			!= count * sizeof(uint8_t))
		throw BufferError(strerror(errno));
	return dst;
}

uint8_t ClassBuffer::NextByte() {
	size_t read;
	uint8_t value = 0;

	reads++;
	if((read = fread(&value, 1, sizeof(uint8_t), input))
			!= sizeof(uint8_t))
		throw BufferError(strerror(errno));
	return value;
}

uint16_t ClassBuffer::NextShort() {
	size_t read;
	uint16_t value = 0;

	reads++;
	if((read = fread(&value, 1, sizeof(uint16_t), input))
			!= sizeof(uint16_t))
		throw BufferError(strerror(errno));
	return FromBigEndian(value);
}

uint32_t ClassBuffer::NextInt() {
	size_t read;
	uint32_t value = 0;

	reads++;
	if((read = fread(&value, 1, sizeof(uint32_t), input))
			!= sizeof(uint32_t))
		throw BufferError(strerror(errno));
	return FromBigEndian(value);
}

} /* JBC */
