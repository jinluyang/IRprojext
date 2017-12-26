import os 

def pars_summary(line):
	return line[9:-10]

topic = 'topics2014.xml'

def extract(query):
	# query is a list
	res = []
	for q in query:
		q = q.replace('.','')
		q = q.replace('-',' ')
		q = q.replace(',',' ')
		q = q.replace("'",' ')
		res.append(q)
	return res

with open(topic) as f:
	lines = f.readlines()
	querys = []
	for line in lines:
		line = line.strip()
		if line.startswith('<summary>'):
			querys.append(pars_summary(line))
			

print(len(querys))
querys = extract(querys)

query_head = '''<parameters>
<index>../../index</index>
<count>100</count>
'''

#<text>to do</text>
query_tail = '''<trecFormat>true</trecFormat>
<runID>mymodel</runID>
</parameters>'''
query_mid = ''
for q in querys:
	query_mid += '<query><text>#combine(' + q + ')</text></query>\n' 
#	os.system('''IndriRunQuery.exe query_param.txt -query="'''+q+'''" >>res.txt''')

with open('query_parameter.txt','w') as f:
	f.write(query_head+query_mid+query_tail)