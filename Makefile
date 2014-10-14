
JC = javac

CFLAGS = -Werror -Wall -Wextra
CPPFLAGS = -D _DEBUG=3 -I include/

objects = ClassBuffer.o ClassBuilder.o List.o \
	ClassFile.o ClassDecoder.o ClassEncoder.o \
	ConstantDecoder.o ConstantEncoder.o ConstantInfo.o \
	MemberDecoder.o MemberEncoder.o MemberInfo.o \
	AttributeEncoder.o AttributeDecoder.o AttributeInfo.o

all: libjbc.a jbctest Test.class

libjbc.a: libjbc.a($(objects))

jbctest: test.o libjbc.a
	$(CC) $(LDFLAGS) -L ./ -o $@ $< $(LDLIBS) -ljbc

.PHONY: clean
clean:
	rm -f *.o *.exe *.a *.class

Test.class : test/Test.java
	$(JC) $(JFALGS) $<
	mv test/*.class .

test.o: test/Main.c include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

%.o: src/%.c include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
