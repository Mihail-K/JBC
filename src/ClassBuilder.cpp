
# include <stdlib.h>

# include "ClassBuilder.h"

ClassBuilder *createBuilder(FILE *classFile) {
	if(classFile == NULL) return NULL;
	if(setvbuf(classFile, NULL, _IOFBF, 4096) == EOF) {
		perror("setvbuf");
		exit(EXIT_FAILURE);
	}

	return classFile;
}

void deleteBuilder(ClassBuilder *builder) {
	if(builder == NULL) return;
	fflush(builder);
	fclose(builder);
}

long int builderPos(ClassBuilder *builder) {
	if(builder == NULL) return -1;
	return ftell(builder);
}

static inline
void swap(uint8_t *a, uint8_t *b) {
	uint8_t t = *a;
	*a = *b;
	*b = t; 
}

static inline
uint16_t tobe16(uint16_t l) {
	union { uint16_t l; uint8_t b[2]; } u = { l };
	swap(&u.b[0], &u.b[1]);
	return u.l;
}

static inline
uint32_t tobe32(uint32_t l) {
	union { uint32_t l; uint8_t b[4]; } u = { l };
	swap(&u.b[0], &u.b[3]);
	swap(&u.b[1], &u.b[2]);
	return u.l;
}

ClassBuilder *buildNextByte(ClassBuilder *builder, uint8_t byte) {
	size_t wrote;
	if(builder == NULL) return NULL;
	if((wrote = fwrite(&byte, 1, sizeof(uint8_t), builder)) != sizeof(uint8_t)) {
		perror("fwrite byte");
		fprintf(stderr, "Write : %zu, expected %zu (Pos : %ld).\n",
				wrote, sizeof(uint8_t), builderPos(builder));
		exit(EXIT_FAILURE);
	}

	return builder;
}

ClassBuilder *buildNextShort(ClassBuilder *builder, uint16_t word) {
	size_t wrote;
	if(builder == NULL) return NULL;

	word = tobe16(word);
	if((wrote = fwrite(&word, 1, sizeof(uint16_t), builder)) != sizeof(uint16_t)) {
		perror("fwrite short");
		fprintf(stderr, "Write : %zu, expected %zu (Pos : %ld).\n",
				wrote, sizeof(uint16_t), builderPos(builder));
		exit(EXIT_FAILURE);
	}

	return builder;
}

ClassBuilder *buildNextInt(ClassBuilder *builder, uint32_t dword) {
	size_t wrote;
	if(builder == NULL) return NULL;

	dword = tobe32(dword);
	if((wrote = fwrite(&dword, 1, sizeof(uint32_t), builder)) != sizeof(uint32_t)) {
		perror("fwrite int");
		fprintf(stderr, "Write : %zu, expected %zu (Pos : %ld).\n",
				wrote, sizeof(uint32_t), builderPos(builder));
		exit(EXIT_FAILURE);
	}

	return builder;
}
