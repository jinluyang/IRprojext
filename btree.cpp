#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include<fstream>
#include<sstream>
#include "btree.hpp"
#include "gammaCode.hpp"
using namespace std;

//#define BTREE_DEBUG
//#define BTREE_OUTPUT

static int btree_split(btree_t *btree, btree_node_t *node) ; 
static int _btree_insert(btree_t *btree, btree_node_t *node, dictnode key, int idx); 
int btree_insert(btree_t *btree, dictnode key); 
void print_bt(btree_t * bt);

btree_t *  btree_create(int m)  
{  
  btree_t *btree = NULL;  

  if(m < 3) {  
    cout << "Parameter 'max' must geater than 2.\n";  
    return NULL;  
  }  

  btree = new(btree_t);  

  btree->max= m - 1;  
  btree->min = m/2;  
  /////shang qu zheng - 1
  if(0 != m%2) {  
    btree->min++;  
  }  
  btree->min--;  
  btree->sidx = m/2;  
  btree->root = NULL; /* 空树 */  
  btree->dictionary.add("");

  return btree;  
}  

/****************************************************************************** 
 **函数名称: btree_creat_node 
 **功    能: 新建结点 
 **输入参数:  
 **     btree: B树 
 **输出参数: NONE 
 **返    回: 节点地址
 ******************************************************************************/  
static btree_node_t *btree_creat_node(btree_t *btree) // using its max 
{  
  btree_node_t *node = NULL;  
  node = new(btree_node_t);  
  node->num = 0;  
  //TODO initial size too small may result in memory error resizing
  node->child.resize(btree->max+2);
  node->keys.resize(btree->max+1);
  node->parent=NULL;
  return node;  
}  

int btree_insert(btree_t *btree, dictnode key, string word)  
{  
  key.str = btree->dictionary.add(word);
  //mainly search
  int idx = 0;  
  btree_node_t *node = btree->root;  

  /* 1. 构建第一个结点 */  
  if(NULL == node) {  
    node = btree_creat_node(btree);  
    if(NULL == node) {  
      cout << " Create node failed!\n";  
      return -1;  
    }  

    node->num = 1;  
    node->keys[0]=key;  
    node->parent = NULL;  

    btree->root = node;  
    return 0;  
  }  

  /* 2. 查找插入位置：在此当然也可以采用二分查找算法 */  
  while(NULL != node) {  
    for(idx=0; idx<node->num; idx++) {  
      if(key.str == node->keys[idx].str) {  
        cout << " The node exists!\n"; 
        //TODO pop the dictionaty , but a assume no duplicates now
        return 0;  
      }  
      else if(key.str < node->keys[idx].str) {  
        break;  
      }  
    }  
    //no problem with this above 
    if(NULL != node->child[idx]) {  
      node = node->child[idx]; //in the next loop , continue searching 
    }  
    else {  
      break;  
    }  
  }  

  /* 3. 执行插入操作 */  
  return _btree_insert(btree, node, key, idx);  
}

static int _btree_insert(btree_t *btree, btree_node_t *node, dictnode key, int idx)  
{  
  int i ;  

  /* 1. 移动关键字:首先在最底层的某个非终端结点上插入一个关键字,因此该结点无孩子结点，故不涉及孩子指针的移动操作 */  
  /*-------------------*/
  for(i=node->num; i>idx; i--) {  
    node->keys[i] = node->keys[i-1];  
  }  

  node->keys[idx] = key; /* 插入 */  
  node->num++;  

  /* 2. 分裂处理 */  
  if(node->num > btree->max) {  
    return btree_split(btree, node);  
  }  

  return 0;  
}

static int btree_split(btree_t *btree, btree_node_t *node)  
{ //如果超过m，分裂 
  int idx = 0, total = 0, sidx = btree->sidx;  
  btree_node_t *parent = NULL, *node2 = NULL;   


  while(node->num > btree->max) { 
    /* Split node */   
    total = node->num;  

    node2 = btree_creat_node(btree);  
    if(NULL == node2) {     
      cerr << "Create node failed!\n"<< endl;  
      return -1;  
    }  

    /* Copy data */   
    // copy the keys and child of node to node2
    for (int j = sidx+1;j < total ;j++)
    {
      node2->keys[j-sidx-1]=node->keys[j];
      node2->child[j-sidx-1]=node->child[j];
    }
    node2->child[total-sidx-1]=node->child[total];//bug here
    node2->num = (total - sidx - 1);  
    node2->parent  = node->parent;  

    node->num = sidx;   
    /* Insert into parent */  
    parent  = node->parent;  
    if(NULL == parent)  {      
      /* Split root node */   
      parent = btree_creat_node(btree);  
      if(NULL == parent) {         
        cout << " Create root failed!";  
        return -1;  
      } 
      btree->root = parent;   
      parent->child[0] = node;
      node->parent = parent;   
      node2->parent = parent;   
      parent->keys[0]=node->keys[sidx];  
      parent->child[1] = node2;
      parent->num++;  
    }
    else {
      /* Insert into parent node */   
      for(idx=parent->num; idx>0; idx--) {         
        if(node->keys[sidx].str < parent->keys[idx-1].str) {         
          parent->keys[idx] = parent->keys[idx-1];  
          parent->child[idx+1] = parent->child[idx];  
          continue;  
        }  
        break;  
      }         

      parent->keys[idx] = node->keys[sidx];  
      parent->child[idx+1] = node2;  
      node2->parent = parent;   
      parent->num++;  }         

    /* Change node2's child->parent */  
    for(idx=0; idx<=node2->num; idx++) {  
      if(NULL != node2->child[idx]) {         
        node2->child[idx]->parent = node2;  
      }         
    }         
    node = parent;   
  }  

  return 0;  
}  

streampos btree_t::search(string query)
{
  btree_node_t * node = this->root;
  int idx;
  while(NULL != node) { 
    for(idx=0; idx<node->num; idx++) {  
#ifdef BTREE_OUTPUT
      cout << "----"<< endl;
      cout <<this->dictionary.getStr(node->keys[idx].str)<<endl;
#endif
      if(query == this->dictionary.getStr(node->keys[idx].str)) {  
        cout << " found the query"<<endl; 
        return node->keys[idx].fpos; 
      } else if(query <  this->dictionary.getStr(node->keys[idx].str)) {  
        break;  
      }  
    }  

    if(NULL != node->child[idx]) {  
      node = node->child[idx]; //in the next loop , continue searching 
#ifdef BTREE_OUTPUT
      cout <<"down"<<endl;
#endif
    } else {  
      cout <<"failed"<< endl;
      return -1;
    }  
  }  
}

void print_bt(btree_t * bt)
{
  btree_node_t * node = bt->root;
  btree_node_t * node2 = bt->root;
  cout << "num"<< node->num<<endl;
  for (int i=0;i<node->num;i++)
  {
    cout <<bt->dictionary.getStr(node->keys[i].str) <<endl;
  }
  cout <<"****"<<endl;
}

void addDic(string filename, btree_t * bt)
{
  ifstream in(filename,ios::in);
  if (!in)
  {
    cout << "文件不存在！" << endl;
    exit(-1);
  }
  string line,word,word1; 
  int num = 0;
  uint32_t codeSize;  // size of gamma codes
#ifdef BTREE_DEBUG
  ofstream of("invertedindex.txt", ios::out);
  if (!of.is_open()) {
    std::cout << "can't open \"invertedindex.txt\"";
    return;
  }
#endif
  while(in >> word)
  {
    string::iterator it;

    streampos fpos = in.tellg();
    in >> codeSize;
    in.get();  // jump the space after codeSize
#ifdef BTREE_DEBUG
    unsigned char *codes = (unsigned char*)malloc(codeSize*sizeof(unsigned char));
    char code;
    for (uint32_t i = 0; i < codeSize; i++) {
      in.get(code);
      codes[i] = (unsigned char)code;
    }
    GammaDecoder gammDecoder(codes, codeSize);
    uint32_t *doc_list = gammDecoder.decode();
    uint32_t df = gammDecoder.getValuesNum();
    of << word << " " << df << " ";
    for (uint32_t i = 0; i < df; i++) {
      of << doc_list[i] << " ";
    }
    of << "\n";
    free(codes);
    free(doc_list);
#else
    // Jump codes.
    for (uint32_t i = 0; i < codeSize; i++) {
      in.get();
    }
#endif
#ifdef BTREE_OUTPUT
    cout << word << " " << codeSize;
    cout << " fpos: " << fpos << "\n";
#endif
    dictnode dnd;
    dnd.fpos = fpos;
    btree_insert(bt, dnd,word);
  }
#ifdef BTREE_DEBUG
  of.close();
#endif
  in.close();
}

//int main()
//{
//  btree_t *  bt;
//  bt = btree_create(8);
//	string word;   //要查找的单词
//	cout << ">>正在构建B树，请稍后……"<<endl;
//	addDic("InvertedIndex.txt",bt);
//	cout << ">>词库添加完毕！" << endl;
//        //print_bt(bt);
//	cout << "\n请输入要查找的单词：";
//  cin >> word;
//  streampos pos = bt->search(word);
//  ifstream in("InvertedIndex.txt",ios::in);
//  if (!in.is_open()) {
//    std::cout << "file open fail\n";
//    return -1;
//  }
//  if (pos == -1) {
//    // NOT FOUND!
//    cout << "NOT FOUND!\n";
//    return -1;
//  }
//  in.seekg(pos);
//  uint32_t codeSize;
//  unsigned char *codes = (unsigned char*)malloc(codeSize*sizeof(unsigned char));
//  in >> codeSize;
//  in.get();  // jump the space after codeSize
//  char code;
//  for (uint32_t i = 0; i < codeSize; i++) {
//    in.get(code);
//    codes[i] = (unsigned char)code;
//  }
//  GammaDecoder gammDecoder(codes, codeSize);
//  uint32_t *doc_list = gammDecoder.decode();
//  uint32_t df = gammDecoder.getValuesNum();
//  for (uint32_t i = 0; i < df; i++) {
//    std::cout << doc_list[i] << " ";
//  }
//  std::cout << "\n";
//  free(codes);
//  free(doc_list);
//  in.close();
//
//	return 0;
//}
