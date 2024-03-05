all : _vector.so vector_wrap.cxx
_vector.so : vector.cc vector.h vector_wrap.cxx
	g++ -shared -fPIC -o _vector.so vector.cc vector_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
vector_wrap.cxx : vector.i
	swig -c++ -python vector.i
clean :
	rm _vector.so vector_wrap.cxx vector.py