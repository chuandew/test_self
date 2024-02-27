all : _track.so track_wrap
_track.so : track.cc track.h track_wrap
	g++ -shared -fPIC -o _track.so track.cc track_wrap.cxx -I${CONDA_PREFIX}/include/python3.11
track_wrap : track.i
	swig -c++ -python -py3 track.i
clean :
	rm _track.so track_wrap.cxx track.py