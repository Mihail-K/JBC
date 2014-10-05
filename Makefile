
CFLAGS = -Werror -Wall -Wextra
CPPFLAGS = -D _DEBUG=3 -I include/

objects = Main.o ClassBuffer.o ClassFile.o ClassDecoder.o ConstantDecoder.o ConstantInfo.o \
	MemberDecoder.o MemberInfo.o AttributeDecoder.o AttributeInfo.o List.o

test: $(objects)
	gcc $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o *.exe

%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
