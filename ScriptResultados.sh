#!/bin/bash

FILES=Benchmark/*.air

for f in $FILES
do
echo -n $f ""| sed -r 's/^.{10}//' >> $1
./practicaVersio1 -n < $f >> $1
done
