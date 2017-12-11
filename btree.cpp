#include <iostream>
#include <vector>
#include <string>
//#include "classes.cpp"
#include <stdio.h>
#include <string.h>
#include<fstream>
#include<sstream>
//#include<conio.h>
#include "single_string_dict.cpp"
#include "btree.h"
using namespace std;


static int btree_split(btree_t *btree, btree_node_t *node) ; 
static int _btree_insert(btree_t *btree, btree_node_t *node, dictnode key, int idx); 
int btree_insert(btree_t *btree, dictnode key); 

btree_t *  btree_create( int m)  
{  
    btree_t *btree = NULL;  
  
    if(m < 3) {  
        cout << "Parameter 'max' must geater than 2.\n";  
//        exit(0);
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
  
//    _btree = btree;  
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
/////here
static btree_node_t *btree_creat_node(btree_t *btree) // using its max 
{  
    btree_node_t *node = NULL;  
  
  
    //node = (btree_node_t *)calloc(1, sizeof(btree_node_t));  
//    cout << "create" << endl;
    node = new(btree_node_t);  
  
    node->num = 0;  
    node->child.resize(btree->max+2);
    node->keys.resize(btree->max+1);
  
    /* More than (max) is for move */  
//    node->key = (int *)calloc(btree->max+1, sizeof(int));  
//    if(NULL == node->key) {  
//        delete(node), node=NULL;  
//        return NULL;  
//    }  
//  
//    /* More than (max+1) is for move */  
//    node->child = (btree_node_t **)calloc(btree->max+2, sizeof(btree_node_t *));  
//    if(NULL == node->child) {  
//        delete(node->key);  
//        delete(node), node=NULL;  
//        cout << "err\n";  
//        return NULL;  
//   }  
  
    return node;  
}  

int btree_insert(btree_t *btree, dictnode key)  
{  
//mainly search
    int idx = 0;  
    btree_node_t *node = btree->root;  
//        cout << "aaea" << endl;
  
    /* 1. 构建第一个结点 */  
    if(NULL == node) {  
//            cout << "first" << endl;
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
                return 0;  
            }  
            else if(key.str < node->keys[idx].str) {  
                break;  
            }  
        }  
  
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
//    dictnode tmp = new(dictnode);
//    node->keys.push_back(tmp);
//    node->keys.resize(node->keys.size()+1);
    for(i=node->num; i>idx; i--) {  
        node->keys[i] = node->keys[i-1];  
    }  
  
    node->keys[idx] = key; /* 插入 */  
    node->num++;  
  
    /* 2. 分裂处理 */  
    if(node->num > btree->max) {  
//            cout <<"split"<<endl;
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
  
/////here
        /* Copy data */   
// copy the keys and child of node to node2
        for (int j = sidx+1;j < total ;j++)
        {
//        memcpy(node2->keys, node->keys + sidx + 1, (total-sidx-1) * sizeof(int));  
//        memcpy(node2->child, node->child+sidx+1, (total-sidx) * sizeof(btree_node_t *));  
//            node2->keys.push_back(node->keys[j]);
//            node2->child.push_back(node->child[j]);
            node2->keys[j-sidx-1]=node->keys[j];
            node2->child[j-sidx-1]=node->child[j];
        }
        node2->child[total]=node->child[total];
  
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
//            parent->child.push_back(node);   
            parent->child[0] = node;
            node->parent = parent;   
            node2->parent = parent;   
  
            parent->keys[0]=node->keys[sidx];  
            parent->child[1] = node2;
//            parent->child.push_back(node2);  
            parent->num++;  
//            cout << parent->num<<endl;
//            cout <<"sidx="<< sidx<<endl;
//            cout << node->keys[sidx].str<<endl;
//            cout <<"lenth of parent->key"<< parent->keys.size()<<endl;
//            cout << parent->keys[0].str<<endl;
        }  
        else {         
            /* Insert into parent node */   
//                parent->keys.resize(parent->keys.size()+1)
//                parent->child.resize(parent->child.size()+1)
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
            parent->num++;  
        }         
  
//        memset(node->keys+sidx, 0, (total - sidx) * sizeof(int));  
//        memset(node->child+sidx+1, 0, (total - sidx) * sizeof(btree_node_t *));  
  
        node->keys.resize(sidx);//+1?
        node->child.resize(sidx+1);
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

List * btree_t::search(string query)
{
	btree_node_t * node = this->root;
        int idx;
	/*while(p->str.compare(query)!=0)
	{
		if (p->str < query)
		{
			
		} else {
			
		}
	} */   
	while(NULL != node) { 
        for(idx=0; idx<node->num; idx++) {  
//                cout <<node->keys[idx].str<<endl;
            if(query == node->keys[idx].str) {  
                cout << " found the query"<<endl; 
                return node->keys[idx].ListHead; 
            }  
            else if(query < node->keys[idx].str) {  
//                cout << "index:"<< idx<< endl;
                break;  
            }  
        }  
  
        if(NULL != node->child[idx]) {  
//                cout << node->keys[0].str;
            node = node->child[idx]; //in the next loop , continue searching 
//            cout <<"down"<<endl;
        }  
        else {  
		cout <<"failed"<< endl;
		return NULL;
            break;  
        }  
    }  

//	return node->;
}
void print_bt(btree_t * bt)
{
    btree_node_t * node = bt->root;
    cout << "num"<< node->num<<endl;
    for (int i=0;i<node->num;i++)
    {
        cout <<node->keys[i].str <<endl;
    }
    cout <<"****"<<endl;
//    node = node->child[0];
}
void addDic(string filename, btree_t * bt)
{
	//string filename = "test.txt"; 
	ifstream in(filename,ios::in);
		if (!in)
		{
			cout << "文件不存在！" << endl;

//			getch();
	//		exit(-1);
		}
	string line,word,word1; 
	int num = 0;
	while(in)
	{
		getline(in, line);

		string::iterator it;

		//去除逗号，冒号，括号，引号等非必要字符
		//for(int i = 0;i<line.size();i++)
//		for(it =line.begin(); it != line.end(); it++)
//		{
//			//if (line[i] == ',')//||line[i]=='\''||line[i]=='['||line[i]=='{'||line[i]=='}')
//			//	line[i] = ' ';
//			//else if (line[i]=='\''||line[i]=='['||line[i]=='{'||line[i]=='}'||line[i]==':')
//			//	line[i] = NULL;
//		//	int tmp = 0;
//		//	if (tmp == 1) {tmp = 0;it--;}
//			if (*it == ',')//||line[i]=='\''||line[i]=='['||line[i]=='{'||line[i]=='}')
//				*it = ' ';
//			else if (*it=='\''||*it=='['||*it=='{'||*it=='}'||*it==':')
//			{			
//				//tmp = tmp + 1;
////				if (tmp == 1) {tmp = 0;it--;}
//		//		tmp = tmp + 1;
//				line.erase(it);
//				if (it == line.begin()) it = line.begin();
//				else it--;
//			}
//		}

		stringstream ss;
		ss << line;
		ss >> word;		//提取出单词
		
		//cout<<word<<endl;
		int j = 0;
		int value = 0;
		List *Head = new List;
		List *Tail = Head;
		dictnode dnd;
		while(j<line.size())	//提取文档编号串
		{
			while(j<line.size()&&(line[j] > '9' || line[j] < '0'))
			{j++;}
		
			int value = 0;
			while(j<line.size()&&line[j] != ' ')
			{
				//cout<<line[j];
				value = value * 10 + line[j] - '0';
				j++;
			}
			j++;
			//cout << value << ' ';

			List *Lnew = new List;		//文档ID链表

			Lnew->DocId = value;	
			Tail->next = Lnew;
			Lnew->next = NULL;
			Tail = Lnew;
		}
//		addNode(word, Head);
		dnd.str = word;
		dnd.ListHead = Head;
		btree_insert(bt, dnd);
//                print_bt(bt);
	}
}
//class Btree
//{
//	public:
//	Btree();
//	void insert();
//};
int main()
{
	string dictionary;
	dictionary = "a$am$as$bus$";
//	cout << dictionary << endl;
        add(dictionary,"but");
//	cout << dictionary << endl;
	//string s;
	//cin >> s;
	//cout<< s;
        btree_t *  bt;
        bt = btree_create(8);
//        if ( bt->root==NULL)
//        {
//          cout <<"yes"<< endl;
//        }
        dictnode dnd ;
        dnd.str="a";
//        btree_insert(bt,dnd);
//        btree_insert(bt,dnd);
//        dnd.str="as";
//        btree_insert(bt,dnd);
//        dnd.str="am";
//        btree_insert(bt,dnd);
        
//        auto dict = new(item)
	string word;   //要查找的单词
	cout << ">>正在添加词典，请稍后……"<<endl;
	addDic("InvertedIndex1.txt",bt);
	cout << ">>词库添加完毕！" << endl;
        //print_bt(bt);
	cout << "\n请输入要查找的单词：";
	cin >> word;
	List * res = bt->search(word);
	if (res==NULL)
		cout <<"fail!"<< endl;
	else
            while(res->next!=NULL)
            {
		cout << " "<<res->next->DocId<<endl;
                res=res->next;
            }
//        if (res->next==NULL)
//                cout << "next NULL"<<endl;

	return 0;
}
