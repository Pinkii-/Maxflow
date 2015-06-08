#!/bin/bash

cd benchmark/;
FILES=*

rm -rf ../EK.out ../EK.time;
rm -rf ../PFP.out ../PFP.time;

for f in $FILES
do
	echo -n $f" " >> ../EK.out
	./../mainEK < $f >> ../EK.out

	echo -n $f" " >> ../PFP.out
	./../mainPFP < $f >> ../PFP.out
done
