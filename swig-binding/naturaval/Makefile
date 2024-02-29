cal : cal.h cal_wrap
	g++ -shared -fPIC -o _cal.so cal_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
cal_wrap : cal.i
	swig -c++ -python -py3 cal.i
clean :
	rm _cal.so cal_wrap.cxx cal.py