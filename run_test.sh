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

## Indexer Testing
echo "Indexer testing begins."
echo ">>>>>>>>Dataset preparing......"
#Tonkenization
echo "DataSet Tokenization..."
echo "---------- Statistics Show ----------"
set Encoding="utf-8"
python CiteSegmentation.py
echo "-------------------------------------"
echo "Tokenization done."
# SPIMI
echo ">>>>>>>>SPIMI InvertIndex generating..."
./SPIMI
echo "SPIMI done."
# Bool Query
echo ">>>>>>>>Bool query:"
echo ""
./search
echo ""
echo "Bool query done."
echo "Indexer Testing Done"

