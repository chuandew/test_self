_person.so : person.h person_wrap
	g++ -shared -fPIC -o _person.so person_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
person_wrap : person.i
	swig -c++ -python -py3 person.i
clean :
	rm _person.so person_wrap.cxx person.py