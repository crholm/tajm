#!/bin/bash

 ## Download
rm -r ./data
mkdir -p data/latest
mkdir -p tmp/zic
cd tmp
curl 'ftp://ftp.iana.org/tz/tzdata-latest.tar.gz' > tzdata-latest.tar.gz
tar -xvf tzdata-latest.tar.gz

## zic
FILES='africa antarctica asia australasia etcetera europe northamerica southamerica pacificnew backward'

for name in $FILES
do 
echo Compiling $name
zic -d zic $name
done

mv ./zic/* ../data/latest
cd ..
rm -r tmp

