#include "single_string_dict.hpp"
int metastr::add(string newstr) {
  int temp = this->size();
  newstr+="$";
  this->append(newstr);
  return temp;
}
string metastr::getStr(int pos) {
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

