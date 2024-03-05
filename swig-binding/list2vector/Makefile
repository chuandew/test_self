all : _person.so person_wrap.cxx
_person.so : person.h person_wrap.cxx
	g++ -shared -fPIC -o _person.so person_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
person_wrap.cxx : person.i
	swig -c++ -python person.i
clean :
	rm -rf _person.so person_wrap.cxx person.py __pycache__