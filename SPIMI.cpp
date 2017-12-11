#include<iostream>
#include<fstream>
#include<string.h>
#include<sstream>
#include<math.h>

#include "gammaCode.hpp"
using namespace std;

#define INF "~"

class Node
{
public:

	Node():docID(-1),tf(1), next(NULL){
	}

	void SetID( int ID ){
		docID = ID;
	}

	void Setnext( Node* pnext ){
		next = pnext;
	}

	void Settf( int ltf ){
		tf = ltf;
	}

	int Gettf(){
		return tf;
	}

	int GetID(){
		return docID;
	}

	Node* Getnext(){
		return next;
	}

protected:

	int docID;
	int tf;
	Node* next ;

};

class List
{
public:

	List():head(), tail(&head), df(0){
	}
	/*~List(){
		Node* cur = &head;
		Node* dp = NULL;
		while(cur->Getnext() != NULL){
			dp = cur;
			cur = cur->Getnext();
			delete dp;
		}
		delete cur;
	}*/

	Node& Gethead(){
		return head;
	}

	Node* Gettail(){
		return tail;
	}

	void Settail( Node* ltail ){
		tail = ltail;
	}

	int Getdf(){
		return df;
	}

	void Setdf( int lfreq ){
		df = lfreq;
	}
	
	void addnode( int  docid , int tf){
		Node *cur = &head;
		while( cur->Getnext() != NULL ){
			if( cur->Getnext()->GetID() ==  docid ){
				cur->Getnext()->Settf(cur->Getnext()->Gettf() + tf);
				break;
			}
			else if( cur->Getnext()->GetID() >  docid ){
				Node* p = new Node;
				Node* q = cur->Getnext();
				p->SetID(docid);
				p->Settf(tf);
				cur->Setnext(p);
				p->Setnext(q);
				df ++;
				break;
			}
			cur = cur->Getnext();
		}
		if( cur == tail ){
			Node* p = new Node;
			p->SetID(docid);
			p->Settf(tf);
			cur->Setnext(p);
			tail = p;
			df++;
		}
	}

	void deletenode( int p ){
		Node *cur = &head;
		Node *pd = NULL;
		while( cur->Getnext() != NULL){
			if(cur->Getnext()->GetID() == p){
				pd = cur->Getnext();
				cur->Setnext(pd->Getnext());
				delete pd;
				df --;
				break;
			}
			cur = cur->Getnext();
		}
	}

  uint32_t * list2Array() {
    uint32_t* p = (uint32_t*)malloc(df*sizeof(uint32_t));
    Node *cur = &head;
    int i = 0;
    while(cur->Getnext() != NULL){
      p[i] = cur->Getnext()->GetID();
      cur = cur->Getnext();
      i++;
    } 
    return p;               //Delete In Time
  }

  void array2List(uint32_t* p, int len) {
    for(int i = 0; i < len; i++)
      addnode(p[i], 1);
  }

protected:

	Node head;
	Node* tail;
	int df;

};

class Term                                  //此为单词项存储格式
{

public:

	Term* next;

	Term():str(""), index(), next(NULL){	
	}

	void Setstr( string s ){
		str = new char(sizeof(s));
		str = s;
	}

	string Getstr(){
		return str;
	}

	List& Getlist(){
		return index;
	}

protected:

	string str;                                  // 词项名称
	List index;                                // 词项索引链表


};

class Dictionary
{

public:
	Term dhead;
	int dlen;
	Term* dtail;

	Dictionary(): dhead(), dlen(0), dtail(&dhead){}
	void addterm( string toke, int docid );

};

void Dictionary::addterm( string toke, int docid ){
	Term *cur = &dhead;
	while( cur->next != NULL ){
		if( cur->next->Getstr() == toke ){
			cur->next->Getlist().addnode( docid, 1 );
			break;
		}
		else if( cur->next->Getstr() >  toke ){
			Term* p = new Term;
			Term* q = cur->next;
			p->Setstr(toke);
			p->Getlist().addnode( docid, 1 );
			cur->next =  p;
			p->next = q;
			dlen ++;
			break;
		}
		cur = cur->next;
	}
	if( cur == dtail ){
		Term* p = new Term;
		p->Setstr(toke);
		p->Getlist().addnode( docid, 1 );
		cur->next = p;
		dtail = p;
		dlen++;
	}
}

void WriteTerm( ofstream &of, Term* t){
	//Node* ncur = &t->Getlist().Gethead();
	//	of<<t->Getstr()<<' '<<t->Getlist().Getdf()<<' ';
	//	while( ncur->Getnext() != NULL ){
	//		of<<ncur->Getnext()->GetID()<<' '<<ncur->Getnext()->Gettf()<<' ';
	//		ncur = ncur->Getnext();
	//	}
	//	of<<'\n';
  of << t->Getstr() << ' ';
  uint32_t *arr = t->Getlist().list2Array();
  GammaEncoder gammEncoder(arr, t->Getlist().Getdf());
  unsigned char *codes = gammEncoder.encode();
  uint32_t codeSize = gammEncoder.getCodesSize();
  of << codeSize << ' ';
  for (uint32_t i = 0; i < codeSize; i++) {
    of << codes[i] << ' ';
  }
  of << '\n';
  free(arr);
  free(codes);
}

void WriteTermDebug( ofstream &of, Term* t, bool write_df=false){
	Node* ncur = &t->Getlist().Gethead();
		of<<t->Getstr()<<' ';
    if (write_df) {
      of<<t->Getlist().Getdf() << ' ';
    }
		while( ncur->Getnext() != NULL ){
			of<<ncur->Getnext()->GetID()<<' ';
			ncur = ncur->Getnext();
		}
		of<<'\n';
}

void WriteBlock( ofstream &of, Dictionary dict ){
	Term* tcur = &dict.dhead;
  static uint32_t i = 0;
	while( tcur->next != NULL ){
		WriteTerm(of, tcur->next);
		//WriteTermDebug(of, tcur->next);
		tcur = tcur->next;
	}
	of.close();
}

Term ReadTerm(stringstream &in)
{
	string toke;
  uint32_t codeSize;
	int docid,tf;
	Term dterm;
	in >> toke >> codeSize;
	dterm.Setstr(toke);
  // Decode
  unsigned char *codes = (unsigned char*)malloc(codeSize*sizeof(uint8_t));
  for (uint32_t i = 0; i < codeSize; i++) {
    in >> codes[i];
  }
  GammaDecoder gammDecoder(codes, codeSize);
  uint32_t *doc_list = gammDecoder.decode();
  uint32_t df = gammDecoder.getValuesNum();
	dterm.Getlist().Setdf(df);
	for (uint32_t i = 0; i < df; i++) {
		dterm.Getlist().addnode(doc_list[i], 1);
	}
  // release room
  free(codes);
  free(doc_list);

	return dterm;
//	string toke;
//	int df;
//	int docid,tf;
//	Term dterm;
//	in >> toke >> df;
//	dterm.Setstr(toke);
//	dterm.Getlist().Setdf(df);
//	while( in >> docid && in >> tf){
//		dterm.Getlist().addnode(docid, tf);
//	}
//	return dterm;
}

Term MergeList(Term t1, Term t2)
{
	Term t;
	Node * n1 = &t1.Getlist().Gethead();
	Node * n2 = &t2.Getlist().Gethead();
	while( n1 -> Getnext() != NULL ){
		t.Getlist().addnode(n1->Getnext()->GetID(), n1->Getnext()->Gettf() );
		n1= n1 -> Getnext();
	}
	while( n2 -> Getnext() != NULL ){
		t.Getlist().addnode(n2->Getnext()->GetID(), n2->Getnext()->Gettf() );
		n2 = n2 -> Getnext();
	}
	return t;
}

void MergeBlock(string s, int block_num, ofstream &of)
{
	int k = 0;
	ifstream block[5];
	int bp[5] = {0};
	Term buffer[5];

	for(k = 0; k < 5; k++){
		s[5] = char( k + '0' );
		block[k].open( s + ".txt", ios::in );
		string s;
		getline(block[k], s);
		stringstream in(s);
		buffer[k] = ReadTerm(in);
	}
	int doc_empty = 0x1F;
	while(doc_empty){
		Term t;
		string  min = buffer[0].Getstr();
		string minb = "10000";
		for( int i = 1; i < 5; i++ ){
			if(min > buffer[i].Getstr()){
				min = buffer[i].Getstr();
				minb = "00000";
				minb[i] = '1';
			}
			else if( min == buffer[i].Getstr() ){
				minb[i] = '1';
			}
		}
		for( int i = 0; i < 5; i++ ){
			if( minb[i] == '1' ){
				t = MergeList( t, buffer[i] );
				string s;
				if( getline(block[i], s )){
					stringstream in(s);
					buffer[i] = ReadTerm(in);
				}
				else{
					buffer[i].Setstr(INF);
					doc_empty &= 0xEF>>i;
				}
			}
		}
		t.Setstr(min);
		WriteTermDebug( of, &t);
	}
}

int main()
{
	int k=0;
	int tsize = 0;
	int flag = 1;
	int dblocknum = 0;

	string s = "blockn";
	ifstream token_union;
	token_union.open("token_union.txt", ios::in);
	ofstream InvertedIndex;
	InvertedIndex.open("InvertedIndex.txt", ios::out);
	//InvertedIndex.close();
	//InvertedIndex.open("InvertedIndex.txt", ios::app|ios::out);

	Dictionary dict;


	while(flag)
	{
		s[5] = char( dblocknum + '0' );
		ofstream  dblock;
		dblock.open( s+".txt", ios::out );
		tsize = 0;
		Dictionary bdict;
		while(tsize < 5000){
			string toke;
			int docid;
			if( token_union>>toke && token_union>>docid){
				tsize ++;
				bdict.addterm(toke, docid);
			}
			else{
				flag = 0;
				break;
			}
			//cout<<sizeof(bdict)<<endl;
		}
		WriteBlock(dblock, bdict);
		dblocknum += 1;
	}
	MergeBlock(s, dblocknum, InvertedIndex);
	token_union.close();
	InvertedIndex.close();

	return 0;
}
