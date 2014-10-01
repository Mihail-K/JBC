
CFLAGS = -D _DEBUG

test: Main.o ClassBuffer.o ConstantInfo.o MemberInfo.o AttributeInfo.o
	gcc $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -f *.o *.exe
