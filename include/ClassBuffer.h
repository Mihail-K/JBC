# ifndef __CLASSBUFFER_H__
# define __CLASSBUFFER_H__

# include <string>
# include <stdio.h>
# include <stdint.h>

struct BufferError {
	const std::string msg;

	inline
	BufferError(const char *msg)
		: msg(std::string(msg)) {
	}
};

class ClassBuffer {
private:
	FILE *input;

public:
	ClassBuffer(FILE *input);
	~ClassBuffer();

public:
	size_t Position();

	uint8_t NextByte();

	uint16_t NextShort();

	uint32_t NextInt();
};

// ClassBuffer *createBuffer(FILE *classFile);
# define createBuffer(input) new ClassBuffer(input)

// void deleteBuffer(ClassBuffer *buffer);
# define deleteBuffer(buffer) delete buffer

// long int bufferPos(ClassBuffer *buffer);
# define bufferPos(buffer) buffer->Position()

// uint8_t bufferNextByte(ClassBuffer *buffer);
# define bufferNextByte(buffer) buffer->NextByte()

// uint16_t bufferNextShort(ClassBuffer *buffer);
# define bufferNextShort(buffer) buffer->NextShort()

// uint32_t bufferNextInt(ClassBuffer *buffer);
# define bufferNextInt(buffer) buffer->NextInt()

# endif /* ClassBuffer.h */
