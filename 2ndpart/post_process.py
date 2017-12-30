# coding=<utf-8>
# prefix = 'D:\\研究生\\2016年秋季学期课件打包（现代信息检索）\\pmc-text-00\\00\\'
import sys

lenth = 44

# file = 'trec_file.txt'
file = sys.argv[1]

with open(file) as input:
	lines = input.readlines()
	
with open('trec_result/'+file,'w') as output:
	for line in lines:
		line = line.split()
		line[2] = (line[2][lenth:-len('.nxml')])
		output.write(' '.join(line)+'\n')