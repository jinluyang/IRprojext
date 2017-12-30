import os

for k1 in [0.8,1.0,1.2,1.4]:
	for b in [0.1, 0.3, 0.5, 0.7, 0.9]:
		for k3 in [5,7,9]:
			print(k1,b,k3)
			os.system('IndriRunQuery.exe query_parameter.txt -baseline=okapi,k1:%s,b:%s,k3:%s'%(k1,b,k3)+'>trec_file_%s_%s_%s'%(k1,b,k3))
			os.system('python post_process.py trec_file_%s_%s_%s'%(k1,b,k3))