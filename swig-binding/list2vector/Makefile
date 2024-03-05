all : _example.so example_wrap.cxx
_example.so : example.h example_wrap.cxx
	g++ -shared -fPIC -o _example.so example_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
example_wrap.cxx : example.i
	swig -c++ -python example.i
clean :
	rm -rf _example.so example_wrap.cxx example.py __pycache__