
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

typedef struct item //like Term in SPIMI.cpp
{
    int df;//TODO
    short ptr;
    //daopaijilubiaozhizhen
}item ;

void add(string &dict ,string newstr )
 {
    item * t = new(item);
    t->ptr = dict.size()-1;
//    newstr+="$";
    dict.append(newstr);
//    dict.push_back("$");

 }
