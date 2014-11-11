# ifndef __CLASSBUFFER_H__
# define __CLASSBUFFER_H__

# include <stdio.h>
# include <stdint.h>

# include "ErrorTypes.h"

namespace JBC {

struct BufferError
		: public DecodeError {
	inline
	BufferError(const char *msg)
		: DecodeError(msg) {
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

	void Skip(size_t count);

	uint8_t *Next(uint8_t *dst, size_t count);

	uint8_t NextByte();

	uint16_t NextShort();

	uint32_t NextInt();
};

} /* JBC */

# endif /* ClassBuffer.h */
