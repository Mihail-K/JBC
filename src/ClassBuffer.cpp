
# include <errno.h>
# include <stdlib.h>
# include <string.h>

# include "ClassBuffer.h"

ClassBuffer::ClassBuffer(FILE *input)
		: input(input) {
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

static inline
void ToBigEndian(uint8_t &a, uint8_t &b) {
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

	ToBigEndian(u.b[0], u.b[1]);
	return u.l;
}

static inline
uint32_t ToBigEndian(uint32_t l) {
	union {
		uint32_t l;
		uint8_t b[4];
	} u = { l };

	ToBigEndian(u.b[0], u.b[3]);
	ToBigEndian(u.b[1], u.b[2]);
	return u.l;
}

uint8_t ClassBuffer::NextByte() {
	size_t read;
	uint8_t value = 0;

	if((read = fread(&value, 1, sizeof(uint8_t), input))
			!= sizeof(uint8_t))
		throw BufferError(strerror(errno));
	return value;
}

uint16_t ClassBuffer::NextShort() {
	size_t read;
	uint16_t value = 0;

	if((read = fread(&value, 1, sizeof(uint16_t), input))
			!= sizeof(uint16_t))
		throw BufferError(strerror(errno));
	return ToBigEndian(value);
}

uint32_t ClassBuffer::NextInt() {
	size_t read;
	uint32_t value = 0;

	if((read = fread(&value, 1, sizeof(uint32_t), input))
			!= sizeof(uint32_t))
		throw BufferError(strerror(errno));
	return ToBigEndian(value);
}
