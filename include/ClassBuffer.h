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
	unsigned reads;

public:
	ClassBuffer(FILE *input);
	~ClassBuffer();

public:
	inline
	unsigned GetReads() {
		return reads;
	}

public:
	size_t Position();

	uint8_t NextByte();

	uint16_t NextShort();

	uint32_t NextInt();
};

# endif /* ClassBuffer.h */
