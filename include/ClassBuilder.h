# ifndef __CLASSBUILDER_H__
# define __CLASSBUILDER_H__

# include <stdio.h>
# include <stdint.h>

# include "ErrorTypes.h"

struct BuilderError
		: public EncodeError {
	inline
	BuilderError(const char *msg)
		: EncodeError(msg) {
	}
};

class ClassBuilder {
private:
	FILE *output;
	unsigned writes;

public:
	ClassBuilder(FILE *output);
	~ClassBuilder();

public:
	inline
	unsigned GetWrites() {
		return writes;
	}

public:
	size_t Position();

	ClassBuilder *Skip(size_t count);

	ClassBuilder *Next(uint8_t *src, size_t count);

	ClassBuilder *NextByte(uint8_t byte);

	ClassBuilder *NextShort(uint16_t word);

	ClassBuilder *NextInt(uint32_t dword);
};

# endif /* ClassBuilder.h */
