#ifndef _SINGLE_STRING_DICT_HPP_
#define _SINGLE_STRING_DICT_HPP_
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
class metastr: public string{
  public:
    int add(string newstr);
    string getStr(int pos);
};
#endif  // _SINGLE_STRING_DICT_HPP_
