#!/usr/bin/env sh
#*******RUN*******#
# This script is writed for run the indexer
Indexer_Path=Indexer/
# DataSet Detecting
for fname in shakespeare-merchant.trec.1 shakespeare-merchant.trec.2
do
        if [ ! -e shakespeare-merchant.trec/$fname ]; then
		echo "DataSet is lack of" $fname
		exit 1
	fi
done

echo "Dataset Prepared"

## Indexer Testing

echo "Indexer Testing Begining"

#Tonkenization
echo "DataSet Tokenization..."
set Encoding="utf-8"
python CiteSegmentation.py
echo "Tokenization Done"

# SPIMI
echo "SPIMI InvertIndex Generating..."
./SPIMI
echo "SPIMI Done"

# Bool Query
echo "Bool Query Ing..."
./search.o
echo "Bool Query Done"

echo "Indexer Testing Done"

