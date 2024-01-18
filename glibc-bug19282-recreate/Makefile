all: testlib.so test

testlib.so: testlib.c
	gcc -fPIC -shared -o testlib.so testlib.c

test: test.c
	gcc -o test test.c -ldl -lpthread
