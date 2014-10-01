
# include <stdio.h>
# include <string.h>

# include "Zalloc.h"
# include "ClassBuffer.h"

struct ClassBuffer {
	FILE *file;
	int pos, length;
	int8_t buffer[4096];
};

ClassBuffer *createBuffer(FILE *classFile) {
	ClassBuffer *buffer;

	if(classFile == NULL) return NULL;
	buffer = zalloc(sizeof(ClassBuffer));

	buffer->file = classFile;
	buffer->pos = buffer->length = 0;

	return buffer;
}

void deleteBuffer(ClassBuffer *buffer) {
	if(buffer == NULL) return;
	if(buffer->file != NULL)
		fclose(buffer->file);
	free(buffer);
}

int bufferEOF(ClassBuffer *buffer) {
	if(buffer == NULL) return 1;
	return feof(buffer->file);
}

int bufferMore(ClassBuffer *buffer) {
	if(buffer == NULL) return -1;
	if((buffer->length = fread(
		buffer->buffer, 1, 4096, buffer->file)) < 1) {
		return -1;
	}
	return 0;
}

static __inline__
void swap(uint8_t *a, uint8_t *b) {
	uint8_t t = *a;
	*a = *b;
	*b = t;
}

static __inline__
uint16_t tole16(uint16_t l) {
	union { uint16_t l; uint8_t b[2]; } u = { l };
	swap(&u.b[0], &u.b[1]);
	return u.l;
}

static __inline__
uint32_t tole32(uint32_t l) {
	union { uint32_t l; uint8_t b[4]; } u = { l };
	swap(&u.b[0], &u.b[3]);
	swap(&u.b[1], &u.b[2]);
	return u.l;
}

static __inline__
uint64_t tole64(uint64_t l) {
	union { uint64_t l; uint8_t b[8]; } u = { l };
	swap(&u.b[0], &u.b[7]);
	swap(&u.b[1], &u.b[6]);
	swap(&u.b[2], &u.b[5]);
	swap(&u.b[3], &u.b[4]);
	return u.l;
}

int8_t bufferNextByte(ClassBuffer *buffer) {
	int8_t value = 0;
	if(buffer == NULL) return -1;
	if(buffer->pos + sizeof(int8_t) - 1 >= buffer->length) {
			if(bufferMore(buffer)) {
				return -1;
			}
	}

	value = buffer->buffer[buffer->pos];
	buffer->pos += sizeof(int8_t);

	return value;
}

int16_t bufferNextShort(ClassBuffer *buffer) {
	int16_t value = 0;
	if(buffer == NULL) return -1;
	if(buffer->pos + sizeof(int16_t) - 1 >= buffer->length) {
			if(bufferMore(buffer)) {
				return -1;
			}
	}

	memcpy(&value, &buffer->buffer[buffer->pos], sizeof(int16_t));
	buffer->pos += sizeof(int16_t);

	return tole16(value);
}

int32_t bufferNextInt(ClassBuffer *buffer) {
	int32_t value = 0;
	if(buffer == NULL) return -1;
	if(buffer->pos + sizeof(int32_t) - 1 >= buffer->length) {
			if(bufferMore(buffer)) {
				return -1;
			}
	}

	memcpy(&value, &buffer->buffer[buffer->pos], sizeof(int32_t));
	buffer->pos += sizeof(int32_t);

	return tole32(value);
}

int64_t bufferNextLong(ClassBuffer *buffer) {
	int64_t value = 0;
	if(buffer == NULL) return -1;
	if(buffer->pos + sizeof(int64_t) - 1 >= buffer->length) {
			if(bufferMore(buffer)) {
				return -1;
			}
	}

	memcpy(&value, &buffer->buffer[buffer->pos], sizeof(int64_t));
	buffer->pos += sizeof(int64_t);

	return tole64(value);
}
