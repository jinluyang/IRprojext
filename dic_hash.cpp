#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <stdio.h>
#include <string.h>
//#include<conio.h>

#define Max_num_words 10000

using namespace std;

//static string result;
//bool SearchWord(string word);

struct DocLIST		//索引表
{
	int DocId;
	DocLIST *next;
};

struct dictnode
{
	string word;  //单词
	//unsigned int df;//文档频率
	DocLIST *ListHead;//倒排索引表头
};

struct TermBlock
{
	string term;
	//unsigned int df;
	DocLIST *ListHead;
	TermBlock *next;
};

struct HashNode    //hash表内每个节点的数据结构
{
	dictnode word;
	HashNode *next;
};

HashNode *hash_table[Max_num_words];

unsigned int ELFhash(string str)//获得键值，采用位运算哈希形式
{
	unsigned int hash_key = 0;
	unsigned int x = 0;
	for(int i = 0;i<str.size();++i)
	{
		hash_key = (hash_key << 4) + str[i];
		if ((x = hash_key & 0xf0000000) != 0)
		{
			hash_key ^= (x >> 24);   //影响5-8位，杂糅一次   
			hash_key &= ~x;   //清空高四位   
		}
	}
	return (hash_key & 0x7fffffff);
}

void CapToLower(string &s)
{
	for (unsigned int i = 0; i < s.length(); i++)
	{
		s[i] = tolower(s[i]);
	}
}

//添加结点
unsigned int AddHashNode(string word, /*string line*//*, int df,*/ DocLIST *ListHead) 
{
	unsigned int finish_label = 0;
	HashNode *p = new HashNode;
	p->word.word = word;
	//p->df = df;
	p->word.ListHead = ListHead;
	p->next = NULL;
	int n = ELFhash(word) % Max_num_words;
	if (!hash_table[n])		//直接散列
	{
		hash_table[n] = p;
		finish_label = 1;
	}
							//冲突处理,向后链表
	else
	{
		HashNode *p2;
		p2 = hash_table[n];
		while (p2->next)
		{
			p2 = p2->next;
		}
		p2->next = p;
		finish_label = 1;
	}
	return finish_label;
}

void addDic(string filename)
{
	//string filename = "test.txt"; 
	ifstream in(filename);
		if (!in)
		{
			cout << "文件不存在！" << endl;
//			getch();
//			exit(-1);
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
		DocLIST *Head = new DocLIST;
		DocLIST *Tail = Head;
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

			DocLIST *Lnew = new DocLIST;		//文档ID链表

			Lnew->DocId = value;	
			Tail->next = Lnew;
			Lnew->next = NULL;
			Tail = Lnew;
		}
		AddHashNode(word, Head);
	}
}


//循环删除结点
void DeleteHashNode(HashNode *p)
{
	HashNode *tmp = p;
	while (p)
	{
		tmp = p->next;
		free(p);
		p = tmp;
	}

	if (p)
	{
		free(p);
		p = NULL;
	}
	
	/*if (p->next)
		DeleteHashNode(p->next);
	else p = NULL;*/
}

//遍历删除链表

void deleteList()
{
	for (int i = 0; i < Max_num_words; i++)
	{
		if (hash_table[i])
		{
			DeleteHashNode(hash_table[i]);
		}
	}
}

//查询输出，返回单词对应的文档ID链表

DocLIST* SearchDocId(string word)
{
	DocLIST *doc_result;
	int n = ELFhash(word) % Max_num_words;
	if (!hash_table[n])
	{
		return false;
	}
	if (hash_table[n]->word.word == word)
	{
		doc_result = hash_table[n]->word.ListHead;
		return doc_result;
	}
	HashNode *p2;
	p2 = hash_table[n]->next;
	while (p2 && p2->word.word != word)
	{
		if (p2->next)
			p2 = p2->next;
		else
		{
			p2 = p2->next;
			break;
		}
	}
	if (!p2)
	{
		return NULL;
	}
	doc_result = p2->word.ListHead;
	return doc_result;
}

//*******************************************************************
//**Description：                                                   *
//**input：部分倒排记录表，查询词                                   *
//**output:查询词的倒排记录表                                       *
//**输入一个部分倒排记录表的串，以及要查询的词                      *
//**对倒排记录表块进行hash，hash空间设定为10000，建议单词不超过1000 *
//**查询完释放hash的空间，便于对下一个块的查询                      *
//**建议以该形式进行输入输出                                        *
//**否则，当调用AddHashNode()逐点进行hash后调用SearchDocId()查询    *
//*******************************************************************

DocLIST* BlockHash(string term,TermBlock* BlockHead)		
{
	TermBlock *Term_tmp = BlockHead->next;
	while(Term_tmp)
	{
		AddHashNode(Term_tmp->term,Term_tmp->ListHead);
		Term_tmp = Term_tmp->next;
	}

	DocLIST *DocIDList = SearchDocId(term);
	if (!SearchDocId(term))
		{
			cout << "未找到单词，请检查拼写！\n";
		}
	deleteList();
	return DocIDList;
}

//词典压缩，未做完
//void dic_compress(string filename)
//{
//	ifstream in(filename);
//	if (!in)
//	{
//		cout << "文件不存在！" << endl;
//		getch();
//		exit(-1);
//	}
//	ofstream out("Compressed_Dictionary.txt",ios::trunc);
//	int lineNumber = 0;
//	string line,word,df;
//	streampos sp[Max_num_words]; 
//	while (in)
//	{
//		lineNumber++;
//		getline(in, line);
//		stringstream ss;
//		ss << line;
//		ss >> word;
//		sp[lineNumber] = out.tellp();
//	}
//
//	out<<word;
//}

int main()
{
	//string word;   //要查找的单词
	//cout << ">>正在添加词典，请稍后……"<<endl;
	//addDic("InvertedIndex1.txt");
	//cout << ">>词库添加完毕！" << endl;
	//cout << "\n请输入要查找的单词：";
	//cin >> word;
	//while (word != "#")
	//{
	//	CapToLower(word);
	//	if (!SearchDocId(word))
	//	{
	//		cout << "未找到单词，请检查拼写！\n";
	//	}
	//	else
	//	{
	//		DocLIST *result_out = SearchDocId(word)->next;
	//		while(result_out)
	//		{
	//			cout<<result_out->DocId<<" ";
	//			result_out = result_out->next;
	//		}
	//		cout<<endl;
	//	}
	//		//cout <<endl<<">> "<< result << endl;
	//	cout << "\n请输入要查找的单词：";
	//	cin >> word;
	//}

	//cout<<"正在清除词典，请稍后……"<<endl;
	//deleteList();
	//cout<<"词典清除完毕，程序退出！"<<endl;

	//生成测试的块，并用于查询，真正实验时数据块由文件解压后读入
	TermBlock *testHead = new TermBlock;
	//testHead->next = NULL;
	TermBlock *p = testHead;
	for (int i = 0; i<5 ; i++)
	{
		TermBlock *p1 = new TermBlock;
		p1->term = i+97;
		p1->ListHead = new DocLIST;
		p1->ListHead->next = new DocLIST;
		p1->ListHead->next->DocId = i;
		p1->ListHead->next->next = NULL;
		p1->next = NULL;
		testHead->next = p1;
		testHead = testHead->next;
	}
	string word;   //要查找的单词
	cout << "请输入要查找的单词：";
	cin >> word;

	if (BlockHash(word,p))
	{
		DocLIST *result_out = BlockHash(word,p)->next;
		while(result_out)
			{
				cout<<result_out->DocId<<" ";
				result_out = result_out->next;
			}
			cout<<endl;
	}

	return 0;
}
