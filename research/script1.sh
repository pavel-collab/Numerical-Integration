#! /usr/bin/env bash

echo "start test"
# запускаем цикл
for ((i = 2; i <= 8; i+=1))
do
../build/main 0 1 x $i
done
echo "end"