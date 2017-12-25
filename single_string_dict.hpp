
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

//typedef struct item //like Term in SPIMI.cpp
//{
//    int df;//TODO
//    short ptr;
//    //daopaijilubiaozhizhen
//}item ;

class metastr: public string{
      public:
        int add(string newstr);
        string getStr(int pos);
};
int metastr::add(string newstr)
{
    int temp = this->size();
    newstr+="$";
    this->append(newstr);
    return temp;
}
string metastr::getStr(int pos)
{
    string res = "";
    char p = (*this)[pos];
    while(p!='$')
    {
        res += p;
        pos ++;
        p = (*this)[pos];
    }
    return res;
}
//void add(string &dict ,string newstr )
// {
//    item * t = new(item);
//    t->ptr = dict.size()-1;
//    newstr+="$";
//    dict.append(newstr);
////    dict.push_back("$");
// }
