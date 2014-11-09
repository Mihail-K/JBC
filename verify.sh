
# A short script to confirm the program functions correctly.

./jbctest Test.class Test2.class

xxd Test.class > Test1.hex
xxd Test2.class > Test2.hex

diff -q Test1.hex Test2.hex
rc=$?

rm Test1.hex Test2.hex Test2.class
exit $rc
