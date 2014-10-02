
# include <stdio.h>
# include <string.h>

# include "Zalloc.h"
# include "ClassBuffer.h"

struct ClassBuffer {
	FILE *file;

	// TODO
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

	return buffer;
}

void deleteBuffer(ClassBuffer *buffer) {
	if(buffer == NULL) return;
	if(buffer->file != NULL)
		fclose(buffer->file);
	free(buffer);
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

int8_t bufferNextByte(ClassBuffer *buffer) {
	int8_t value = 0;
	if(buffer == NULL) return -1;
	if(fread(&value, 1, sizeof(uint8_t), buffer->file) != sizeof(uint8_t)) {
		perror("fread byte");
		exit(EXIT_FAILURE);
	}

	return value;
}

int16_t bufferNextShort(ClassBuffer *buffer) {
	int16_t value = 0;
	if(buffer == NULL) return -1;
	if(fread(&value, 1, sizeof(uint16_t), buffer->file) != sizeof(uint16_t)) {
		perror("fread short");
		exit(EXIT_FAILURE);
	}

	return tole16(value);
}

int32_t bufferNextInt(ClassBuffer *buffer) {
	int32_t value = 0;
	if(buffer == NULL) return -1;
	if(fread(&value, 1, sizeof(uint32_t), buffer->file) != sizeof(uint32_t)) {
		perror("fread int");
		exit(EXIT_FAILURE);
	}


	return tole32(value);
}
