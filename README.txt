������˵���ĵ�

��������˼·���£�
ʹ��SPIMI�㷨�����Ƚ��������ݷ�Ϊ���ɸ��飬ÿ�����С�ܱ��ڴ�ȫ��װ�룬��ÿ����ά���ʵ�͵���������Ȼ��ϲ������顣���ʵ�͵�������ͨ�����롢ѹ�������̣�Ȼ��Ӵ��ָ̻�ȫ�ִʵ��ȫ�ֵ������������������������ؽ����

���������������º���ʵ�֣�
Tokenstream()���ִʣ�Ϊ����������������׼����
indexer()��main������ڣ�ʵ�ֽ��顢�ϲ���Ĺ��ܣ�
singlestringcomp()����ȫ�ִʵ���е��ַ���ѹ������ѹ�����־û������̣�
gamma()����ȫ�ֵ�����������gamma���뼰�־û���
find()��ʵ�ֲ�����ѯ��


��Indexer main()������ʼ������ָ������װ·����Ȼ�����indexer()���������ĵ�����ַ���롣

�����empty_stream��ʼֵΪFalse������TokenStream��������find��ʼֵΪNone��
Takenstream()����

��ʼ����������term_cnt��document_numͳ�ƴ����������ĵ�������

һ������������

��SPIMI�㷨���������飬��������־û������̡�
�������������spimi_invert��������������־û������̵���write_index_block������spimi_invert�������£�

�����ݷֿ飬Ϊÿ����ά��һ���ֵ�͵��ż�¼��������ֱ�����С�����趨�Ŀ��С����������дʡ��������Ĵʲ��ڴʵ��У��Ͱ�������ʵ䣬����������1������ͷ������ڴʵ��е�λ�á�����ô����ڵ��ĵ�ID���ڵ��ż�¼����ͰѸ��ĵ�ID���뵹�ż�¼����Ĵ�С��1.
���У�����get_next_term_pos_docid��������

Write_index_block�������£�

���ȶ��ֵ����򣬽��������ֵ�Ԫ�طŵ��µ��б�temp_lst�����Singlestringcomp.write_dic�������ֵ�д����̣�Singlestringcomp.write_dic�ֵ���Singlestringcomp.sscompress�������ֵ���е�һ�ַ���ѹ�����ﵽ����һ�ַ���ѹ������ֵ�д����̵�Ŀ�ġ�
Ȼ�����Gamma.write_invert_index_noencode������δgammaѹ���ĵ��ż�¼��д����̣���ѹ����������ֵ��Ӧ��
Singlestringcomp.write_dic������Singlestringcomp.sscompress�������£�


���ˣ����������鲿����ɡ�

�����ϲ�������

ʹ��merge_index_block�����ϲ������顣

���ȴ�ԭ���־û���������ȡ�����������ʹ��

ά��һ����С�Ѷ��С�
�����������£�

�������ΰ�ÿ���ĵ��ʵ���뵽���ȼ����У�����ʵ�ϲ���ϣ��ͽ����ϲ���

�����С����ҵ������Ӧ�ĵ��ż�¼���ϲ���ͬ���ĵ�ID��ȡ����Ӧ���ż�¼���ж�Ӧ�����docID�б������ǰID����docID�б��У��ͽ�����롣

�ϲ���������ɺ�õ�ȫ�ִʵ��ȫ�ֵ��ż�¼����һ��Ҫ�־û����ַ���ѹ����ȫ�ִʵ��gamma����ѹ����ȫ�ֵ��ż�¼��
����Singlestringcomp.write_dic����ʵ�ֳ־û����ַ���ѹ����ȫ�ִʵ䡣ǰ���Ѿ�˵�����ڴ˲���׸����
����Gamma.write_invert_index_encode����ʵ�ֳ־û�gamma����ѹ����ȫ�ֵ��ż�¼��Gamma.write_invert_index_encode�������£�

���ˣ������ݵ�����������ɡ�

������ѯ����

�������е�����Singlestringcomp.read_dic��������ȡ�ո�ѹ����ȫ�ִʵ䡣Singlestringcomp.read_dic��������Singlestringcomp.ssdecompress��������ѹ���Ĵʵ��н�ѹ��ԭ���ʵ�����£�

����������Gamma.read_invert_index_decode�����Ӵ����ж�ȡȫ�ֵ��ż�¼��

����Gamma.entity_gamma_decode������gamma��������д����docID�б���н��롣����ʵ�����£�


���е���gamma_decode����Ϊ����docID���롣
Gamma_decode������

�ٴλص������������docID���ĵ������ֵ䡣ʹ��Find����ʵ�ֲ�����ѯ��˳��Ϊ�����ҡ�

fd.findΪ��ѯ�����������عؼ��ֵĵ��ż�¼��fd.find�������£�

fd.show_result���������˲�ѯ����ķ��ط�ʽ��


�ġ���������
1.python v3.6 , package re, sci-kit;
2.gcc v12;
3.perl;
4.Indri v5.11


�塢���з�ʽ
1. �ֲ���������
1.1 ����Ԥ����
�����ļ���CiteSegmentation.py    		
���з�ʽ��$ python CiteSegmentation.py
�����ļ���shakespeare-merchant.trec/shakespeare-merchant.trec.1
shakespeare-merchant.trec/shakespeare-merchant.trec.2
����ļ���token_union.txt������������ statistic.txt���ĵ�ͳ������	
	
1.2 SPIMI��������
�����ļ���SPIMI.cpp��gammaCode.hpp
���з�ʽ��$ g++ -o SPIMI SPIMI.cpp
	 $ ./SPIMI
�����ļ���token_union.txt
����ļ���blocki.txt��InvertIndex.txt

1.3 Bool��ѯ
�����ļ���btree.cpp��gammaCode.hpp
���з�ʽ��$ g++ -o search.o search.cpp
         $ ./search.o
�����ļ���InvertedIndex.txt
����ļ���������print������ѯ��ĵ���������

2. ���в������

�����ļ���rootpath/Indexer/Run_test.sh, rootpath/Indexer/Makefile 
���з�ʽ��
	$ make clean
	$ make all
	$ bash Run_test.sh
�����linux������ִ��


