#include <iostream>
using namespace std;

class Node
{
public:

	Node(int ID = 0, Node* pnext = NULL){
		docID = docID;
		next = pnext;
	}
	void SetID( int ID ){
		docID = ID;
	}
	void Setnext( Node* pnext ){
		next = pnext;
	}
	int GetID(){
		return docID;
	}
	Node* Getnext(){
		return next;
	}

protected:

	int docID;
	Node* next ;

};

class List
{
public:

	List(Node lhead, Node* ltail = NULL, int llength = 0){
		head = lhead;
		tail = &head;
		length = llength;
	}
	void addnode( Node p ){
		length ++;
		tail->Setnext( &p );
		tail = &p;
	}
	void deletenode( Node p ){
	}
protected:

	Node head;
	Node* tail;
	int length;

};

class Token                                  //此为单词条存储格式
{
public:

	char *str;                                  // 词条名称
	List Index;                                // 词条索引链表

};