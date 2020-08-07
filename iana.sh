#!/bin/bash

 ## Download
rm -r ./data/iana-latest
mkdir -p ./data/iana-latest
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

mv ./zic/* ../data/iana-latest
cd ..
rm -r tmp

bsb
rm /home/raz/workspace/re-time/src/iana/lib/Tajm_Iana_Lib*.re
node lib/js/scripts/Generate_Iana.js
bsb
