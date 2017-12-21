#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "btree.cpp"
using namespace std;

typedef struct List *LinkList;
void muliIntersect();

int InitList (LinkList *L);
void intersect_L(LinkList &La, LinkList &Lb, LinkList &Lc);
void union_L(LinkList &La, LinkList &Lb, LinkList &Lc);
void negation_L(LinkList &La, LinkList &Lb, LinkList &Lc);
int listTraverse(LinkList L, void(*vi)(LinkList&));
void visit(LinkList &node);

int InitList (LinkList *L) {
    *L = (LinkList)malloc(sizeof(struct List));
    if(!*L)
        exit(-2);
    (*L)->next = NULL;
    return 1;
}

// AND
void intersect_L(LinkList &La, LinkList &Lb, LinkList &Lc) {
    LinkList pa, pb, pc;
    pa = La->next; pb = Lb->next;
    Lc = pc = La;
    while (pa && pb) {
        if (pa->DocId == pb->DocId) {
            pc = pa; 
            pa = pa->next;
        } else if (pa->DocId < pb->DocId) {
            pc->next = pa->next;
            free(pa);
            pa = pc->next;
        } else
            pb = pb->next;
    }
    pc->next = NULL;
    while (pa) {
        pc = pa;
        pa = pc->next;
        free(pc);
    }
    pb = Lb;
    while (pb) {
        pc = pb;
        pb = pc->next;
        free(pc);
    }

}

// OR
void union_L(LinkList &La, LinkList &Lb, LinkList &Lc) {
    LinkList pa, pb, pc;
    pa = La->next; pb = Lb->next;
    Lc = pc = La;
    while (pa && pb) {
        if (pb->DocId > pa->DocId) {
            pc = pa; 
            pa = pa->next;
        } else {
            pc->next = pb;
            pb = pb->next;
            pc = pc->next;
            pc->next = pa;
        }
    }
    while (pa) {
        pc = pa;
        pa = pa->next;
    }
    while (pb){
        pc->next = pb;
        pb = pb->next;
        pc = pc->next;
        pc->next = pa;
    }

}

// AND NOT
// A AND NOT B
void negation_L(LinkList &La, LinkList &Lb, LinkList &Lc) {
    LinkList pa, pb, pc;
    pa = La->next; pb = Lb->next;
    Lc = pc = La;
    while (pa && pb) {
        if (pa->DocId == pb->DocId) {
            pc->next = pa->next;
            free(pa);
            pa = pc->next;
        } else if (pa->DocId < pb->DocId) {
            pc = pa;
            pa = pa->next;
        } else
            pb = pb->next;
    }
    pc->next = NULL;
    pb = Lb;
    while (pb) {
        pc = pb;
        pb = pc->next;
        free(pc);
    }

}

// Traverse
int listTraverse(LinkList L, void(*vi)(LinkList&)) {
    LinkList p = L->next;
    while (p != NULL) {
        vi(p);
        p = p->next;
    } 
    return 1;
}

void visit(LinkList &node) {
    cout << node->DocId << endl;
}

//循环添加文件，实现多个布尔查询，例如输入：a AND about OR accept AND NOT able
//返回：1 3 5 7 9 12
//输入的关键字全部存入string[]中
//暂时不支持括号内查询
//暂不支持区分大小写，操作符必须大写
void muliIntersect() {
    char word[100];
    btree_t* bt;
    bt = btree_create(8);
    cout << "请输入要查找的单词：";
    cin.get(word, 100);
    int num = 0;
    int i = 0;
    
    //将输入的单词存入数组
    const char* split = " ";
    char* p;
    char* ptr;
    char** str = (char**)calloc(128, sizeof(char*));
    string* s = (string*)malloc(sizeof(string) * 10);
    ptr = strtok_r(word, split, &p);
    str[0] = ptr;
    s[0] = str[0];
    while (ptr != NULL) {
        str[i] = ptr;
        s[i] = str[i];
        ptr = strtok_r(NULL, split, &p);
        i++;
    }
    num = i;

    string tmp;
    string words[num];
    string ops[num];
//for 重新排序 s[]
    int m = 0;
    int n = 0;
    for (i = 0; i < num; i++) {
        if (strcmp(s[i].c_str(), "AND")==0 || strcmp(s[i].c_str(), "OR")==0 || strcmp(s[i].c_str(), "NOT")==0) {
  //          transform(s[i].begin(), s[i].end(), s[i].begin(), ::toupper);     //将操作符小写转换为大写
            if (strcmp(s[i+1].c_str(), "NOT") == 0 && strcmp(s[i].c_str(), "AND") == 0) {   //如果操作符为NOT，且与AND并列，需先存入NOT，再存AND
                ops[m+1] = s[i]; 
    //            transform(s[i+1].begin(), s[i+1].end(), s[i+1].begin(), ::toupper);
                ops[m] = s[i+1];
                i++;
            } else 
                ops[m] = s[i];
            m++;
        } else { 
            words[n] = s[i];
            n++;
        }
    }
    //如果第一个操作符为NOT，交换关键字位置再进行操作
    if (strcmp(ops[0].c_str(), "NOT") == 0) {
        tmp = words[0];
        words[0] = words[1];
        words[1] = tmp;
    }

/*  打印关键字和操作符
    i=0;
    while (!words[i].empty()) {
        cout << words[i] << endl;
        i++;
    }
    i=0;
    while (!ops[i].empty()) {
        cout << ops[i] << endl;
        i++;
    }
*/	
    addDic("InvertedIndex.txt",bt);
    LinkList A, B, C;
    InitList(&A);
    InitList(&B);
    m=1;n=0;
    try {
    A = bt->search(words[m-1]);
    B = bt->search(words[m]);
    } catch (exception &e) {
        cout << e.what();
    }
 
    while (!ops[n].empty()) {
        if ((A->next==NULL || B->next==NULL) && n==0) {
            cout << "ERROR: 没有您要检索的关键字！请检查后重新启动" << endl;
            break;
        } 
        if (A->next==NULL || B->next==NULL) 
            break;
        else {
            if(ops[n] == "AND") 
                intersect_L(A, B, C);
            else if (ops[n] == "OR") 
                union_L(A, B, C);
            else if (ops[n] == "NOT") {
                negation_L(A, B, C);
                n++;
            }
        }
        if (!words[m+1].empty()) {
            A = C;
            B = bt->search(words[m+1]);
        }
             
        m++;
        n++;
    }
    cout << "Output: " << endl; 
    listTraverse(C, visit);

}

int main()
{
    muliIntersect();

	return 0;
}

