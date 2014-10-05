# ifndef __ZALLOC_H__
# define __ZALLOC_H__

# include <stdlib.h>

# include "Defines.h"

static inline
void *zalloc(size_t size) {
	void *bytes = malloc(size);
	if(bytes == NULL) {
		perror("zalloc");
		exit(EXIT_FAILURE);
	}
	return bytes;
}

# endif /* Zalloc.h */
