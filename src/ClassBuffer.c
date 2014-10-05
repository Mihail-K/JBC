
# include <stdio.h>
# include <string.h>

# include "Zalloc.h"
# include "ClassBuffer.h"

struct ClassBuffer {
	FILE *file;
	unsigned int pos;
};

ClassBuffer *createBuffer(FILE *classFile) {
	ClassBuffer *buffer;

	if(classFile == NULL) return NULL;
	if(setvbuf(classFile, NULL, _IOFBF, 4096) == EOF) {
		perror("setvbuf");
		exit(EXIT_FAILURE);
	}

	buffer = zalloc(sizeof(ClassBuffer));
	buffer->file = classFile;
	buffer->pos = 0;

	return buffer;
}

void deleteBuffer(ClassBuffer *buffer) {
	if(buffer == NULL) return;
	if(buffer->file != NULL)
		fclose(buffer->file);
	free(buffer);
}

unsigned int bufferPos(ClassBuffer *buffer) {
	if(buffer == NULL) return -1;
	return buffer->pos;
}

static inline
void swap(uint8_t *a, uint8_t *b) {
	uint8_t t = *a;
	*a = *b;
	*b = t;
}

static inline
uint16_t tole16(uint16_t l) {
	union { uint16_t l; uint8_t b[2]; } u = { l };
	swap(&u.b[0], &u.b[1]);
	return u.l;
}

static inline
uint32_t tole32(uint32_t l) {
	union { uint32_t l; uint8_t b[4]; } u = { l };
	swap(&u.b[0], &u.b[3]);
	swap(&u.b[1], &u.b[2]);
	return u.l;
}

uint8_t bufferNextByte(ClassBuffer *buffer) {
	size_t read;
	uint8_t value = 0;
	if(buffer == NULL) return -1;
	if((read = fread(&value, 1, sizeof(uint8_t), buffer->file)) != sizeof(uint8_t)) {
		perror("fread byte");
		fprintf(stderr, "Read : %u, expected %u (Pos : %u).\n",
				read, sizeof(uint8_t), buffer->pos);
		exit(EXIT_FAILURE);
	}

	buffer->pos += read;

	return value;
}

uint16_t bufferNextShort(ClassBuffer *buffer) {
	size_t read;
	uint16_t value = 0;
	if(buffer == NULL) return -1;
	if((read = fread(&value, 1, sizeof(uint16_t), buffer->file)) != sizeof(uint16_t)) {
		perror("fread short");
		fprintf(stderr, "Read : %u, expected %u (Pos : %u).\n",
				read, sizeof(uint16_t), buffer->pos);
		exit(EXIT_FAILURE);
	}

	buffer->pos += read;

	return tole16(value);
}

uint32_t bufferNextInt(ClassBuffer *buffer) {
	size_t read;
	uint32_t value = 0;
	if(buffer == NULL) return -1;
	if((read = fread(&value, 1, sizeof(uint32_t), buffer->file)) != sizeof(uint32_t)) {
		perror("fread int");
		fprintf(stderr, "Read : %u, expected %u (Pos : %u).\n",
				read, sizeof(uint32_t), buffer->pos);
		exit(EXIT_FAILURE);
	}

	buffer->pos += read;

	return tole32(value);
}
