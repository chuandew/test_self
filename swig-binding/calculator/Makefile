calculator : calculator.cc calculator.h calculator_wrap
	g++ -shared -fPIC -o _calculator.so calculator.cc calculator_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
calculator_wrap : calculator.i
	swig -c++ -python -py3 calculator.i
clean :
	rm _calculator.so calculator_wrap.cxx calculator.py