all : spimi.cpp gammaCode.cpp search.cpp btree.cpp single_string_dict.cpp
	g++ spimi.cpp gammaCode.cpp --std=c++11 -o SPIMI
	g++ search.cpp btree.cpp single_string_dict.cpp gammaCode.cpp --std=c++11 -o search

clean :
	rm block*.txt InvertedIndex.txt SPIMI search 
btree : btree.cpp single_string_dict.cpp gammaCode.cpp
	g++ btree.cpp single_string_dict.cpp gammaCode.cpp --std=c++11 -o btree
hash : dic_hash.cpp
	g++ dic_hash.cpp --std=c++11 -o hash
spimi : spimi.cpp gammaCode.cpp
	g++ spimi.cpp gammaCode.cpp --std=c++11 -o SPIMI
search : btree.cpp search.cpp single_string_dict.cpp gammaCode.cpp
	g++ -g btree.cpp search.cpp single_string_dict.cpp gammaCode.cpp --std=c++11 -o search 
