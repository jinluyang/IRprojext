#include <iostream>
#include <vector>
#include <string>
#include "classes.cpp"
#include <stdio.h>
#include <string.h>
using namespace std;

/* B树结点结构 */  
typedef struct dictnode
{
    string str;
    //TODO daopaijilubiao zaizifuchuanzhongdeweizhi 
}dictnode;

//typedef struct _btree_node_t  
//{  
//    int num;                        /* 关键字个数 */  
//    vector<dictnode>  keys;                      /* 关键字：所占空间为(max+1) - 多出来的1个空间用于交换空间使用 */  
//    vector<btree_node_t> child;   /* 子结点：所占空间为（max+2）- 多出来的1个空间用于交换空间使用 */  
//    struct _btree_node_t *parent;   /* 父结点 */  
//}btree_node_t;  
class btree_node_t
{  
public:
    int num;                        /* 关键字个数 */  
    vector<dictnode>  keys;                      /* 关键字：所占空间为(max+1) - 多出来的1个空间用于交换空间使用 */  
    vector<btree_node_t*> child;   /* 子结点：所占空间为（max+2）- 多出来的1个空间用于交换空间使用 */  
    btree_node_t *parent;   /* 父结点 */  
    btree_node_t(){
            num = 0;
//    this->keys = &(dictnode[m]);
    }
};  

/* B树结构 */  
typedef struct  
{  
    int max;                        /* 单个结点最大关键字个数 - 阶m=max+1 */  
    int min;                        /* 单个结点最小关键字个数 */  
    int sidx;                       /* 分裂索引 = (max+1)/2 */  
    btree_node_t *root;             /* B树根结点地址 */  
}btree_t;  

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
    cout << "create" << endl;
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
        cout << "aaea" << endl;
  
    /* 1. 构建第一个结点 */  
    if(NULL == node) {  
            cout << "first" << endl;
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
  
//            parent->keys.push_back(node->keys[sidx]);  
            parent->child[1] = node2;
//            parent->child.push_back(node2);  
            parent->num++;  
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


//class Btree
//{
//	public:
//	Btree();
//	void insert();
//};

int main()
{
	string dictionary;
	dictionary = "a\0am\0as\0bus\0";
	cout << dictionary << endl;
	//string s;
	//cin >> s;
	//cout<< s;
        btree_t *  bt;
        bt = btree_create(4);
//        if ( bt->root==NULL)
//        {
//          cout <<"yes"<< endl;
//        }
        dictnode dnd ;
        dnd.str="a";
        btree_insert(bt,dnd);
        btree_insert(bt,dnd);
        dnd.str="as";
        btree_insert(bt,dnd);
        dnd.str="am";
        btree_insert(bt,dnd);
        

	return 0;
}
