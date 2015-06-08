#!/bin/bash

cd benchmark/;
FILES=*

rm -rf ../EK.out ../EK.time;
rm -rf ../PFP.out ../PFP.time;

for f in $FILES
do
	echo -n $f" " >> ../EK.out
	/usr/bin/time -f "%e" ./../mainEK < $f >> ../EK.out 2>> ../EK.time

	echo -n $f" " >> ../PFP.out
	/usr/bin/time -f "%e" ./../mainPFP < $f >> ../PFP.out 2>> ../PFP.time
done
