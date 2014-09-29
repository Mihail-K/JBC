# ifndef __CLASSBUFFER_H__
# define __CLASSBUFFER_H__

# include <stdint.h>

typedef struct ClassBuffer ClassBuffer;

ClassBuffer *createBuffer(FILE *classFile);

void deleteBuffer(ClassBuffer *buffer);

int bufferEOF(ClassBuffer *buffer);

int8_t bufferNextByte(ClassBuffer *buffer);

int16_t bufferNextShort(ClassBuffer *buffer);

int32_t bufferNextInt(ClassBuffer *buffer);

int64_t bufferNextLong(ClassBuffer *buffer);

# endif /* ClassBuffer.h */
