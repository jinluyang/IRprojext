
#include <string>
#include <string.h>
#include <iostream>

typedef struct item
{
    int df;//TODO
    int ptr;
    //daopaijilubiaozhizhen
}item ;

void add(string &dict ,string newstr )
 {
    item * t = new(item);
    t->ptr = dict.size();
    newstr+="$";
    dict.append(newstr);
//    dict.push_back("$");

 }
