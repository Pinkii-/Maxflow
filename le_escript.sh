#!/bin/bash

cd benchmark/;
FILES=*

for f in $FILES
do
	echo $f `./../mainEK < $f` >> ../EK.out
	echo $f `./../mainPFP < $f` >> ../PFP.out
done
