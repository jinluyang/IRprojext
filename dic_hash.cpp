#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <stdio.h>
#include <string.h>
#include<conio.h>

#define Max_num_words 10000

using namespace std;

//static string result;
//bool SearchWord(string word);

struct DocLIST		//������
{
	int DocId;
	DocLIST *next;
};

struct dictnode
{
	string word;  //����
	//unsigned int df;//�ĵ�Ƶ��
	DocLIST *ListHead;//����������ͷ
};

struct TermBlock
{
	string term;
	//unsigned int df;
	DocLIST *ListHead;
	TermBlock *next;
};

struct HashNode    //hash����ÿ���ڵ�����ݽṹ
{
	dictnode word;
	HashNode *next;
};

HashNode *hash_table[Max_num_words];

unsigned int ELFhash(string str)//��ü�ֵ������λ�����ϣ��ʽ
{
	unsigned int hash_key = 0;
	unsigned int x = 0;
	for(int i = 0;i<str.size();++i)
	{
		hash_key = (hash_key << 4) + str[i];
		if ((x = hash_key & 0xf0000000) != 0)
		{
			hash_key ^= (x >> 24);   //Ӱ��5-8λ������һ��   
			hash_key &= ~x;   //��ո���λ   
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

//��ӽ��
unsigned int AddHashNode(string word, /*string line*//*, int df,*/ DocLIST *ListHead) 
{
	unsigned int finish_label = 0;
	HashNode *p = new HashNode;
	p->word.word = word;
	//p->df = df;
	p->word.ListHead = ListHead;
	p->next = NULL;
	int n = ELFhash(word) % Max_num_words;
	if (!hash_table[n])		//ֱ��ɢ��
	{
		hash_table[n] = p;
		finish_label = 1;
	}
							//��ͻ����,�������
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
			cout << "�ļ������ڣ�" << endl;
			getch();
			exit(-1);
		}
	string line,word,word1; 
	int num = 0;
	while(in)
	{
		getline(in, line);

		string::iterator it;

		//ȥ�����ţ�ð�ţ����ţ����ŵȷǱ�Ҫ�ַ�
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
		ss >> word;		//��ȡ������
		
		//cout<<word<<endl;
		int j = 0;
		int value = 0;
		DocLIST *Head = new DocLIST;
		DocLIST *Tail = Head;
		while(j<line.size())	//��ȡ�ĵ���Ŵ�
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

			DocLIST *Lnew = new DocLIST;		//�ĵ�ID����

			Lnew->DocId = value;	
			Tail->next = Lnew;
			Lnew->next = NULL;
			Tail = Lnew;
		}
		AddHashNode(word, Head);
	}
}


//ѭ��ɾ�����
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

//����ɾ������

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

//��ѯ��������ص��ʶ�Ӧ���ĵ�ID����

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
//**Description��                                                   *
//**input�����ֵ��ż�¼����ѯ��                                   *
//**output:��ѯ�ʵĵ��ż�¼��                                       *
//**����һ�����ֵ��ż�¼��Ĵ����Լ�Ҫ��ѯ�Ĵ�                      *
//**�Ե��ż�¼������hash��hash�ռ��趨Ϊ10000�����鵥�ʲ�����1000 *
//**��ѯ���ͷ�hash�Ŀռ䣬���ڶ���һ����Ĳ�ѯ                      *
//**�����Ը���ʽ�����������                                        *
//**���򣬵�����AddHashNode()������hash�����SearchDocId()��ѯ    *
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
			cout << "δ�ҵ����ʣ�����ƴд��\n";
		}
	deleteList();
	return DocIDList;
}

//�ʵ�ѹ����δ����
//void dic_compress(string filename)
//{
//	ifstream in(filename);
//	if (!in)
//	{
//		cout << "�ļ������ڣ�" << endl;
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
	//string word;   //Ҫ���ҵĵ���
	//cout << ">>������Ӵʵ䣬���Ժ󡭡�"<<endl;
	//addDic("InvertedIndex1.txt");
	//cout << ">>�ʿ������ϣ�" << endl;
	//cout << "\n������Ҫ���ҵĵ��ʣ�";
	//cin >> word;
	//while (word != "#")
	//{
	//	CapToLower(word);
	//	if (!SearchDocId(word))
	//	{
	//		cout << "δ�ҵ����ʣ�����ƴд��\n";
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
	//	cout << "\n������Ҫ���ҵĵ��ʣ�";
	//	cin >> word;
	//}

	//cout<<"��������ʵ䣬���Ժ󡭡�"<<endl;
	//deleteList();
	//cout<<"�ʵ������ϣ������˳���"<<endl;

	//���ɲ��ԵĿ飬�����ڲ�ѯ������ʵ��ʱ���ݿ����ļ���ѹ�����
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
	string word;   //Ҫ���ҵĵ���
	cout << "������Ҫ���ҵĵ��ʣ�";
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