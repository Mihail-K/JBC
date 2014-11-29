
JC = javac

CFLAGS = -Werror -Wall -Wextra
CPPFLAGS = -I include/

objects = ClassBuffer.o ClassBuilder.o \
	ClassFile.o ClassDecoder.o ClassEncoder.o \
	ConstantDecoder.o ConstantEncoder.o ConstantInfo.o \
	MemberDecoder.o MemberEncoder.o MemberInfo.o \
	StackMapFrame.o ElementValue.o \
	AttributeDecoder.o AttributeEncoder.o AttributeInfo.o

all: libjbc.a jbctest Test.class

libjbc.a: libjbc.a($(objects))

jbctest: test.o libjbc.a
	$(CC) $(LDFLAGS) -L ./ -o $@ $< $(LDLIBS) -ljbc -lstdc++

.PHONY: clean
clean:
	rm -f *.o *.exe *.a *.class *.hex

Test.class : test/Test.java
	$(JC) $(JFALGS) $<
	mv test/*.class .

test.o: test/Main.cpp include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

%.o: src/%.cpp include/*.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
