import struct as st
import os
import re

if __name__ == '__main__':

	principle1 = re.compile(r'\W')              # Translate trecs from all types of character to only letters and numbers
	principle2 = re.compile(r'\s+')
	p_sub = re.compile(r'</?.*?>')


	doc_num = 0
	doc_len = []
	doc_avgl = 0
	docname = {}
	doctitle = {}

	token_set = []
	token_num = 0
	token_avgl = 0
	token_lmax = 0

	cites_dict = {}
	cites_union = []

	# Write infos into text
	token_union = open( 'token_union.txt', 'w')
	# InvertedIndex_txt = open( 'InvertedIndex.txt', 'w')
	statistic = open( 'statistic.txt', 'w')

	# Read in files
	file1 = open('shakespeare-merchant.trec/shakespeare-merchant.trec.1', 'r')
	file2 = open('shakespeare-merchant.trec/shakespeare-merchant.trec.2', 'r')
	trec = file1.read() + file2.read()
	trec = trec.lower()                          # Trec stores the whole docs

	pcur = 0;


	while( pcur < len(trec) - 1 ):
		idx1  = trec.find('<doc>', pcur)         # Split doc
		idxn1 = trec.find('<docno>', pcur)       # Split docno
		idxn2 = trec.find('</docno>', pcur)      
		idxt1 = trec.find('<title>', pcur)       # Split title
		idxt2 = trec.find('</title>', pcur)
		idx2  = trec.find('</doc>', idxn2 + 1)


		if( idx1 == -1 ): 
			break;
		elif( ( idxt1 == -1 ) | ( idxt1 > idx2 ) ):
			idxt2 = idxn2;
			idxt1 = idxt2 - 8

		pcur = idx2 + 1
		docname[doc_num] = trec[ idxn1 + 7 : idxn2 ]
		doctitle[doc_num] = trec[ idxt1 + 8 : idxt2 ]

		# Process Contents
		doc = trec[ idxn2 + 8 : idx2 ]

## Filter
		doc = p_sub.sub(r' ', doc)
		doc = principle1.sub(r' ', doc)
		doc_len.append(0)
		token_set = principle2.split(doc)

		for token in token_set:
			if token != '':
				token_num += 1
				doc_len[doc_num] += 1
				token_avgl += len(token)
				token_lmax = max(token_lmax,len(token))

## Token Save
				token_union.write(str(token))
				token_union.write(' ')
				token_union.write(str(doc_num))
				token_union.write('\n')

## Dictionary Generate
		cites_dict[doc_num] = list(set(token_set))
		cites_dict[doc_num].sort()
		cites_union = list(set(cites_union).union(set(cites_dict[doc_num])))
		doc_num += 1
	#cites_union.sort()


## Statistic
	token_avgl /= token_num
	doc_avgl = token_num /doc_num

	statistic.write('Term Number          :  '  + str(len(cites_union)) + '\n')
	statistic.write('Doc Number           :  '  + str(doc_num)          + '\n')
	statistic.write('Token Number         :  '  + str(token_num)        + '\n')
	statistic.write('Doc Average Length   :  '  + str(doc_avgl)         + '\n')
	statistic.write('Token Average Length :  '  + str(token_avgl)       + '\n')
	statistic.write('Token Max Length     :  '  + str(token_lmax)       + '\n')
	statistic.write('Doc Title            :  '  + str(doctitle)         + '\n')
	statistic.write('Doc Name             :  '  + str(docname)          + '\n')

	print('Term Number          :  '  + str(len(cites_union)) + '\n')
	print('Doc Number           :  '  + str(doc_num)          + '\n')
	print('Token Number         :  '  + str(token_num)        + '\n')
	print('Doc Average Length   :  '  + str(doc_avgl)         + '\n')
	print('Token Average Length :  '  + str(token_avgl)       + '\n')
	print('Token Max Length     :  '  + str(token_lmax)       + '\n')


## Dictionary Save
	# InvertedIndex = {}
	# for v in cites_union:
	# 	InvertedIndex[v] = []
	# for k, cites in cites_dict.items():
	# 	for cit in cites:
	# 		InvertedIndex[cit].append(k)

	# del InvertedIndex['']            # delete empty cite

	# for cites, clist in InvertedIndex.items():
	# 	InvertedIndex_txt.write(cites)
	# 	InvertedIndex_txt.write(' ')
	# 	for k in range(len(clist)):
	# 		InvertedIndex_txt.write(str(clist[k]))
	# 		InvertedIndex_txt.write(' ')
	# 	InvertedIndex_txt.write('\n')

	# print(cites_dict)
	# print(len(doctitle))
	# print(InvertedIndex)
	# print(doctitle)
	# print(docname)

	token_union.close()
	# InvertedIndex_txt.close()
	statistic.close()