import os

for k1 in [0.8,1.0,1.2,1.4]:
    for b in [0.1, 0.3, 0.5, 0.7, 0.9]:
        for k3 in [5,7,9]:
            print(k1,b,k3)
            with open('log_grid','a') as log:
                log.write('%s %s %s \n'%(k1,b,k3))
            os.system('./sample_eval.pl qrels-sampleval-2014.txt trec_result/trec_file_%s_%s_%s'%(k1,b,k3)+'>>log_grid')
