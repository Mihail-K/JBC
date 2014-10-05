# ifndef __CLASSBUFFER_H__
# define __CLASSBUFFER_H__

# include <stdio.h>
# include <stdint.h>

typedef FILE ClassBuffer;

ClassBuffer *createBuffer(FILE *classFile);

void deleteBuffer(ClassBuffer *buffer);

long int bufferPos(ClassBuffer *buffer);

uint8_t bufferNextByte(ClassBuffer *buffer);

uint16_t bufferNextShort(ClassBuffer *buffer);

uint32_t bufferNextInt(ClassBuffer *buffer);

# endif /* ClassBuffer.h */
