
CFLAGS = -D _DEBUG=3 -Werror -Wall -Wextra

test: Main.o ClassBuffer.o ClassFile.o ConstantInfo.o MemberInfo.o AttributeInfo.o
	gcc $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o *.exe
