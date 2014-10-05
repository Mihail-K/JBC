
CFLAGS = -Werror -Wall -Wextra
CPPFLAGS = -D _DEBUG=3 -I include/

objects = ClassBuffer.o ClassBuilder.o List.o ClassFile.o ClassDecoder.o ConstantDecoder.o \
	ConstantInfo.o MemberDecoder.o MemberInfo.o AttributeDecoder.o AttributeInfo.o

all: libjbc.a test

libjbc.a: libjbc.a($(objects))

test: test.o libjbc.a
	$(CC) $(LDFLAGS) -L ./ -o $@ $< $(LDLIBS) -ljbc

.PHONY: clean
clean:
	rm -f *.o *.exe *.a

test.o: test/Main.c include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
