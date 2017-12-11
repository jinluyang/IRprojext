all : SPIMI.cpp gammaCode.cpp gammaCode.hpp
	g++ SPIMI.cpp gammaCode.cpp --std=c++11 -o SPIMI

clean :
	rm block*.txt InvertedIndex.txt SPIMI
