# target: prerequisites - the rule head
# $@ - means the target
# $^ - means all prerequisites
# $< - means just the first prerequisite
# ar - a Linux tool to create, modify, and extract from archives see the man pages for further information. The options in this case mean:
# r - replace files existing inside the archive
# c - create an archive if not already present
# s - create an object-file index into the archive

TARGET = prog

$(TARGET): main.o libtestb.so libtesta.a
	gcc $^ -o $@

main.o: main.c
	gcc -c $< -o $@
     
libtestb.so: libtestb.o
	gcc -shared -o $@ $^

libtestb.o: testb.c header.h
	gcc -c -fPIC -o $@ $<

libtesta.a: libtesta.o
	ar rcs $@ $^

libtesta.o: testa.c header.h
	gcc -c -fPIC -o $@ $<

clean:
	rm -f *.o *.a *.so $(TARGET)