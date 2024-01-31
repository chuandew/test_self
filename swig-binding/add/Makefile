add.so: add.c add.h add_wrap.c
	gcc -shared -fPIC -o _add.so add.c add_wrap.c -I${CONDA_PREFIX}/include/python3.11

add_wrap.c: add.i
	swig -python -py3 add.i

clean:
	rm _add.so add_wrap.c add.py