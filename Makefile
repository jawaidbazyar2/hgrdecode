hgrd: hgrd.cpp ppm.cpp font.cpp util.cpp hgr.cpp hgr.hpp
	clang++ -std=c++17 -fsanitize=address  -o hgrd hgrd.cpp ppm.cpp font.cpp util.cpp hgr.cpp

ch: ch.cpp
	clang++ -std=c++17 -fsanitize=address  -o ch ch.cpp

#iir: iir.cpp ppm.cpp util.cpp
#	clang++ -std=c++17 -fsanitize=address  -o iir iir.cpp ppm.cpp util.cpp

all: hgrd comp phase ch iir

comp: comp.cpp ppm.cpp Matrix3x3.cpp
	clang++ -O3 -std=c++17 -fsanitize=address  -o comp comp.cpp ppm.cpp Matrix3x3.cpp

phase: phase.cpp
	clang++ -std=c++17 -fsanitize=address  -o phase phase.cpp
