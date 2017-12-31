索引器说明文档

本索引器思路如下：
使用SPIMI算法，首先将所有数据分为若干个块，每个块大小能被内存全部装入，对每个块维护词典和倒排索引，然后合并索引块。将词典和倒排索引通过编码、压缩到磁盘，然后从磁盘恢复全局词典和全局倒排索引，最后进行搜索，返回结果。

本索引器采用以下函数实现：
Tokenstream()：分词，为后续构建索引块做准备；
indexer()：main函数入口，实现建块、合并块的功能；
singlestringcomp()：对全局词典进行单字符串压缩及解压，并持久化到磁盘；
gamma()：对全局倒排索引进行gamma编码及持久化；
find()：实现布尔查询。


从Indexer main()函数开始，首先指定程序安装路径，然后调用indexer()函数，将文档集地址传入。

设变量empty_stream初始值为False，调用TokenStream函数。设find初始值为None。
Takenstream()函数

初始化，定义了term_cnt和document_num统计词项数量和文档数量。

一、构建索引块

用SPIMI算法构建索引块，将索引块持久化到磁盘。
构建索引块调用spimi_invert函数，将索引块持久化到磁盘调用write_index_block函数。spimi_invert函数如下：

将数据分块，为每个块维护一个字典和倒排记录表。读入词项，直到块大小超过设定的块大小，或读完所有词。如果读入的词不在词典中，就把它加入词典，词项数量加1，否则就返回它在词典中的位置。如果该词所在的文档ID不在倒排记录表里，就把该文档ID加入倒排记录表，块的大小加1.
其中，调用get_next_term_pos_docid函数如下

Write_index_block函数如下：

首先对字典排序，将排序后的字典元素放到新的列表temp_lst里，调用Singlestringcomp.write_dic函数将字典写入磁盘，Singlestringcomp.write_dic又调用Singlestringcomp.sscompress函数将字典进行单一字符串压缩，达到将单一字符串压缩后的字典写入磁盘的目的。
然后调用Gamma.write_invert_index_noencode函数把未gamma压缩的倒排记录表写入磁盘，与压缩的排序的字典对应。
Singlestringcomp.write_dic函数和Singlestringcomp.sscompress函数如下：


至此，构建索引块部分完成。

二、合并索引块

使用merge_index_block函数合并索引块。

首先从原来持久化的索引块取出倒排索引和词项。

维护一个最小堆队列。
具体做法如下：

按序依次把每个文档词典加入到优先级队列，如果词典合并完毕，就结束合并。

输出最小词项，找到词项对应的倒排记录表，合并相同的文档ID。取出对应倒排记录表中对应词项的docID列表，如果当前ID不在docID列表中，就将其加入。

合并索引块完成后得到全局词典和全局倒排记录表，下一步要持久化单字符串压缩的全局词典和gamma编码压缩的全局倒排记录表。
调用Singlestringcomp.write_dic函数实现持久化单字符串压缩的全局词典。前面已经说过，在此不再赘述。
调用Gamma.write_invert_index_encode函数实现持久化gamma编码压缩的全局倒排记录表。Gamma.write_invert_index_encode函数如下：

至此，对数据的索引基本完成。

三、查询部分

主函数中调用了Singlestringcomp.read_dic函数来读取刚刚压缩的全局词典。Singlestringcomp.read_dic函数调用Singlestringcomp.ssdecompress函数来从压缩的词典中解压出原词项。实现如下：

主函数调用Gamma.read_invert_index_decode函数从磁盘中读取全局倒排记录表。

调用Gamma.entity_gamma_decode函数对gamma编码的所有词项的docID列表进行解码。具体实现如下：


其中调用gamma_decode函数为单个docID解码。
Gamma_decode函数：

再次回到主函数。获得docID和文档名的字典。使用Find函数实现布尔查询，顺序为从左到右。

fd.find为查询主函数，返回关键字的倒排记录表。fd.find函数如下：

fd.show_result函数定义了查询结果的返回方式。


四、环境配置
1.python v3.6 , package re, sci-kit;
2.gcc v12;
3.perl;
4.Indri v5.11


五、运行方式
1. 分部调试运行
1.1 语料预处理
程序文件：CiteSegmentation.py    		
运行方式：$ python CiteSegmentation.py
输入文件：shakespeare-merchant.trec/shakespeare-merchant.trec.1
shakespeare-merchant.trec/shakespeare-merchant.trec.2
输出文件：token_union.txt（词条集）， statistic.txt（文档统计量）	
	
1.2 SPIMI索引构建
程序文件：SPIMI.cpp、gammaCode.hpp
运行方式：$ g++ -o SPIMI SPIMI.cpp
	 $ ./SPIMI
输入文件：token_union.txt
输出文件：blocki.txt，InvertIndex.txt

1.3 Bool查询
程序文件：btree.cpp、gammaCode.hpp
运行方式：$ g++ -o search.o search.cpp
         $ ./search.o
输入文件：InvertedIndex.txt
输出文件：命令行print布尔查询后的倒排索引表

2. 运行测试输出

程序文件：rootpath/Indexer/Run_test.sh, rootpath/Indexer/Makefile 
运行方式：
	$ make clean
	$ make all
	$ bash Run_test.sh
输出：linux命令行执行


