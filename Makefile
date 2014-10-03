
CFLAGS = -D _DEBUG=3 -Werror -Wall -Wextra

objects = Main.o ClassBuffer.o ClassFile.o ConstantInfo.o \
	MemberInfo.o AttributeInfo.o AttributeInfo0.o

test: $(objects)
	gcc $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o *.exe
