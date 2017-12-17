#include "single_string_dict.cpp"
struct List		//索引表
{
	int DocId;
	List *next;
};

typedef struct dictnode
{
    int str;
	//unsigned int df;//文档频率
	List *ListHead;//倒排索引表头
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
    List * search(string);
    metastr dictionary;
}btree_t;  
