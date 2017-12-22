all : SPIMI.cpp gammaCode.cpp gammaCode.hpp btree.cpp btree.h dic_hash.cpp
	g++ SPIMI.cpp gammaCode.cpp --std=c++11 -o SPIMI
	g++ btree.cpp --std=c++11 -o btree.o
	g++ dic_hash.cpp --std=c++11 -o hash.o
	g++ search.cpp --std=c++11 -o search.o

clean :
	rm block*.txt InvertedIndex.txt SPIMI
btree :
	g++ btree.cpp gammaCode.cpp --std=c++11 -o btree.o
hash :
	g++ dic_hash.cpp --std=c++11 -o hash.o
spimi :
	g++ SPIMI.cpp gammaCode.cpp --std=c++11 -o SPIMI
search :
	g++ search.cpp --std=c++11 -o search.o
