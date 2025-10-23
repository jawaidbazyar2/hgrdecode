hgrd: hgrd.cpp ppm.cpp font.cpp util.cpp hgr.cpp hgr.hpp ntsc.cpp Matrix3x3.cpp lgr.cpp dhgr.cpp *.hpp shr.cpp shr.hpp
	clang++ -std=c++17 -O3 -o hgrd hgrd.cpp ppm.cpp font.cpp util.cpp hgr.cpp ntsc.cpp lgr.cpp Matrix3x3.cpp dhgr.cpp shr.cpp

ch: ch.cpp
	clang++ -std=c++17 -fsanitize=address  -o ch ch.cpp

#iir: iir.cpp ppm.cpp util.cpp
#	clang++ -std=c++17 -fsanitize=address  -o iir iir.cpp ppm.cpp util.cpp

comp: comp.cpp ppm.cpp Matrix3x3.cpp ntsc.cpp
	clang++ -O3 -std=c++17 -fsanitize=address  -o comp comp.cpp ppm.cpp Matrix3x3.cpp ntsc.cpp

phase: phase.cpp
	clang++ -std=c++17 -fsanitize=address  -o phase phase.cpp

all: hgrd comp phase ch iir

