# ifndef __CLASSBUILDER_H__
# define __CLASSBUILDER_H__

# include <stdio.h>
# include <stdint.h>

typedef FILE ClassBuilder;

ClassBuilder *createBuilder(FILE *classFile);

void deleteBuilder(ClassBuilder *builder);

long int builderPos(ClassBuilder *builder);

ClassBuilder *buildNextByte(ClassBuilder *builder, uint8_t byte);

ClassBuilder *buildNextShort(ClassBuilder *builder, uint16_t word);

ClassBuilder *buildNextInt(ClassBuilder *builder, uint32_t dword);

# endif /* ClassBuilder.h */
