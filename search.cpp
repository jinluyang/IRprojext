#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "gammaCode.hpp"
#include "btree.hpp"

using namespace std;
typedef uint32_t * UINT_32;
typedef string * STRING;

int initL(UINT_32 *L, int type_len);
int transformStr(STRING &tranStr, STRING &tranOps);
int transformPos(streampos pos, UINT_32 &doc_list);
int intersect(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list);
int unionDocIds(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list);
int negation(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list);
void visit(UINT_32 &doc_list, int len);

//初始化
int initL(UINT_32 *L, int type_len) {
  *L = (UINT_32)malloc(sizeof(uint32_t) * type_len);
  if(!*L)
    exit(-2);
  return 1;
}

// A AND B
// len1 > len2
// if len1==-1 or len2 == -1    return -1;
// after operation AND, length = 0      return -1;
int intersect(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list) {
  int i = 0, a = 0, b = 0;
  initL(&doc_list, len2);
  if (len1==-1 || len2==-1) {
    cout << "对不起，没有您要查找的关键字!" << endl;
    return -1;
  }

  while (b < len2) {
    if(pa[a] > pb[b]) b++;
    else if (pa[a] < pb[b]) a++;
    else {
      doc_list[i] = pa[a];
      i++;
      a++;
      b++;
    } 
  }
  if (i == 0) {
    cout << "AND操作后结果集为空" << endl;
    return -1;
  }
  return i;
}

// A OR B
// len1 > len2
// if len1 == -1 and len2 == -1     reutrn -1
// after operation OR, length = 0       return -1
int unionDocIds(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list) {
  int i = 0, a=0, b=0;
  initL(&doc_list, len1+len2);
  if (len1==-1 && len2==-1) {
    cout << "对不起，没有您要查找的关键字!" << endl;
    return -1;
  }

  while (b < len2 && a < len1) {
    if(pa[a] == pb[b]) {
      doc_list[i] = pa[a];
      a++;
      b++;
    } else if (pa[a] < pb[b]) {
      doc_list[i] = pa[a];
      a++;
    } else {
      if (i > 0 && doc_list[i-1] < pb[b]) doc_list[i] = pb[b]; 
      b++;
    }
    i++;
  }
  while (a < len1) {
    doc_list[i] = pa[a];
    a++;
    i++;
  }
  while (b < len2) {
    doc_list[i] = pb[b];
    b++;
    i++;
  } 
  if (i == 0) {
    cout << "OR操作后结果集为空!" << endl;
    return -1;
  }

  return i;
}

// A AND NOT B
// if len1 == -1      return  -1
// after operation NOT, length = 0, return -1
int negation(UINT_32 &pa, UINT_32 &pb, int len1, int len2, UINT_32 &doc_list) {
  int i = 0, a=0, b=0;
  initL(&doc_list, len1);
  if (len1==-1) {
    cout << "对不起，没有您要查找的关键字!" << endl;
    return -1;
  }
  while (b < len2 && a < len1) {
    if(pa[a] == pb[b]) {
      a++;
      b++;
    } else if (pa[a] < pb[b]) {
      doc_list[i] = pa[a]; 
      i++;
      a++;
    } else {
      b++;
    }
  }

  while (a < len1) {
    doc_list[i] = pa[a];
    a++;
    i++;
  }
  if (i == 0) {
    cout << "NOT操作后结果集为空!" << endl;
    return -1;
  }

  return i;

}

// user input words
// transform words to keywords and operation
int transformStr(STRING &tranStr, STRING &tranOps) {
  tranStr = (STRING)calloc(128, sizeof(string));
  tranOps = (STRING)calloc(128, sizeof(string));
  char word[100];
  btree_t* bt;
  bt = btree_create(8);
  cout << "请输入要查找的单词：";
  cin.get(word, 100);
  int num = 0;
  int i = 0;
  if (cin.rdstate() == ios::failbit) { 
    cout << "对不起，您输入的关键字为空！" << endl;
    return -1;
  }

  //将输入的单词存入数组
  const char* split = " ,.-?";
  char* ptr;
  string* s = (string*)calloc(128, sizeof(string));
  ptr = strtok(word, split);
  if (ptr == NULL) {
    cout << "对不起，您的输入有误！" << endl;
    return -1;
  }
  while (ptr != NULL) {
    s[i] = ptr;
    ptr = strtok(NULL, split);
    i++;
  }
  num = i;

  string tmp;
  //for 重新排序 s[]
  int m = 0;
  int n = 0;
  for (i = 0; i < num; i++) {
    if (s[i] == "AND" || s[i] == "OR" || s[i] == "NOT") {
      if (s[i+1] == "NOT" && s[i] == "AND") {   //如果操作符为NOT，且与AND并列，需先存入NOT，再存AND
        tranOps[m+1] = s[i]; 
        tranOps[m] = s[i+1];
        i++;
        m++;
      } else { 
        if (i != 0 && tranOps[m-1] == s[i-1]) {
          cout << "对不起，您的输入不符合语义，请重新输入！" << endl;
          return -1;
        }
        tranOps[m] = s[i];
      }
      m++;
    } else { 
      if (i != 0 && tranStr[n-1] == s[i-1]) {
        cout << "对不起，您的输入不符合语义，请重新输入！" << endl;
        return -1;
      }
      tranStr[n] = s[i];
      n++;
    }
  }
  //如果第一个操作符为NOT，交换关键字位置再进行操作
  if (strcmp(s[0].c_str(), "NOT") == 0) {
    tmp = tranStr[0];
    tranStr[0] = tranStr[1];
    tranStr[1] = tmp;
  }
  free(s);
  return num;
}

// convert pos(streampos) to doc_list(uint32_t*)
int transformPos(streampos pos, UINT_32 &doc_list) {
  ifstream in("InvertedIndex.txt",ios::in);
  if (!in.is_open()) {
    cout << "file open fail\n";
    return -1;
  }
  in.seekg(pos);
  uint32_t codeSize;
  unsigned char *codes = (unsigned char*)malloc(codeSize*sizeof(unsigned char));
  in >> codeSize;
  in.get();
  char code;
  for (uint32_t i = 0; i < codeSize; i++) {
    in.get(code);
    codes[i] = (unsigned char) code;
  }
  GammaDecoder gammDecoder(codes, codeSize);
  doc_list = gammDecoder.decode();
  uint32_t df = gammDecoder.getValuesNum();

  free(codes);
  in.close();

  return df;
}

// traverse
void visit(UINT_32 &doc_list, int len) {
  for (uint32_t i = 0; i < len; i++) {
      cout << doc_list[i] << " ";
    }
    cout << endl;
}


int main()
{
  btree_t *  bt;
  bt = btree_create(8);
  cout << ">>正在构建B树，请稍后……"<<endl;
  addDic("InvertedIndex.txt",bt);
  cout << ">>词库添加完毕！" << endl;
  cout << "===操作符请使用大写，否则，将视为关键字处理^_^===" << endl;
  STRING tranStr;
  STRING tranOps;
  int result = transformStr(tranStr, tranOps);
  if (result == -1)
    return -1;

  int n = 0;
  int len1 = -1;
  int len2 = -1;
  UINT_32 doc_list;
  if (result == 1) {
    cout << "Only one word: " << tranStr[n] << endl;
    streampos pos = bt->search(tranStr[n]);
    if (pos == -1) {
      cout << "对不起，没有您要搜索的关键字！请重新输入" << endl;
      return -1;        
    }
    int len = transformPos(pos, doc_list);
    if (len == -1) 
      return -1;
    else {
      visit(doc_list, len);
      return 0;
    }
  } else if (tranOps[n].empty()) {
    cout << "对不起，您输入的语义有误，请重新输入！" << endl;
    return -1;    
  } else {
    int m = 1;
    streampos pos1 = bt->search(tranStr[m-1]);
    UINT_32 docL1;
    if (pos1 != -1) 
      len1 = transformPos(pos1, docL1);
    streampos pos2 = bt->search(tranStr[m]);
    UINT_32 docL2;
    if (pos2 != -1)
      len2 = transformPos(pos2, docL2);

    while (!tranOps[n].empty()) {
      if (tranOps[n] == "AND") {
        len1 = (len1 > len2) ? intersect(docL1, docL2, len1, len2, doc_list) :intersect(docL2, docL1, len2, len1, doc_list);
      } else if (tranOps[n] == "OR") {
        len1  = (len1 > len2) ? unionDocIds(docL1, docL2, len1, len2, doc_list) : unionDocIds(docL2, docL1, len2, len1, doc_list);
      } else if (tranOps[n] == "NOT") {
        len1 = negation(docL1, docL2, len1, len2, doc_list);
        n++;  
      }
      if (!tranStr[m+1].empty()) {
        docL1 = doc_list;
        pos2 = bt->search(tranStr[m+1]);
        len2 = transformPos(pos2, docL2);
      }
      m++;
      n++;
    }

  }

  if (len1 == -1) { 
    cout << "您查找的最终结果为空！" << endl;
    return -1;
  } else {
    cout << "OUTPUT: " << endl;
    visit(doc_list, len1);
  }

  free(tranStr);
  free(tranOps);
  free(doc_list);
  return 0;

}
